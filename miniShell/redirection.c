#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "redirection.h"
#include "external.h"
#include "builtin.h"

int pronadjiZnakRedirekcije(char** args) {
    for (int i = 0; args[i] != NULL; i++) {
        if (strcmp(args[i], ">") == 0 || strcmp(args[i], ">>") == 0 ||
            strcmp(args[i], "2>") == 0 || strcmp(args[i], "&>") == 0) {
            return i;
        }
    }
    return -1;
}

bool executeRedirect(char** args, int brojArgumenata) {
    int idx = pronadjiZnakRedirekcije(args);
    if (idx == -1 || args[idx + 1] == NULL) {
        fprintf(stderr, "Greska: Nepravilna upotreba redirekcije.\n");
        return false;
    }

    char* znak = args[idx];
    char* fajl = args[idx + 1];

    char* sacuvanZnak = args[idx];
    args[idx] = NULL;

    int fd;
    int flegovi = O_WRONLY | O_CREAT;
    int mod = 0644; 
    
    int originalStdout = dup(STDOUT_FILENO);
    int originalStderr = dup(STDERR_FILENO);
    bool uspeh = false;

    if (strcmp(znak, ">") == 0) {
        fd = open(fajl, flegovi | O_TRUNC, mod);
        dup2(fd, STDOUT_FILENO);
    } 
    else if (strcmp(znak, ">>") == 0) {
        fd = open(fajl, flegovi | O_APPEND, mod);
        dup2(fd, STDOUT_FILENO);
    }
    else if (strcmp(znak, "2>") == 0) {
        fd = open(fajl, flegovi | O_TRUNC, mod);
        dup2(fd, STDERR_FILENO);
    }
    else if (strcmp(znak, "&>") == 0) {
        fd = open(fajl, flegovi | O_TRUNC, mod);
        dup2(fd, STDOUT_FILENO);
        dup2(fd, STDERR_FILENO);
    }

    if (fd < 0) {
        perror("Greska pri otvaranju fajla za redirekciju");
        args[idx] = sacuvanZnak; 
        return false;
    }

    close(fd);

    if (isBuiltin(args[0])) {
        uspeh = executeBuiltin(args, idx);
    } else {
        uspeh = executeExternal(args, idx);
    }

    dup2(originalStdout, STDOUT_FILENO);
    dup2(originalStderr, STDERR_FILENO);
    close(originalStdout);
    close(originalStderr);

    args[idx] = sacuvanZnak;

    return uspeh;
}