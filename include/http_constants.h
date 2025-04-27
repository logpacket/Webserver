//
// Created by Seongyun Jang on 3/23/25.
//

#ifndef WEBSERVER_HTTP_CONSTANTS_H
#define WEBSERVER_HTTP_CONSTANTS_H

#include <string>
#include <unordered_map>

namespace http {

// Direct mapping from file extension to MIME type string
inline std::unordered_map<std::string, std::string> EXTENSION_TO_MIME = {
    {".txt", "text/plain"},
    {".html", "text/html"},
    {".htm", "text/html"},
    {".css", "text/css"},
    {".js", "text/javascript"},
    {".xml", "text/xml"},
    {".csv", "text/csv"},
    {".md", "text/markdown"},

    {".json", "application/json"},
    {".pdf", "application/pdf"},
    {".zip", "application/zip"},
    {".doc", "application/msword"},
    {".xls", "application/vnd.ms-excel"},
    {".ppt", "application/vnd.ms-powerpoint"},
    {".docx",
     "application/vnd.openxmlformats-officedocument.wordprocessingml.document"},
    {".xlsx",
     "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet"},
    {".pptx", "application/"
              "vnd.openxmlformats-officedocument.presentationml.presentation"},

    {".jpg", "image/jpeg"},
    {".jpeg", "image/jpeg"},
    {".png", "image/png"},
    {".gif", "image/gif"},
    {".svg", "image/svg+xml"},
    {".webp", "image/webp"},
    {".bmp", "image/bmp"},
    {".tiff", "image/tiff"},
    {".tif", "image/tiff"},
    {".ico", "image/x-icon"},

    {".mp3", "audio/mpeg"},
    {".ogg", "audio/ogg"},
    {".wav", "audio/wav"},
    {".webm", "audio/webm"},

    {".mp4", "video/mp4"},
    {".ogv", "video/ogg"},
    {".webm", "video/webm"},
    {".mpeg", "video/mpeg"},
    {".mpg", "video/mpeg"},

    {".woff", "font/woff"},
    {".woff2", "font/woff2"},
    {".ttf", "font/ttf"},
    {".otf", "font/otf"}};

// Default MIME type for unknown extensions
const std::string DEFAULT_MIME_TYPE = "application/octet-stream";

// HTTP Status Codes enum
enum class StatusCode {
  // 1xx Informational
  CONTINUE = 100,
  SWITCHING_PROTOCOLS = 101,
  PROCESSING = 102,
  EARLY_HINTS = 103,

  // 2xx Success
  OK = 200,
  CREATED = 201,
  ACCEPTED = 202,
  NON_AUTHORITATIVE_INFORMATION = 203,
  NO_CONTENT = 204,
  RESET_CONTENT = 205,
  PARTIAL_CONTENT = 206,
  MULTI_STATUS = 207,
  ALREADY_REPORTED = 208,
  IM_USED = 226,

  // 3xx Redirection
  MULTIPLE_CHOICES = 300,
  MOVED_PERMANENTLY = 301,
  FOUND = 302,
  SEE_OTHER = 303,
  NOT_MODIFIED = 304,
  USE_PROXY = 305,
  TEMPORARY_REDIRECT = 307,
  PERMANENT_REDIRECT = 308,

  // 4xx Client Error
  BAD_REQUEST = 400,
  UNAUTHORIZED = 401,
  PAYMENT_REQUIRED = 402,
  FORBIDDEN = 403,
  NOT_FOUND = 404,
  METHOD_NOT_ALLOWED = 405,
  NOT_ACCEPTABLE = 406,
  PROXY_AUTHENTICATION_REQUIRED = 407,
  REQUEST_TIMEOUT = 408,
  CONFLICT = 409,
  GONE = 410,
  LENGTH_REQUIRED = 411,
  PRECONDITION_FAILED = 412,
  PAYLOAD_TOO_LARGE = 413,
  URI_TOO_LONG = 414,
  UNSUPPORTED_MEDIA_TYPE = 415,
  RANGE_NOT_SATISFIABLE = 416,
  EXPECTATION_FAILED = 417,
  MISDIRECTED_REQUEST = 421,
  UNPROCESSABLE_ENTITY = 422,
  LOCKED = 423,
  FAILED_DEPENDENCY = 424,
  TOO_EARLY = 425,
  UPGRADE_REQUIRED = 426,
  PRECONDITION_REQUIRED = 428,
  TOO_MANY_REQUESTS = 429,
  REQUEST_HEADER_FIELDS_TOO_LARGE = 431,
  UNAVAILABLE_FOR_LEGAL_REASONS = 451,

  // 5xx Server Error
  INTERNAL_SERVER_ERROR = 500,
  NOT_IMPLEMENTED = 501,
  BAD_GATEWAY = 502,
  SERVICE_UNAVAILABLE = 503,
  GATEWAY_TIMEOUT = 504,
  HTTP_VERSION_NOT_SUPPORTED = 505,
  VARIANT_ALSO_NEGOTIATES = 506,
  INSUFFICIENT_STORAGE = 507,
  LOOP_DETECTED = 508,
  NOT_EXTENDED = 510,
  NETWORK_AUTHENTICATION_REQUIRED = 511
};

// Global map for HTTP status code and reason phrases
inline std::unordered_map<StatusCode, std::string> STATUS_CODE_MAP = {
    // 1xx Informational
    {StatusCode::CONTINUE, "Continue"},
    {StatusCode::SWITCHING_PROTOCOLS, "Switching Protocols"},
    {StatusCode::PROCESSING, "Processing"},
    {StatusCode::EARLY_HINTS, "Early Hints"},

    // 2xx Success
    {StatusCode::OK, "OK"},
    {StatusCode::CREATED, "Created"},
    {StatusCode::ACCEPTED, "Accepted"},
    {StatusCode::NON_AUTHORITATIVE_INFORMATION,
     "Non-Authoritative Information"},
    {StatusCode::NO_CONTENT, "No Content"},
    {StatusCode::RESET_CONTENT, "Reset Content"},
    {StatusCode::PARTIAL_CONTENT, "Partial Content"},
    {StatusCode::MULTI_STATUS, "Multi-Status"},
    {StatusCode::ALREADY_REPORTED, "Already Reported"},
    {StatusCode::IM_USED, "IM Used"},

    // 3xx Redirection
    {StatusCode::MULTIPLE_CHOICES, "Multiple Choices"},
    {StatusCode::MOVED_PERMANENTLY, "Moved Permanently"},
    {StatusCode::FOUND, "Found"},
    {StatusCode::SEE_OTHER, "See Other"},
    {StatusCode::NOT_MODIFIED, "Not Modified"},
    {StatusCode::USE_PROXY, "Use Proxy"},
    {StatusCode::TEMPORARY_REDIRECT, "Temporary Redirect"},
    {StatusCode::PERMANENT_REDIRECT, "Permanent Redirect"},

    // 4xx Client Error
    {StatusCode::BAD_REQUEST, "Bad request"},
    {StatusCode::UNAUTHORIZED, "Unauthorized"},
    {StatusCode::PAYMENT_REQUIRED, "Payment Required"},
    {StatusCode::FORBIDDEN, "Forbidden"},
    {StatusCode::NOT_FOUND, "Not Found"},
    {StatusCode::METHOD_NOT_ALLOWED, "Method Not Allowed"},
    {StatusCode::NOT_ACCEPTABLE, "Not Acceptable"},
    {StatusCode::PROXY_AUTHENTICATION_REQUIRED,
     "Proxy Authentication Required"},
    {StatusCode::REQUEST_TIMEOUT, "request Timeout"},
    {StatusCode::CONFLICT, "Conflict"},
    {StatusCode::GONE, "Gone"},
    {StatusCode::LENGTH_REQUIRED, "Length Required"},
    {StatusCode::PRECONDITION_FAILED, "Precondition Failed"},
    {StatusCode::PAYLOAD_TOO_LARGE, "Payload Too Large"},
    {StatusCode::URI_TOO_LONG, "URI Too Long"},
    {StatusCode::UNSUPPORTED_MEDIA_TYPE, "Unsupported Media Type"},
    {StatusCode::RANGE_NOT_SATISFIABLE, "Range Not Satisfiable"},
    {StatusCode::EXPECTATION_FAILED, "Expectation Failed"},
    {StatusCode::MISDIRECTED_REQUEST, "Misdirected request"},
    {StatusCode::UNPROCESSABLE_ENTITY, "Unprocessable Entity"},
    {StatusCode::LOCKED, "Locked"},
    {StatusCode::FAILED_DEPENDENCY, "Failed Dependency"},
    {StatusCode::TOO_EARLY, "Too Early"},
    {StatusCode::UPGRADE_REQUIRED, "Upgrade Required"},
    {StatusCode::PRECONDITION_REQUIRED, "Precondition Required"},
    {StatusCode::TOO_MANY_REQUESTS, "Too Many Requests"},
    {StatusCode::REQUEST_HEADER_FIELDS_TOO_LARGE,
     "request Header Fields Too Large"},
    {StatusCode::UNAVAILABLE_FOR_LEGAL_REASONS,
     "Unavailable For Legal Reasons"},

    // 5xx Server Error
    {StatusCode::INTERNAL_SERVER_ERROR, "Internal Server Error"},
    {StatusCode::NOT_IMPLEMENTED, "Not Implemented"},
    {StatusCode::BAD_GATEWAY, "Bad Gateway"},
    {StatusCode::SERVICE_UNAVAILABLE, "Service Unavailable"},
    {StatusCode::GATEWAY_TIMEOUT, "Gateway Timeout"},
    {StatusCode::HTTP_VERSION_NOT_SUPPORTED, "HTTP Version Not Supported"},
    {StatusCode::VARIANT_ALSO_NEGOTIATES, "Variant Also Negotiates"},
    {StatusCode::INSUFFICIENT_STORAGE, "Insufficient Storage"},
    {StatusCode::LOOP_DETECTED, "Loop Detected"},
    {StatusCode::NOT_EXTENDED, "Not Extended"},
    {StatusCode::NETWORK_AUTHENTICATION_REQUIRED,
     "Network Authentication Required"}};

// Common HTTP headers
struct HTTPHeaders {
  inline static const std::string CONTENT_TYPE = "Content-Type";
  inline static const std::string CONTENT_LENGTH = "Content-Length";
  inline static const std::string CONTENT_ENCODING = "Content-Encoding";
  inline static const std::string CONTENT_LANGUAGE = "Content-Language";
  inline static const std::string CONTENT_LOCATION = "Content-Location";
  inline static const std::string CONTENT_DISPOSITION = "Content-Disposition";
  inline static const std::string CONTENT_RANGE = "Content-Range";

  inline static const std::string DATE = "Date";
  inline static const std::string SERVER = "Server";
  inline static const std::string CONNECTION = "Connection";
  inline static const std::string KEEP_ALIVE = "Keep-Alive";

  inline static const std::string CACHE_CONTROL = "Cache-Control";
  inline static const std::string ETAG = "ETag";
  inline static const std::string LAST_MODIFIED = "Last-Modified";
  inline static const std::string EXPIRES = "Expires";

  inline static const std::string ACCEPT = "Accept";
  inline static const std::string ACCEPT_CHARSET = "Accept-Charset";
  inline static const std::string ACCEPT_ENCODING = "Accept-Encoding";
  inline static const std::string ACCEPT_LANGUAGE = "Accept-Language";
  inline static const std::string ACCEPT_RANGES = "Accept-Ranges";

  inline static const std::string HOST = "Host";
  inline static const std::string USER_AGENT = "User-Agent";
  inline static const std::string REFERER = "Referer";
  inline static const std::string COOKIE = "Cookie";
  inline static const std::string SET_COOKIE = "Set-Cookie";

  inline static const std::string AUTHORIZATION = "Authorization";
  inline static const std::string WWW_AUTHENTICATE = "WWW-Authenticate";

  inline static const std::string LOCATION = "Location";
  inline static const std::string ALLOW = "Allow";

  inline static const std::string ACCESS_CONTROL_ALLOW_ORIGIN =
      "Access-Control-Allow-Origin";
  inline static const std::string ACCESS_CONTROL_ALLOW_METHODS =
      "Access-Control-Allow-Methods";
  inline static const std::string ACCESS_CONTROL_ALLOW_HEADERS =
      "Access-Control-Allow-Headers";
  inline static const std::string ACCESS_CONTROL_ALLOW_CREDENTIALS =
      "Access-Control-Allow-Credentials";
};

// HTTP Methods enum
enum class Method {
  GET,
  POST,
  PUT,
  DELETE,
  HEAD,
  OPTIONS,
  PATCH,
  TRACE,
  CONNECT
};

// Global map for HTTP methods
inline std::unordered_map<Method, std::string> METHOD_MAP = {
    {Method::GET, "GET"},        {Method::POST, "POST"},
    {Method::PUT, "PUT"},        {Method::DELETE, "DELETE"},
    {Method::HEAD, "HEAD"},      {Method::OPTIONS, "OPTIONS"},
    {Method::PATCH, "PATCH"},    {Method::TRACE, "TRACE"},
    {Method::CONNECT, "CONNECT"}};

// Reverse mapping from string to Method
inline std::unordered_map<std::string, Method> STRING_TO_METHOD = {
    {"GET", Method::GET},        {"POST", Method::POST},
    {"PUT", Method::PUT},        {"DELETE", Method::DELETE},
    {"HEAD", Method::HEAD},      {"OPTIONS", Method::OPTIONS},
    {"PATCH", Method::PATCH},    {"TRACE", Method::TRACE},
    {"CONNECT", Method::CONNECT}};


} // namespace http

#endif // WEBSERVER_HTTP_CONSTANTS_H