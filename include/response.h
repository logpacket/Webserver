//
// Created by Seongyun Jang on 3/17/25.
//

#ifndef WEBSERVER_RESPONSE_H
#define WEBSERVER_RESPONSE_H

#include "http_constants.h"
#include <map>
#include <string>

using namespace std;

class Response {
public:
  Response(const string &data, http::StatusCode status_code,
           const map<string, string> &headers, bool keep_alive);
  Response(http::StatusCode status_code, const map<string, string> &headers);
  void serialize();
  string get_headers();
  string get_body();
  string get_chunk(size_t offset, size_t size) const;
  string get_metadata() const;

private:
  string data;
  string body;
  map<string, string> headers;
  string serialized_headers;
  string serialized_body;
  inline static const map<string, string> default_headers = {
      {"Server", "WebServer/1.0"},
      {"Host", "localhost"},
      {"Accept", "*/*"},
      {"Accept-Language", "en-US,en;q=0.5"}};
  http::StatusCode status_code;
  // Template for HTTP headers
  static constexpr auto const *HTTP_HEADER_TEMPLATE = "{0}: {1}\r\n";

  // Template for HTTP responses
  static constexpr auto const *HTTP_RESPONSE_TEMPLATE =
      "HTTP/1.1 {0} {1}\r\n{2}\r\n";

  // Common error page templates
  static constexpr auto const *ERROR_PAGE_TEMPLATE =
      "<!DOCTYPE html>\n"
      "<html>\n"
      "<head>\n"
      "    <title>{0} {1}</title>\n"
      "    <style>\n"
      "        body {{ font-family: Arial, sans-serif; margin: 40px; }}\n"
      "        h1 {{ color: #444; }}\n"
      "        p {{ color: #666; }}\n"
      "    </style>\n"
      "</head>\n"
      "<body>\n"
      "    <h1>{0} {1}</h1>\n"
      "    <p>{2}</p>\n"
      "</body>\n"
      "</html>";
};

#endif // WEBSERVER_RESPONSE_H
