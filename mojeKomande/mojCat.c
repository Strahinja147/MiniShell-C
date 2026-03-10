#include <stdio.h>

int main(int argc, char* argv[])
{
    FILE* fajlZaCitanje;
    char linija[BUFSIZ];

    if (argc == 1) 
    {
        fajlZaCitanje = stdin;
    }
    else if (argc == 2)
    {
        fajlZaCitanje = fopen(argv[1], "r");
        if (fajlZaCitanje == NULL) 
        {
            perror("Greska pri otvaranju fajla");
            return 1;
        }
    }
    else 
    {
        fprintf(stderr, "Upotreba: %s [ime_fajla]\n", argv[0]);
        return 1;
    }

    while (fgets(linija, sizeof(linija), fajlZaCitanje) != NULL)
    {
        printf("%s", linija);
    }

    if (fajlZaCitanje != stdin)
    {
        fclose(fajlZaCitanje);
    }

    return 0;
}