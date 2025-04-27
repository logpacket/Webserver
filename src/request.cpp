//
// Created by Seongyun Jang on 3/23/25.
//

#include <sstream>
#include "request.h"

Request::Request(string& buffer) {
    // Parse request
    istringstream iss(buffer);

    // Parse first line
    string line;
    getline(iss, line);
    istringstream first_line(line);
    string method_str;

    // Parse method
    first_line >> method_str;
    this->method = http::STRING_TO_METHOD.find(method_str)->second;

    // Parse path and query
    first_line >> this->path;
    if (size_t pos = this->path.find('?'); pos != string::npos) {
        this->query = this->path.substr(pos + 1);
        this->path = this->path.substr(0, pos);
    }

    // Parse headers
    while (getline(iss, line)) {
        // Check for end of headers
        if (line == "\r") {
            break;
        }

        // Parse header
        size_t colon_pos = line.find(':');
        string key = line.substr(0, colon_pos);
        string value = line.substr(colon_pos + 2);
        this->headers[key] = value;
    }

    // Parse body
    if (this->method == http::Method::POST) {
        this->body = iss.str();
    }
}

Request::~Request() {
    // Destructor
    this->headers.clear();
}