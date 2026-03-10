#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <ctype.h>
#include <stdbool.h>
#include <errno.h>


bool daLiJeBroj(const char* s) {
    for (int i = 0; s[i] != '\0'; i++) {
        if (!isdigit(s[i])) return false;
    }
    return true;
}

int main(int argc, char* argv[]) {

    if(argc != 1)
    {
        fprintf(stderr, "Upotreba: %s\n", argv[0]);
        return 1;
    }
    DIR* procDp = opendir("/proc");
    struct dirent* procUnos;

    if (procDp == NULL) {
        perror("Greška prilikom otvaranja /proc direktorijuma");
        return 1;
    }

    printf("%-8s %s\n", "PID", "NAZIV PROCESA");
    printf("--------------------------\n");

    while ((procUnos = readdir(procDp)) != NULL) {
       
        if (!daLiJeBroj(procUnos->d_name)) {
            continue;
        }

        char putanjaStatus[512];
        snprintf(putanjaStatus, sizeof(putanjaStatus), "/proc/%s/status", procUnos->d_name);

        FILE* f = fopen(putanjaStatus, "r");
        if (f == NULL) {
            
            continue;
        }

        char linija[256];
        if (fgets(linija, sizeof(linija), f)) {

            char* ime = linija + 5; 
            while (*ime == ' ' || *ime == '\t') {
                ime++;
            }

            ime[strcspn(ime, "\n")] = 0;

            printf("%-8s %s\n", procUnos->d_name, ime);
        }

        fclose(f);
    }

    closedir(procDp);
    return 0;
}