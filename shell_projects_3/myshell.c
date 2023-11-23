#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <signal.h>

#define MAX_CMD_ARG 10
#define BUFSIZ 256

const char *prompt = "myshell> ";
char *cmdvector[MAX_CMD_ARG];
char cmdline[BUFSIZ];
int flag = 0;

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

//////// Helper Functions ///////
void reset_cmdvector(int tokens) {
    for (int i = 0; i < tokens; i++) {
        cmdvector[i] = NULL;
    }
}
////////////////////////////////

/////// Signal Handlers ///////
void ignore_handler(int signo) {
    printf("\tIgnore\n");
    strcpy(cmdline, "\n");
}

void bg_handler(int signo) {
    pid_t pid;
    if ((pid = waitpid(-1, NULL, WNOHANG)) > 0) {
        printf("\n[%d]\tDone\n", pid);
        strcpy(cmdline, "\n");
        flag = 1;
    }
}
//////////////////////////////

/////// Redirection "<, >" ///////
void redirection() {
    int fd = -1;
    for (int i = 0; cmdvector[i] != NULL; i++) {
        if (strcmp(cmdvector[i], ">") == 0) {
            if (cmdvector[i + 1] == NULL || cmdvector[i + 2] != NULL) {
                fatal("redirection()");
            } else {
                if ((fd = open(cmdvector[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644)) == -1) {
                    fatal("redirection()");
                }

                dup2(fd, 1);
                close(fd);

                for (int j = i; cmdvector[j] != NULL; j++) {
                    cmdvector[j] = NULL;
                }
            }
        } else if (strcmp(cmdvector[i], "<") == 0) {
            if (cmdvector[i + 1] == NULL) {
                fatal("redirection()");
            } else {
                if ((fd = open(cmdvector[i + 1], O_RDONLY | O_CREAT, 0644)) == -1) {
                    fatal("redirection()");
                }

                dup2(fd, 0);
                close(fd);

                for (int j = i; cmdvector[j] != NULL; j++) {
                    cmdvector[j] = cmdvector[j + 2];
                }
                i--;
            }
        }
    }
}
/////////////////////////////////////

int main(int argc, char **argv) {
    pid_t pid;


    static struct sigaction ignore_act, bg_act;
    ignore_act.sa_handler = ignore_handler;
    bg_act.sa_handler = bg_handler;

    sigaction(SIGINT, &ignore_act, NULL);
    sigaction(SIGQUIT, &ignore_act, NULL);
    sigaction(SIGTSTP, &ignore_act, NULL);
    sigaction(SIGCHLD, &bg_act, NULL);

    while (1) {
        if (flag == 0)
            fputs(prompt, stdout);   // display prompt "myshell> "
        fgets(cmdline, BUFSIZ, stdin);    // read commands from terminal

        if (!strcmp(cmdline, "\n")) continue;

        cmdline[strlen(cmdline) - 1] = '\0'; // replace '\n' with null character '\0'

        int tokens = makelist(cmdline, " \t", cmdvector, MAX_CMD_ARG);

        if (!strcmp(cmdvector[0], "exit")) {
            printf("Shell Terminated...\n");
            exit(0);
        } else if (!strcmp(cmdvector[0], "cd")) {
            if (chdir(cmdvector[1]) == -1)    // error on chdir()
                fatal("main() - chdir()");
        } else {
            if (!strcmp(cmdvector[0], "ll")) {
                cmdvector[0] = "ls";
                cmdvector[tokens] = "-als";
            }

            int background = 0;
            if (!strcmp(cmdvector[tokens - 1], "&")) {
                cmdvector[tokens - 1] = NULL;
                background = 1;
            }
            switch (pid = fork()) {
                case 0:
                    redirection();
                    execvp(cmdvector[0], cmdvector);
                    fatal("main()");
                    break;
                case -1:
                    fatal("main()");
                    break;
                default:
                    if (background == 0) {
                        waitpid(pid, NULL, 0);
                    }

                    break;
            }
        }
        flag = 0;
        reset_cmdvector(tokens);  // in case of bad commands, reset cmdvector[]
    }
    return 0;
}
