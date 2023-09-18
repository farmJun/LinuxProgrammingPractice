#include <stdio.h>

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

char* octls(int octal) {
    char* permissions;
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

int main() {
    printf("0%o\n", lsoct("rwxr-xr-x"));

    printf("%s\n", octls(0755));

    return 0;
}
