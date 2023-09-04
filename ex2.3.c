#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    int fd1, fd2;
    char *file = "test";

    if ((fd1 = creat(file, S_IRUSR | S_IWUSR)) == -1) {
        printf("Failed to create file %s\n", file);
        exit(1);
    } else {
        printf("File created successfully %s\n", file);
    }

    if ((fd2 = open(file, O_RDWR)) == -1) {
        printf("Failed to open file %s\n", file);
        exit(1);
    } else {
        printf("File opened successfully %sIn", file);
        exit(0);
    }
}
