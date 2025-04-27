//
// Created by Seongyun Jang on 3/23/25.
//

#ifndef WEBSERVER_REQUEST_H
#define WEBSERVER_REQUEST_H

#include <map>
#include <string>
#include "http_constants.h"

using namespace std;

class Request {
public:
    explicit Request(string& buffer);
    ~Request();
    http::Method method;
    string path;
    string query;
    map<string, string> headers;
    map<string, string> params;
    string body;
};


#endif //WEBSERVER_REQUEST_H
