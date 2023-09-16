#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int fd;

int fileopen(const char *filename, const char *mode) {

    int flags;

    if (strcmp(mode, "r") == 0) {
        flags = O_RDONLY;
    } else if (strcmp(mode, "w") == 0) {
        flags = O_WRONLY | O_CREAT | O_TRUNC;
    } else if (strcmp(mode, "rw") == 0) {
        flags = O_RDWR | O_CREAT;
    } else if (strcmp(mode, "a") == 0) {
        flags = O_WRONLY | O_CREAT | O_APPEND;
    } else {
        printf("Invalid mode\n");
        return -1;
    }

    if ((fd = open(filename, flags)) < 0) {
        perror("Error opening file");
        return -1;
    }

    return fd;
}

int main() {
    const char *filename = "ex2.14.txt";
    int fd;

    fd = fileopen(filename, "r");
    close(fd);

    fd = fileopen(filename, "w");
    close(fd);

    fd = fileopen(filename, "rw");
    close(fd);

    fd = fileopen(filename, "a");
    close(fd);

    return 0;
}




