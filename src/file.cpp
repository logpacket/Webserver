//
// Created by Seongyun Jang on 3/31/25.
//

#include "file.h"

#include <fcntl.h>
#include <stdexcept>
#include <unistd.h>
#include <cerrno>


File::File(const char *path) {
    this->path = path;
    this->fd = open(path, O_RDONLY);
    if (this->fd == -1) {
        throw std::runtime_error("File not found");
    }
    if (fstat(this->fd, &this->file_stat) == -1) {
        close(this->fd);
        throw std::runtime_error("Failed to get file status");
    }
    if (this->file_stat.st_mode & S_IFDIR) {
        close(this->fd);
        throw std::runtime_error("Path is a directory");
    }
    if (this->file_stat.st_size == 0) {
        close(this->fd);
        throw std::runtime_error("File is empty");
    }
}

void File::read(char * buffer) const {
    // Reset file offset to the beginning
    lseek(fd, 0, SEEK_SET);

    // Read file into buffer
    const size_t size = this->file_stat.st_size;
    ssize_t bytes_read = 0;
    size_t bytes_left = size;
    char *buffer_pos = buffer;
    while (bytes_left > 0) {
        bytes_read = ::read(this->fd, buffer_pos, bytes_left);
        if (bytes_read < 0) {
            if (errno == EINTR) {
                continue; // Retry on interrupt
            }
            close(this->fd);
            throw std::runtime_error("Error reading file");
        }
        if (bytes_read == 0) {
            break; // EOF
        }
        buffer_pos += bytes_read;
        bytes_left -= bytes_read;
    }
}

size_t File::size() const {
    return this->file_stat.st_size;
}

File::~File() {
    close(this->fd);
};

