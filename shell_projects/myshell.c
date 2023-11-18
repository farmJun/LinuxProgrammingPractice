#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_CMD_ARG 10
#define BUFSIZ 256

const char *prompt = "myshell> ";
char *cmdvector[MAX_CMD_ARG];
char cmdline[BUFSIZ];

void fatal(char *str) {
    perror(str);
    exit(1);
}

int makelist(char *s, const char *delimiters, char **list, int MAX_LIST) {
    int i = 0;
    int numtokens = 0;
    char *snew = NULL;

    if ((s == NULL) || (delimiters == NULL)) return -1;

    snew = s + strspn(s, delimiters);    /* Skip delimiters */
    if ((list[numtokens] = strtok(snew, delimiters)) == NULL)
        return numtokens;

    numtokens = 1;

    while (1) {
        if ((list[numtokens] = strtok(NULL, delimiters)) == NULL)
            break;
        if (numtokens == (MAX_LIST - 1)) return -1;
        numtokens++;
    }
    return numtokens;
}

int main(int argc, char **argv) {
    int i = 0;
    pid_t pid;

    while (1) {
        fputs(prompt, stdout);   // display prompt "myshell> "
        fgets(cmdline, BUFSIZ, stdin);    // read commands from terminal
        cmdline[strlen(cmdline) - 1] = '\0'; // replace '\n' with null character '\0'


        i = makelist(cmdline, " \t", cmdvector, MAX_CMD_ARG);

        int background = 0;
        if (i > 0 && strcmp(cmdvector[i - 1], "&") == 0) {
            background = 1;
            cmdvector[i - 1] = NULL;
        }

        if (strcmp(cmdvector[0], "cd") == 0) {
            if (cmdvector[1] == NULL) {
                fprintf(stderr, "cd: argument required\n");
            } else if (chdir(cmdvector[1]) < 0) {
                perror("chdir()");
            }
            continue;
        } else if (strcmp(cmdvector[0], "exit") == 0) {
            fputs("System terminated...", stdout);
            exit(0);
          }

        switch (pid = fork()) {
            case 0:
                if (strcmp(cmdvector[0], "ll") == 0) {
                    cmdvector[0] = "ls";
                    cmdvector[1] = "-als";
                    execvp(cmdvector[0], cmdvector);
                }else{
                    execvp(cmdvector[0], cmdvector);
                    fatal("main()");
                }
            case -1:
                fatal("main()");
            default:
                if (!background) {
                    wait(NULL);
                }
        }
    }

    return 0;
}
