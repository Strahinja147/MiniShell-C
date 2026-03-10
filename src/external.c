#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <limits.h>
#include <stdbool.h>
#include "external.h"
#include "utils.h"

typedef struct {
    const char* komanda;
    const char* binarniFajl;
} MapaKomandi;

static MapaKomandi eksterneKomande[] = {
    {"ls", "mojLs"}, {"cat", "mojCat"}, {"mkdir", "mojMkdir"},
    {"rmdir", "mojRmdir"}, {"echo", "mojEcho"}, {"touch", "mojTouch"},
    {"cp", "mojCp"}, {"rm", "mojRm"}, {"head", "mojHead"},
    {"tail", "mojTail"}, {"date", "mojDate"}, {"pwd", "mojPwd"},
    {"whoami", "mojWhoami"}, {"ps", "mojPs"}
};

static char apsolutnaPutanjaDoKomandi[PATH_MAX];


void inicijalizujPutanje(void) {
    char trenutni[PATH_MAX];
    if (getcwd(trenutni, sizeof(trenutni)) != NULL) {
        char pomocna[PATH_MAX + 128];
        // snprintf(pomocna, sizeof(pomocna), "%s/mojeKomande", trenutni);
        snprintf(pomocna, sizeof(pomocna), "%s/bin", trenutni);
        
        if (realpath(pomocna, apsolutnaPutanjaDoKomandi) == NULL) {
            strncpy(apsolutnaPutanjaDoKomandi, pomocna, PATH_MAX);
        }
    }
}

bool isExternal(char* arg) {
    if (arg == NULL) return false;
    int brojKomandi = sizeof(eksterneKomande) / sizeof(MapaKomandi);
    for(int i = 0; i < brojKomandi; i++) {
        if(strcmp(arg, eksterneKomande[i].komanda) == 0)
            return true;
    }
    return false;
}

bool executeExternal(char** args) {
    char* binarnoIme = NULL;
    int brojKomandi = sizeof(eksterneKomande) / sizeof(MapaKomandi);

    for(int i = 0; i < brojKomandi; i++) {
        if(strcmp(args[0], eksterneKomande[i].komanda) == 0) {
            binarnoIme = (char*)eksterneKomande[i].binarniFajl;
            break;
        }
    }

    if (binarnoIme == NULL) return false;

    char punaPutanja[PATH_MAX + 128];
    snprintf(punaPutanja, sizeof(punaPutanja), "%s/%s", apsolutnaPutanjaDoKomandi, binarnoIme);

    pid_t pid = fork();

    if (pid == 0) { 
        if (execv(punaPutanja, args) < 0) {
            fprintf(stderr, "Greska: Komanda '%s' nije pronadjena na putanji: %s\n", args[0], punaPutanja);
            exit(1);
        }
    } 
    else if (pid > 0) { 
        int status;
        waitpid(pid, &status, 0);
        return (WIFEXITED(status) && WEXITSTATUS(status) == 0);
    } 
    else {
        perror("Fork nije uspeo");
        return false;
    }

    return true;
}