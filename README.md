# Webserver

Webserver is a simple HTTP server written in C++.

## C++ version
C++ 23

## Build
```bash
cmake ./
make
```

## Run
```bash
./webserver <port>
```

## Features
- Simple HTTP GET request handling
- Basic error handling
- Select-based I/O multiplexing
- Thread-safe logging
- Logging to console
- Logging to file
- Configurable server port
- Graceful shutdown on SIGINT
- Basic HTML response