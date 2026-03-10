#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <ctype.h>

// Pomoćna funkcija koja proverava da li je string broj
int daLiJeBroj(const char* s) {
    if (s == NULL || *s == '\0') return 0;
    for (int i = 0; s[i] != '\0'; i++) {
        if (!isdigit(s[i])) return 0;
    }
    return 1;
}

// Implementacija za pajp (stdin) - koristi kružni bafer (Circular Buffer)
void tailIzPajpa(int brojLinija) {
    if (brojLinija <= 0) return;

    // Alokacija niza pokazivača za čuvanje linija
    char** bafer = (char**)calloc(brojLinija, sizeof(char*));
    char linija[BUFSIZ];
    int trenutniIndeks = 0;
    int ukupnoLinija = 0;

    // Čitamo sve iz stdin-a
    while (fgets(linija, sizeof(linija), stdin) != NULL) {
        if (bafer[trenutniIndeks]) {
            free(bafer[trenutniIndeks]);
        }
        bafer[trenutniIndeks] = strdup(linija);
        trenutniIndeks = (trenutniIndeks + 1) % brojLinija;
        ukupnoLinija++;
    }

    // Određujemo odakle počinje ispis
    int start = (ukupnoLinija > brojLinija) ? trenutniIndeks : 0;
    int brojZaIspis = (ukupnoLinija > brojLinija) ? brojLinija : ukupnoLinija;

    for (int i = 0; i < brojZaIspis; i++) {
        int index = (start + i) % brojLinija;
        printf("%s", bafer[index]);
        free(bafer[index]);
    }
    free(bafer);
}

int main(int argc, char* argv[]) {
    int brojLinijaZaCitanje = 10;
    char* imeFajla = NULL;

    // 1. Parsiranje argumenata
    if (argc == 1) {
        tailIzPajpa(brojLinijaZaCitanje);
        return 0;
    } 
    else if (argc == 2) {
        if (daLiJeBroj(argv[1])) {
            brojLinijaZaCitanje = atoi(argv[1]);
            tailIzPajpa(brojLinijaZaCitanje);
            return 0;
        } else {
            imeFajla = argv[1];
        }
    } 
    else if (argc == 3) {
        imeFajla = argv[1];
        brojLinijaZaCitanje = atoi(argv[2]);
    } 
    else {
        fprintf(stderr, "Upotreba: %s <fajl> [broj_linija] ili komanda | %s [broj_linija]\n", argv[0], argv[0]);
        return 1;
    }

    // 2. Rad sa fajlom (fseek metoda)
    FILE* file = fopen(imeFajla, "r");
    if (!file) {
        perror("Greška prilikom otvaranja fajla");
        return 1;
    }

    // Idemo na kraj fajla
    fseek(file, 0, SEEK_END);
    long velicina = ftell(file);
    
    if (velicina > 0) {
        long pozicija = velicina - 1;
        int pronadjenoLinija = 0;

        // Ako je poslednji karakter novi red, preskoči ga da ne bismo odmah završili
        fseek(file, pozicija, SEEK_SET);
        if (fgetc(file) == '\n') {
            if (pozicija > 0) pozicija--;
        }

        // Tražimo linije unazad
        while (pozicija >= 0 && pronadjenoLinija < brojLinijaZaCitanje) {
            fseek(file, pozicija, SEEK_SET);
            if (fgetc(file) == '\n') {
                pronadjenoLinija++;
            }
            if (pronadjenoLinija < brojLinijaZaCitanje) {
                pozicija--;
            }
        }

        // Pomeramo kursor na početak prve linije koju treba štampati
        // Ako smo stali na \n, pomeramo se za 1 bajt napred
        if (pozicija >= 0 && pronadjenoLinija == brojLinijaZaCitanje) {
            fseek(file, pozicija + 1, SEEK_SET);
        } else {
            fseek(file, 0, SEEK_SET);
        }
    } else {
        fseek(file, 0, SEEK_SET);
    }

    // Štampanje sadržaja od te pozicije do kraja
    char bafer[BUFSIZ];
    while (fgets(bafer, sizeof(bafer), file) != NULL) {
        printf("%s", bafer);
    }

    fclose(file);
    return 0;
}