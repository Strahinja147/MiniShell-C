#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <limits.h>
#include <stdbool.h> 
#include "backgroundProcess.h"
#include "utils.h"
#include "builtin.h"
#include "external.h"
#include "pipe.h"
#include "redirection.h"

bool isBackgroundProcess(char** args, int brojArgumenata)
{
    if (brojArgumenata > 0 && strcmp(args[brojArgumenata - 1], "&") == 0)
        return true;
    return false;
}

bool executeBackgroundProcess(char** args, int brojArgumenata)
{
    fflush(stdout);

    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork nije uspeo");
        return false;
    }

    if (pid == 0) { 
        char putDoLoga[PATH_MAX + 128];
        char trenutni[PATH_MAX];
        
        getcwd(trenutni, sizeof(trenutni));
        snprintf(putDoLoga, sizeof(putDoLoga), "%s/logs/pid_%d.log", trenutni, getpid());

        int fd = open(putDoLoga, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd < 0) {
            fprintf(stderr, "Greska: Ne mogu da napravim log fajl. Proverite da li 'logs' folder postoji.\n");
            exit(1);
        }

        dup2(fd, STDOUT_FILENO);
        dup2(fd, STDERR_FILENO);
        close(fd);

        setvbuf(stdout, NULL, _IONBF, 0);

        if (checkForPipe(args)) {
            executePipe(args);
        } else if (checkForRedirect(args)) {
            executeRedirect(args);
        } else if (isBuiltin(args[0])) {
            executeBuiltin(args, brojArgumenata);
        } else {
            executeExternal(args);
        }

        exit(0);
    } 
    else { 
        printf("[POZADINA] Proces pokrenut sa PID: %d. Log: logs/pid_%d.log\n", pid, pid);
    }

    return true;
}