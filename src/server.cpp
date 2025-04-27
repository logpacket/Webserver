//
// Created by Seongyun Jang on 3/17/25.
//

#include "server.h"

#include "file.h"
#include "logger.h"
#include "request.h"
#include "response.h"

#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <unistd.h>

Server::Server(const int port, const size_t max_cache_size) {
  this->port = port;
  this->cache = new Cache(max_cache_size); // 10 MB cache
  FD_ZERO(&master);
}

bool Server::init() {
  // Create server socket
  this->server_socket = socket(AF_INET, SOCK_STREAM, 0);

  // Check for errors
  if (this->server_socket == -1) {
    Logger::LOG_ERROR("Error creating socket");
    exit(1);
  }

  // Set socket to non-blocking
  const int flags = fcntl(this->server_socket, F_GETFL, 0);
  fcntl(this->server_socket, F_SETFL, flags | O_NONBLOCK);

  // Set socket options
  if (setsockopt(this->server_socket, SOL_SOCKET, SO_REUSEADDR, &flags,
                 sizeof(flags)) == -1) {
    Logger::LOG_ERROR("Error setting socket options");
    exit(1);
  }

  // Bind socket to port
  this->server_address.sin_family = AF_INET;
  this->server_address.sin_addr.s_addr = INADDR_ANY;
  this->server_address.sin_port = htons(this->port);

  if (bind(this->server_socket,
           reinterpret_cast<sockaddr *>(&this->server_address),
           sizeof(this->server_address)) == -1) {
    Logger::LOG_ERROR("Error binding socket");
    exit(1);
  }

  // Listen for incoming connections
  if (listen(this->server_socket, 10) == -1) {
    Logger::LOG_ERROR("Error listening on socket");
    exit(1);
  }

  // Add server socket to master set
  FD_SET(this->server_socket, &master);
  this->max_fd = this->server_socket;

  Logger::LOG_INFO("Server running on port " + std::to_string(this->port));

  return true;
}

void Server::wait_for_socket_ready(const int client_socket) {
  fd_set write_fds;
  FD_ZERO(&write_fds);
  FD_SET(client_socket, &write_fds);

  switch (
      select(client_socket + 1, nullptr, &write_fds, nullptr, &this->timeout)) {
  case -1:
    Logger::LOG_ERROR(
        "Error in select (send retry): " +
        string(system_error(errno, std::system_category()).what()));
    break;
  case 0:
    Logger::LOG_WARNING("Send retry timeout");
    break;
  default:
    break;
  }
}

void Server::run() {
  while (true) {
    // Copy master set to read set
    fd_set read_fds = master;

    // Wait for activity on any socket
    if (select(this->max_fd + 1, &read_fds, nullptr, nullptr, nullptr) == -1) {
      Logger::LOG_ERROR("Error in select");
      exit(1);
    }

    // Check all sockets for activity
    for (int i = 0; i <= this->max_fd; i++) {
      if (FD_ISSET(i, &read_fds)) {
        // If server socket has activity, accept new connection
        if (i == this->server_socket) {
          handle_new_connection();
        } else {
          // Handle client socket
          handle_client(i);
        }
      }
    }
  }
}

void Server::handle_new_connection() {
  sockaddr_in client_address{};
  socklen_t address_len = sizeof(client_address);

  // Accept new connection
  const int client_socket =
      accept(this->server_socket, reinterpret_cast<sockaddr *>(&client_address),
             &address_len);

  // Check for errors
  if (client_socket == -1) {
    Logger::LOG_ERROR("Error accepting connection");
  }

  // Set client socket to non-blocking
  const int flags = fcntl(client_socket, F_GETFL, 0);
  fcntl(client_socket, F_SETFL, flags | O_NONBLOCK);

#ifdef __APPLE__
  // 새 클라이언트 소켓에 SO_NOSIGPIPE 옵션 설정
  constexpr int set = 1;
  if (setsockopt(client_socket, SOL_SOCKET, SO_NOSIGPIPE, &set, sizeof(set)) ==
      -1) {
    Logger::LOG_ERROR("Error setting SO_NOSIGPIPE option");
  }

#endif

  // Add client socket to master set
  FD_SET(client_socket, &master);

  // Update max_fd if necessary
  if (client_socket > this->max_fd) {
    this->max_fd = client_socket;
  }

  Logger::LOG_INFO("New connection from " +
                   std::string(inet_ntoa(client_address.sin_addr)));
}

void Server::handle_client(const int client_socket) {
  char in_buffer[1024];
  const long bytes_received =
      recv(client_socket, in_buffer, sizeof(in_buffer), 0);

  // Check for errors
  if (bytes_received == -1) {
    if (errno == EAGAIN || errno == EWOULDBLOCK) {
      // 비차단 소켓의 일시적 상태, 무시해도 됨
      return;
    }
    Logger::LOG_ERROR("Error receiving data");
    close_session(client_socket);
    return;
  }

  // Check for closed connection
  if (bytes_received == 0) {
    Logger::LOG_INFO("Connection closed");
    close_session(client_socket);
    return;
  }

  // Check for end of request
  if (auto buffer = string(in_buffer, bytes_received);
      buffer.find("\r\n\r\n") != string::npos) {
    // Create request
    const Request request(buffer);
    handle_request(request, client_socket);
  }
}

void Server::handle_request(const Request &request, const int client_socket) {
  // Check keep-alive
  const bool keep_alive =
      request.headers.find(http::HTTPHeaders::CONNECTION)->second != "close";
  // Handle request
  Logger::LOG_INFO("Received request: " + request.path);

  // Check if request method is GET
  if (request.method != http::Method::GET) {
    // Create error response
    Response response("Method not allowed",
                      http::StatusCode::METHOD_NOT_ALLOWED,
                      {{"Content-Type", "text/html"}}, keep_alive);
    send_response(response, client_socket, keep_alive);
    return;
  }

  if (request.path == "/") {
    // Redirect to index.html
    Response response("", http::StatusCode::PERMANENT_REDIRECT,
                      {{"Location", +"/" + DEFAULT_INDEX}}, keep_alive);
    send_response(response, client_socket, keep_alive);
  }

  handle_get_request(request, client_socket, keep_alive);
}

void Server::handle_get_request(const Request &request, const int client_socket,
                                const bool keep_alive) {
  string path = DEFAULT_ROOT + request.path;

  // Load file
  try {
    const File file(path.c_str());
    // Check if request uses cache
    const bool use_cache =
        request.headers.find(http::HTTPHeaders::CACHE_CONTROL)->second !=
        "no-cache";

    char *data = nullptr;
    size_t size = 0;

    if (use_cache && file.size() < cache->get_max_size()) {
      // Check if file is in cache
      if (!this->cache->contains(request.path)) {
        this->cache->set(request.path, file);
      }

      // Get file from cache
      const auto [cached_data, cached_size] = this->cache->get(request.path);
      data = cached_data;
      size = cached_size;
    } else {
      // Read file directly
      size = file.size();
      data = new char[file.size()];
      file.read(data);
    }

    // Get Content-Type from path
    const size_t pos = request.path.find_last_of('.');
    string content_type = http::DEFAULT_MIME_TYPE;
    if (pos != string::npos)
      content_type = http::EXTENSION_TO_MIME[request.path.substr(pos)];

    Response response(string(data, size), http::StatusCode::OK,
                      {{http::HTTPHeaders::CONTENT_TYPE, content_type}},
                      keep_alive);
    send_response(response, client_socket, keep_alive);
    // Clean up
    if (!use_cache)
      delete[] data;

  } catch (const std::exception &e) {
    // Create error response
    Response response(e.what(), http::StatusCode::NOT_FOUND,
                      {{http::HTTPHeaders::CONTENT_TYPE, "text/html"}},
                      keep_alive);
    send_response(response, client_socket, keep_alive);
  }
}
ssize_t Server::send_with_retry(const int client_socket, const string &data) {
  short retry_count = 0;
  ssize_t result = 0;
  do {
    result = send(client_socket, data.c_str(), data.size(), send_option);
    if (errno == EAGAIN || errno == EWOULDBLOCK) {
      // Non-blocking mode, retry
      wait_for_socket_ready(client_socket);
      if (FD_ISSET(client_socket, &master) == 0)
        break;
    } else
      break;
  } while (result < 0 && ++retry_count < max_retries);

  if (retry_count == max_retries)
    Logger::LOG_WARNING("Max retries reached for sending data");

  if (result <= 0) {
    if (errno != EPIPE)
      Logger::LOG_WARNING(
          "Error sending data: " +
          string(system_error(errno, std::system_category()).what()));
    close_session(client_socket);
  }

  return result;
}

void Server::send_response(Response &response, const int client_socket,
                           const bool keep_alive) {
  Logger::LOG_INFO("Sending response: " + response.get_metadata());
  // Serialize response
  response.serialize();
  const string headers = response.get_headers();
  const string &body = response.get_body();

  if (body.size() < DEFAULT_BUFFER_SIZE - headers.size()) {
    const string serialized_response = headers + "\r\n\r\n" + body;
    send_with_retry(client_socket, serialized_response);
    return;
  }

  // Send headers
  if (send_with_retry(client_socket, headers) <= 0) {
    Logger::LOG_WARNING("Failed to send response headers");
  };

  // Send data in chunks
  size_t offset = 0;
  while (offset < body.size()) {
    // Calculate the size of the chunk
    const size_t chunk_size =
        min(Server::DEFAULT_BUFFER_SIZE, body.size() - offset);
    // Create the chunk & Send it
    string chunk = response.get_chunk(offset, chunk_size);
    const ssize_t send_size = send_with_retry(client_socket, chunk);
    if (send_size <= 0)
      return;
    // Update offset
    offset += send_size;
  }
  if (!keep_alive)
    close_session(client_socket);
}

void Server::close_session(const int fd) {
  close(fd);
  FD_CLR(fd, &master);
}

Server::~Server() {
  // Close server socket
  close(this->server_socket);
  // Delete cache
  delete this->cache;
  // Clear master set
  for (int i = 0; i <= this->max_fd; i++)
    if (FD_ISSET(i, &master))
      close(i);

  Logger::LOG_INFO("Server shutdown");
}
