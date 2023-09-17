#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define SIZE 512

int main(int argc, char *argv[]) {
    ssize_t nread;
    char buf[SIZE];

    if (argc > 1) {

        for (int i = 1; i < argc; i++) {
            int file = open(argv[i], O_RDONLY);

            if (file == -1) {
                perror("fail to open file");
                continue;
            }

            while ((nread = read(file, buf, SIZE)) > 0) {
                write(1, buf, nread);
            }

            close(file);
        }
    } else {
        while ((nread = read(0, buf, SIZE)) > 0) {
            write(1, buf, nread);ㅛ
        }
    }

    exit(0);
}

