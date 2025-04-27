cmake_minimum_required(VERSION 3.27)
project(webserver)

set(CMAKE_CXX_STANDARD 23)

set(SOURCES
    src/main.cpp
    src/response.cpp
    src/server.cpp
    src/request.cpp
    src/cache.cpp
    src/file.cpp
    src/logger.cpp
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -Wall -Wextra")

include_directories(include)

add_executable(webserver ${SOURCES})

target_include_directories(webserver PRIVATE include)
