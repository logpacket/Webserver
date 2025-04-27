#include <iostream>
#include "server.h"
#include "logger.h"

int main(const int argc, char *argv[]) {
    if (argc != 2) {
        Logger::LOG_ERROR("Usage: " + std::string(argv[0]) + " <port>");
        return 1;
    }
    const int port = stoi(argv[1]);
    Server server(port);
    server.init();
    server.run();

    return 0;
}
