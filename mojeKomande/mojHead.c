#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>

int daLiJeBroj(const char* s) {
    if (s == NULL || *s == '\0') return 0;
    for (int i = 0; s[i] != '\0'; i++) {
        if (!isdigit(s[i])) return 0;
    }
    return 1;
}

int main(int argc, char* argv[])
{
    FILE* ulaz = stdin; 
    int brojLinija = 10;     
    char* imeFajla = NULL;

    if (argc == 1) {
        ulaz = stdin;
    }
    else if (argc == 2) {
        if (daLiJeBroj(argv[1])) {
            brojLinija = atoi(argv[1]);
            ulaz = stdin;
        } else {
            imeFajla = argv[1];
        }
    }
    else if (argc == 3) {
        imeFajla = argv[1];
        brojLinija = atoi(argv[2]);
    }
    else {
        fprintf(stderr, "Upotreba: %s [fajl] [broj_linija] ili komanda | %s [broj_linija]\n", argv[0], argv[0]);
        return 1;
    }

    if (imeFajla != NULL) {
        ulaz = fopen(imeFajla, "r");
        if (ulaz == NULL) {
            perror("Greška pri otvaranju fajla");
            return 1;
        }
    }

    char linija[BUFSIZ];
    for (int i = 0; i < brojLinija; i++) {
        if (fgets(linija, sizeof(linija), ulaz) != NULL) {
            printf("%s", linija);
        } else {
            break;
        }
    }

    if (imeFajla != NULL && ulaz != NULL) {
        fclose(ulaz);
    }

    return 0;
}