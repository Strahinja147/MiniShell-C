#include <stdio.h>
#include <unistd.h>

int main(int argc, char* argv[])
{

    if (argc != 2) {
        fprintf(stderr, "Upotreba: %s <ime_direktorijuma>\n", argv[0]);
        return 1;
    }

    const char* zeljeniDirektorijum = argv[1];

    if (rmdir(zeljeniDirektorijum) == -1)
    {
        perror("Greška prilikom brisanja direktorijuma");
        return 1;
    }
    else
    {
        printf("Uspesno izbrisan direktorijum: %s\n", zeljeniDirektorijum);
    }

    return 0;
}