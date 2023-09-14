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


int freeroom(int roomno) {

    off_t offset = (roomno - 1) * NAMELENGTH;
    printf("offset = %lld ", offset);

    if (lseek(infile, offset, SEEK_SET) == -1) {
        return -1;
    }

    char *empty = "!EMPTY!\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\n";

    if (write(infile, empty, strlen(empty)) < 0) {
        printf("Failed to free the room.\n");
        return -1;
    }

    return 0;
}


int addGuest(int roomNumber, char *guestName) {

    off_t offset = (roomNumber - 1) * NAMELENGTH;

    char *occupier1 = getOccupier(roomNumber);

    if (occupier1[0] != '!') {
        printf("Room %d is not empty\n", roomNumber);
        return roomNumber;
    }

    if (lseek(infile, offset, SEEK_SET) == -1) {
        return -1;
    }

    for (int i = 1; i < NAMELENGTH - strlen(guestName); i++) {
        guestName += '\0';
    }

    guestName += '\n';

    if (write(infile, guestName, strlen(guestName)) < 0) {
        printf("Failed to free the room.\n");
        return -1;
    }

    return 0;
}


int main() {

    if (infile == -1 && (infile = open("residents", O_RDWR)) == -1) {
        return 0;
    }


    int roomToFree = 4;
    if (freeroom(roomToFree) == 0) {
        printf("Room %d freed successfully\n", roomToFree);
    } else {
        printf("Failed to free room %d\n", roomToFree);
    }


    close(infile);

    return 0;
}
