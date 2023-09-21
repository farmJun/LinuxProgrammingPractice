#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char *argv[]) {

    if (access(argv[1], W_OK) == -1) {
        printf("you don't have write permission\n");
        printf("Do you want unlink this file?\n");
        printf("(Yes = 1, No123 = 0)\n");


        int isUnlink = 0;
        scanf("%d", &isUnlink);

        if (isUnlink == 1) {
            if (unlink(argv[1]) == -1) {
                perror("unlink failed");
                exit(1);
            }
        }
    } else {
        printf("you have write permission\n");

        if (unlink(argv[1]) == -1) {
            perror("unlink failed");
            exit(1);
        }
    }
}