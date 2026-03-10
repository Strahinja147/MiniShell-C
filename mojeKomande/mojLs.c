#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <limits.h>
#include <unistd.h>

int main(int argc, char* argv[])
{

    if (argc > 2) {
        fprintf(stderr, "Upotreba: %s [putanja_direktorijuma]\n", argv[0]);
        return 1;
    }

    DIR* dp;
    struct dirent *dirp;
    char putanja[PATH_MAX];

    if (argc == 1)
    {
        strcpy(putanja, ".");
    }
    else
    {
        strncpy(putanja, argv[1], PATH_MAX - 1);
        putanja[PATH_MAX - 1] = '\0';
    }

    dp = opendir(putanja);
    
    if (dp == NULL)
    {
        fprintf(stderr, "ls: Ne mogu da otvorim direktorijum '%s': ", putanja);
        perror(""); 
        return 1;
    }

    while ((dirp = readdir(dp)) != NULL)
    {
        if (strcmp(dirp->d_name, ".") == 0 || strcmp(dirp->d_name, "..") == 0)
        {
            continue;
        }

        printf("%s  ", dirp->d_name);
    }

    printf("\n");

    closedir(dp);
    return 0;
}