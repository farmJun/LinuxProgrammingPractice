#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define NAMELENGTH 41
char namebuf[NAMELENGTH];
int infile = -1;

char *getOccupier(int roomNumber) {
    off_t offset;
    ssize_t nread;

    offset = (roomNumber - 1) * NAMELENGTH;

    if (lseek(infile, offset, SEEK_SET) == -1) {
        return NULL;
    }

    if ((nread = read(infile, namebuf, NAMELENGTH)) <= 0) {
        return NULL;
    }

    namebuf[nread - 1] = '\0';
    return namebuf;
}


int findFree() {
    int roomNumber = 1;

    while (roomNumber <= 10) {
        char *occupier1 = getOccupier(roomNumber);

        if (occupier1[0] == '!') {
            return roomNumber;
        }

        roomNumber++;
    }

    return -1;
}

int main() {

    if (infile == -1 && (infile = open("residents", O_RDWR)) == -1) {
        return 0;
    }

    int freeRoom = findFree();

    if (freeRoom != -1) {
        printf("The lowest numbered free room is room %d\n", freeRoom);
    } else {
        printf("We don't have any empty rooms right now\n");
    }

    close(infile);

    return 0;
}
