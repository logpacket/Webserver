//
// Created by Seongyun Jang on 3/31/25.
//

#ifndef FILE_H
#define FILE_H
#include <sys/stat.h>
#include <unistd.h>

class File {
public:
    explicit File(const char* path);
    ~File();
    void read(char * buffer) const;
    size_t size() const;
private:
    const char* path;
    int fd;
    struct stat file_stat{};
};


#endif //FILE_H
