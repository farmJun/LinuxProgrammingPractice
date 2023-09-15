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


int freeRoom(int roomno) {

    off_t offset = (roomno - 1) * NAMELENGTH;

    if (lseek(infile, offset, SEEK_SET) == -1) {
        return -1;
    }

    char *empty = "!EMPTY!\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\n";

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
        return -1;
    }

    if (lseek(infile, offset, SEEK_SET) == -1) {
        return -1;
    }

    if (write(infile, guestName, strlen(guestName)) < 0) {
        return -1;
    }

    for (int j = 0; j < NAMELENGTH - strlen(guestName); ++j) {
        if (write(infile, "\0", 1) < 0) {
            return -1;
        }
    }

    if (write(infile, "\n", 1) < 0) {
        return -1;
    }

    return 0;
}



int main() {

    if (infile == -1 && (infile = open("residents", O_RDWR)) == -1) {
        return 0;
    }

    while (1) {
        printf("Front Desk Utility Menu:\n");
        printf("1. Find a free room\n");
        printf("2. Check-in a guest\n");
        printf("3. Check-out a guest\n");
        printf("4. Get occupier of a room\n");
        printf("5. Quit\n");
        printf("Enter your choice: ");

        int choice;
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a valid choice.\n");
            while (getchar() != '\n');
            continue;
        }

        switch (choice) {
            case 1: {
                int roomNumber = findFree();
                if (roomNumber == -1) {
                    printf("No free rooms available.\n");
                } else {
                    printf("Room %d is available.\n", roomNumber);
                }
                break;
            }
            case 2: {
                int roomNumber;
                char guestName[NAMELENGTH];
                printf("Enter room number: ");
                if (scanf("%d", &roomNumber) != 1 || roomNumber < 1 || roomNumber > 10) {
                    printf("Invalid room number. Please enter a number between 1 and 10.\n");
                    while (getchar() != '\n');
                    continue;
                }
                while (getchar() != '\n');
                printf("Enter guest name: ");
                if (fgets(guestName, sizeof(guestName), stdin) == NULL) {
                    printf("Error reading guest name.\n");
                    continue;
                }
                guestName[strlen(guestName) - 1] = '\0';
                if (addGuest(roomNumber, guestName) == 0) {
                    printf("Guest %s checked into room %d.\n", guestName, roomNumber);
                } else {
                    printf("Failed to check-in guest.\n");
                }
                break;
            }
            case 3: {
                int roomNumber;
                printf("Enter room number to check-out: ");
                if (scanf("%d", &roomNumber) != 1 || roomNumber < 1 || roomNumber > 10) {
                    printf("Invalid room number. Please enter a number between 1 and 10.\n");
                    while (getchar() != '\n');
                    continue;
                }
                if (freeRoom(roomNumber) == 0) {
                    printf("Guest checked out from room %d.\n", roomNumber);
                } else {
                    printf("Failed to check-out guest.\n");
                }
                break;
            }
            case 4: {
                int roomNumber;
                printf("Enter room number: ");
                if (scanf("%d", &roomNumber) != 1 || roomNumber < 1 || roomNumber > 10) {
                    printf("Invalid room number. Please enter a number between 1 and 10.\n");
                    while (getchar() != '\n');
                    continue;
                }
                char *occupier = getOccupier(roomNumber);
                if (occupier != NULL) {
                    printf("Occupier of room %d: %s\n", roomNumber, occupier);
                } else {
                    printf("Room %d is empty or not found.\n", roomNumber);
                }
                break;
            }
            case 5:
                close(infile);
                return 0;
        }

        printf("==============================\n");
    }

    return 0;
}
