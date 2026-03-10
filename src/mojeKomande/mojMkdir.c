#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(int argc, char* argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Upotreba: %s <ime_direktorijuma>\n", argv[0]);
        return 1;
    }

    const char* zeljeniDirektorijum = argv[1];

    if (mkdir(zeljeniDirektorijum, 0777) == -1)
    {
        perror("Greška prilikom kreiranja direktorijuma");
        return 1; 
    }
    else
    {
        printf("Uspesno napravljen direktorijum: %s\n", zeljeniDirektorijum);
    }

    return 0;
}