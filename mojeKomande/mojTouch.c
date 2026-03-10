#include <stdio.h>
#include <utime.h>
#include <sys/stat.h>

int main(int argc, char* argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Upotreba: %s <ime_fajla>\n", argv[0]);
        return 1;
    }

    char* imeFajla = argv[1];

    if (utime(imeFajla, NULL) == 0) 
    {
        printf("Vreme fajla %s je azurirano.\n", imeFajla);
    } 
    else 
    {
        FILE* fajl = fopen(imeFajla, "a");
        if (fajl == NULL) 
        {
            perror("Greska prilikom kreiranja fajla");
            return 1;
        }
        printf("Fajl %s je uspesno napravljen.\n", imeFajla);
        fclose(fajl); 
    }

    return 0;
}