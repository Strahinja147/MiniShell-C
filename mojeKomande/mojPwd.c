#include <stdio.h>
#include <unistd.h>
#include <limits.h> 

int main(int argc, char* argv[])
{
    if(argc != 1)
    {
        fprintf(stderr, "Upotreba: %s\n", argv[0]);
        return 1;
    }

    char trenutniDirektorijum[PATH_MAX];

    if (getcwd(trenutniDirektorijum, sizeof(trenutniDirektorijum)) != NULL) 
    {
        printf("%s\n", trenutniDirektorijum);
    } 
    else 
    {
        perror("pwd greška");
        return 1;
    }

    return 0;
}