#include <stdio.h>
#include <time.h>

int main(int argc, char* argv[])
{
    if (argc != 1) {
        fprintf(stderr, "Upotreba: %s\n", argv[0]);
        return 1; 
    }

    time_t trenutnoVreme;
    struct tm* vremenskaStruktura;
    trenutnoVreme = time(NULL);
    vremenskaStruktura = localtime(&trenutnoVreme);
    if (vremenskaStruktura == NULL) {
        fprintf(stderr, "Greška: Nije moguće dobiti lokalno vreme.\n");
        return 1;
    }

    printf("%s", asctime(vremenskaStruktura));

    return 0;
}