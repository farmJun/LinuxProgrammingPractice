#include <stdio.h>
#include <unistd.h>

void whatable(char *filename) {
    if (access(filename, R_OK) == -1) {
        perror("this file is not readable");

    } else {
        printf("%s readable, proceeding\n", filename);
    }

    if (access(filename, W_OK) == -1) {
        perror("this file is not writable");
    } else {
        printf("%s writeable, proceeding\n", filename);
    }

    if (access(filename, X_OK) == -1) {
        perror("this file is not executable");
    } else {
        printf("%s executable, proceeding\n", filename);
    }
}

int main() {

    char *filename = "ex3.6.txt";

    whatable(filename);

    return 0;
}
