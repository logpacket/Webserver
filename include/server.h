//
// Created by Seongyun Jang on 3/17/25.
//

#ifndef WEBSERVER_SERVER_H
#define WEBSERVER_SERVER_H

#include <netinet/in.h>
#include <sys/select.h>

#include "cache.h"
#include "request.h"
#include "response.h"

using namespace std;

class Server {
public:
    explicit Server(int port, size_t max_cache_size = 1024 * 1024 * 3);
    ~Server();
    bool init();
    void run();
    static constexpr size_t DEFAULT_BUFFER_SIZE = 8096;
private:
    int port;
    fd_set master{};
    int max_fd{};
    int server_socket{};
    sockaddr_in server_address{};
    Cache* cache;
    static constexpr string DEFAULT_ROOT = "resources";
    static constexpr string DEFAULT_INDEX = "index.html";
    static constexpr short max_retries = 5;
    static constexpr string end_of_chunk = "0\r\n\r\n";
    #ifdef __APPLE__
        static constexpr int send_option = 0;
    #else
        static constexpr int send_option = MSG_NOSIGNAL;
    #endif
    timeval timeout{1, 0}; // 1s timeout

    void handle_new_connection();
    void handle_client(int client_socket);
    void handle_request(const Request& request, int client_socket);
    void handle_get_request(const Request& request, int client_socket, bool keep_alive);
    void send_response(Response &response, int client_socket, bool keep_alive);
    void close_session(int fd);
    void wait_for_socket_ready(int client_socket);
    ssize_t send_with_retry(int client_socket, const string& data);
};


#endif //WEBSERVER_SERVER_H
