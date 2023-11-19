#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>

#define MAX_CMD_ARG 10
#define BUFSIZ 256

const char *prompt = "myshell> ";
char *cmdvector[MAX_CMD_ARG];
char cmdline[BUFSIZ];

pid_t background_processes[MAX_CMD_ARG];
int num_background_processes = 0;

pid_t foreground_process = 0;

void sigint_ignore(int signal_number) {
    if (foreground_process != 0) {
        printf("\n");
        kill(foreground_process, SIGINT);
        foreground_process = 0;
    } else {
        printf("\t Ignored\n");
        fputs(prompt, stdout);
        fflush(stdout);
    }
}

void sigquit_ignore(int signal_number) {
    if (foreground_process != 0) {
        printf("\n");
        kill(foreground_process, SIGQUIT);
        foreground_process = 0;
    } else {
        printf("\t Ignored\n");
        fputs(prompt, stdout);
        fflush(stdout);
    }
}

void sigtstp_ignore(int signal_number) {
    if (foreground_process != 0) {
        printf("\n");
        kill(foreground_process, SIGTSTP);
        foreground_process = 0;
    } else {
        printf("\t Ignored\n");
        fputs(prompt, stdout);
        fflush(stdout);
    }
}

void child_handler(int signal_number) {
    int status;
    pid_t pid;

    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        for (int i = 0; i < num_background_processes; i++) {
            if (background_processes[i] == pid) {
                printf("[%d] Done\n", pid);
                for (int j = i; j < num_background_processes - 1; j++) {
                    background_processes[j] = background_processes[j + 1];
                }
                num_background_processes--;
                break;
            }
        }
    }
}

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

    signal(SIGCHLD, child_handler);
    signal(SIGINT, sigint_ignore);
    signal(SIGQUIT, sigquit_ignore);
    signal(SIGTSTP, sigtstp_ignore);

    while (1) {
        fputs(prompt, stdout);   // display prompt "myshell> "
        fgets(cmdline, BUFSIZ, stdin);    // read commands from terminal

        if (!strcmp(cmdline, "\n")) continue;

        cmdline[strlen(cmdline) - 1] = '\0'; // replace '\n' with null character '\0'

        int tokens = makelist(cmdline, " \t", cmdvector, MAX_CMD_ARG);

        //// Feature #2 : exit ////
        if (!strcmp(cmdvector[0], "exit")) {
            printf("Shell Terminated...\n");
            exit(0);
        }
            //// End of Feature #2 ////

            //// Feature #1 : cd ////
        else if (!strcmp(cmdvector[0], "cd")) {
            if (chdir(cmdvector[1]) == -1) {    // error on chdir()
                fatal("main() - chdir()");
            }
        }
            //// End of Feature #1 ////

        else {
            //// Bonus : ll ////
            if (!strcmp(cmdvector[0], "ll")) {
                cmdvector[0] = "ls";
                cmdvector[tokens] = "-als";
            }
            ////////////////////

            int background = 0;
            if (!strcmp(cmdvector[tokens - 1], "&")) {
                cmdvector[tokens - 1] = NULL;
                background = 1;
            }

            switch (pid = fork()) {
                case 0:
                    execvp(cmdvector[0], cmdvector);
                    fatal("main()");
                case -1:
                    fatal("main()");
                default:
                    if (background == 0) {
                        foreground_process = pid;
                        waitpid(pid, NULL, WUNTRACED);
                        foreground_process = 0;
                    } else {
                        background_processes[num_background_processes++] = pid;
                    }
            }
        }
    }
    return 0;
}
