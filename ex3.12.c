#include <stdio.h>
#include <sys/stat.h>
#include <ctype.h>
#include <stdlib.h>

int lsoct(char *permissions) {
    int octal = 0;
    for (int i = 0; i < 9; i++) {
        octal <<= 1;
        if (permissions[i] != '-') {
            octal |= 1;
        }
    }
    return octal;
}

char *octls(int octal) {
    char *permissions;
    for (int i = 8; i >= 0; i--) {
        permissions[i] = (octal & 1) ? 'x' : '-';
        octal >>= 1;
    }
    for (int i = 0; i < 9; i += 3) {
        if (permissions[i] == 'x') {
            permissions[i] = 'r';
        }
    }
    for (int i = 1; i < 9; i += 3) {
        if (permissions[i] == 'x') {
            permissions[i] = 'w';
        }
    }
    permissions[9] = '\0';

    return permissions;
}

int main(int argc, char *argv[]) {

    char *permissions = argv[argc - 1];

    for (int i = 1; i < argc - 1; ++i) {
        char *filename = argv[i];
        if (isdigit(permissions[0])) {
            chmod(filename, strtol(permissions, 0, 8));
        } else {
            chmod(filename, lsoct(permissions));
        }
    }

    return 0;
}
