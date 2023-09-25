#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>

int my_double_ls(const char *name) {
    struct dirent *d;
    DIR *dp;
    struct stat buf;
    char path[1024];

    if ((dp = opendir(name)) == NULL)
        return (-1);

    while ((d = readdir(dp)) != NULL) {
        if (d->d_ino != 0) {
            snprintf(path, sizeof(path), "%s/%s", name, d->d_name);
            if (stat(path, &buf) == 0) {
                if (S_ISDIR(buf.st_mode))
                    printf("%s* ", d->d_name);
                else
                    printf("%s  ", d->d_name);
            }
        }
    }
    printf("\n");
    rewinddir(dp);

    while ((d = readdir(dp)) != NULL) {
        if (d->d_ino != 0) {
            snprintf(path, sizeof(path), "%s/%s", name, d->d_name);
            if (stat(path, &buf) == 0) {
                if (S_ISDIR(buf.st_mode))
                    printf("%s* ", d->d_name);
                else
                    printf("%s  ", d->d_name);
            }
        }
    }

    closedir(dp);
    return (0);
}

int main(int argc, char *argv[]) {
    if (argc == 1) {
        my_double_ls(".");
    } else {
        my_double_ls(argv[1]);
    }

    return 0;
}

