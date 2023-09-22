#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#define BUFF_SIZE 1000

struct stat sb;
char buff[BUFF_SIZE];
ssize_t nread;

int fd = -1;

void slowWatch(const char *name, time_t last) {
    printf("watching...\n");

    if (stat(name, &sb) == -1 || sb.st_mtime != last) {
        printf("%s changed\n", name);

        while ((nread = read(fd, buff, BUFF_SIZE - 1)) > 0) {
            buff[nread] = '\0';
            puts(buff);
        }

        close(fd);
        exit(1);
    }
}

int main(int argc, char *argv[]) {
    time_t lastTime;

    if ((fd = open(argv[1], O_RDONLY)) < 0) {
        perror("open error");
        exit(1);
    }

    if (stat(argv[1], &sb) == -1) {
        perror("stat error");
        exit(1);
    }

    lastTime = sb.st_mtime;

    while (1) {
        slowWatch(argv[1], lastTime);
        sleep(2);
    }
}
 

