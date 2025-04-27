//
// Created by Seongyun Jang on 3/17/25.
//

#include "response.h"
#include "http_constants.h"
#include <format>
#include <server.h>
#include <sstream>

using namespace std;

Response::Response(const string& data, const http::StatusCode status_code, const map<string, string>& headers={}, const bool keep_alive = true) {
    this->data = data;
    this->status_code = status_code;
    this->headers = headers;
    if (keep_alive) {
        this->headers[http::HTTPHeaders::CONNECTION] = "keep-alive";
        this->headers[http::HTTPHeaders::KEEP_ALIVE] = "timeout=5, max=200";
    }
}

Response::Response(const http::StatusCode status_code, const map<string, string>& headers={}) {
    this->status_code = status_code;
    this->headers = headers;
}

void Response::serialize() {
    // If the status code is not OK, render the error page
    if (status_code >= http::StatusCode::BAD_REQUEST)
        // render error page
            this->body = format(
                ERROR_PAGE_TEMPLATE,
                static_cast<int>(status_code),
                http::STATUS_CODE_MAP[this->status_code],
                this->data
            );
    else this->body = this->data;

    // 기본 헤더 설정
    for (const auto& [key, value] : default_headers)
        this->headers[key] = value;

    // Add Content-Length header
    this->headers["Content-Length"] = to_string(this->data.size());
    string headers_string;
    for (const auto& [header, value] : this->headers)
        headers_string.append(format(HTTP_HEADER_TEMPLATE, header, value));

    this-> serialized_headers = format(
        HTTP_RESPONSE_TEMPLATE,
        static_cast<int>(status_code),
        http::STATUS_CODE_MAP[this->status_code],
        headers_string
    );
}

string Response::get_body() {
    return this->body;
}

string Response::get_chunk(const size_t offset, const size_t size) const{
    // Check if offset is within bounds
    return {data.c_str() + offset, size};
}

string Response::get_headers() {
    return this->serialized_headers;
}

string Response::get_metadata() const {
    return http::STATUS_CODE_MAP[this->status_code];
}
