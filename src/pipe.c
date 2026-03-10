#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "pipe.h"
#include "external.h"
#include "builtin.h"

int nadjiIndeksPajpa(char** args) {
    for (int i = 0; args[i] != NULL; i++) {
        if (strcmp(args[i], "|") == 0) return i;
    }
    return -1;
}

bool executePipe(char** args) {
    int idx = nadjiIndeksPajpa(args);
    if (idx == -1 || args[idx + 1] == NULL) {
        fprintf(stderr, "Greska: Nepravilna upotreba pajpa.\n");
        return false;
    }

    char** args1 = args;
    char** args2 = &args[idx + 1];

    char* sacuvanZnak = args[idx];
    args[idx] = NULL;

    int pd[2];
    if (pipe(pd) == -1) {
        perror("Pipe nije uspeo");
        args[idx] = sacuvanZnak;
        return false;
    }

    pid_t pid1 = fork();
    if (pid1 == 0) {
        dup2(pd[1], STDOUT_FILENO); 
        close(pd[0]); 
        close(pd[1]); 

        if (isBuiltin(args1[0])) executeBuiltin(args1, idx);
        else executeExternal(args1);
        exit(1); 
    }

    pid_t pid2 = fork();
    if (pid2 == 0) {
        dup2(pd[0], STDIN_FILENO); 
        close(pd[1]); 
        close(pd[0]); 

        int brojArg2 = 0;
        while(args2[brojArg2] != NULL) brojArg2++;

        if (isBuiltin(args2[0])) executeBuiltin(args2, brojArg2);
        else executeExternal(args2);
        exit(1);
    }

    close(pd[0]);
    close(pd[1]);

    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);

    args[idx] = sacuvanZnak;
    return true;
}