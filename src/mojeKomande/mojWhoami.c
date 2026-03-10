#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

int main(int argc, char* argv[])
{
    if(argc != 1)
    {
        fprintf(stderr, "Upotreba: %s\n", argv[0]);
        return 1;
    }

    struct passwd *korisnikPodaci;
    uid_t korisnikID = getuid();

    korisnikPodaci = getpwuid(korisnikID);

    if (korisnikPodaci == NULL)
    {
        perror("Nije uspelo dobijanje podataka o korisniku");
        return 1;
    }

    printf("%s\n", korisnikPodaci->pw_name);

    return 0;
}