#include <stdio.h>

int main(int argc, char* argv[])
{

    if (argc != 2) {
        fprintf(stderr, "Upotreba: %s <ime_fajla>\n", argv[0]);
        return 1;
    }

    char* imeFajlaZaBrisanje = argv[1];

    if (remove(imeFajlaZaBrisanje) == 0)
    {
        printf("Fajl '%s' je uspesno obrisan.\n", imeFajlaZaBrisanje);
    }
    else
    {
        perror("Greska prilikom brisanja fajla");
        return 1;
    }

    return 0;
}