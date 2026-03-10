#include "utils.h"
#include "history.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <termios.h>
#include <dirent.h>

char* readLine(void)
{
   
    char buffer[100];
    int n = 100;
    if(fgets(buffer, n, stdin) == NULL)
    {
        printf("Greska pri unosu komande!");
        return NULL;
    }

    int duzinaUnosa = strlen(buffer);
    if(duzinaUnosa > 0 && buffer[duzinaUnosa - 1] == '\n')
    {
        buffer[duzinaUnosa - 1] = '\0';
    }


    char* linija = (char*)malloc((strlen(buffer) + 1) * sizeof(char));
    if(linija == NULL)  
    {
        printf("Greska pri alokaciji memorije!\n");
        return NULL;
    }

    strcpy(linija, buffer);

    return linija;
}

char** splitLine(char* line)
{

    int i = 0;

    char* token = strtok(line, " ");
    char** tokenizovanTekst = malloc(100 * sizeof(char*));
    if(tokenizovanTekst == NULL)
    {
        printf("Greska pri alokaciji memorije, za tokenizovanTekst!");
        return NULL;
    }

    while(token != NULL)
    {
        tokenizovanTekst[i] = (char*)malloc((strlen(token) + 1) * sizeof(char));
        if(tokenizovanTekst[i] == NULL)
        {
            printf("Greska pri alokaciji memorije za tokenizovaniTekst[i]!");
            return NULL;
        }
        strcpy(tokenizovanTekst[i], token);
        i++;
        token = strtok(NULL, " ");
    }
    tokenizovanTekst[i] = NULL;

    return tokenizovanTekst;
}

int brojArgumenata(char** argumenti)
{
    int i = 0;
    while(argumenti[i] != NULL)
    {
        i++;
    }
    return i;
}

void printPrompt(void)
{
    char tekuciRadniDirektorijum[100];
    getcwd(tekuciRadniDirektorijum, 100);
    printf("%s> ", tekuciRadniDirektorijum);
    fflush(stdout);
}

bool brojArgumenataValidan(int brojArgumenata, int brojZaProveru)
{
    if(brojArgumenata != brojZaProveru)
    {
        return false;
    }
    else
    {
        return true;
    }
}

void postaviRawMod(void)
{
    struct termios t;
    tcgetattr(STDIN_FILENO, &t);
    t.c_lflag &= ~(ICANON | ECHO); 
    tcsetattr(STDIN_FILENO, TCSANOW, &t);    
}

void postaviNaCannonicMod(void)
{
    struct termios t;
    tcgetattr(STDIN_FILENO, &t);
    t.c_lflag |= (ICANON | ECHO); 
    tcsetattr(STDIN_FILENO, TCSANOW, &t);
}

char* novoCitanjeLinija(void)
{
    char c;
    char buffer[1024];
    int poz = 0;
    int pomeriKursor = 1;

    postaviRawMod();

    while (1) 
    {
            
        read(STDIN_FILENO, &c, 1);

        if (c == '\n') 
        {
            buffer[poz] = '\0';
            //printf("\nUnos: %s\n", buffer);
            break;
        } 
        else if (c == 127) 
        { // backspace
            if (poz > 0) 
            {
                poz--;
                printf("\b \b"); 
                fflush(stdout);
            }
        } 
        else if (c == 27) 
        { // pocetak escape sekvence
            char seq[2];
            if (read(STDIN_FILENO, &seq[0], 1) == 1 &&
                read(STDIN_FILENO, &seq[1], 1) == 1) {
                if (seq[0] == '[' && seq[1] == 'A') 
                {
                    char* poruka = gornjaKomanda();
                    char tekuciRadniDirektorijum[100];
                    getcwd(tekuciRadniDirektorijum, 100);
                    printf("\r"); // vrati kursor na pocetak linije
                    printf("\033[K"); // obrisi celu liniju od kursora do kraja
                    printf("%s> %s", tekuciRadniDirektorijum, poruka);
                    fflush(stdout);
                    strcpy(buffer, poruka);
                    poz = strlen(buffer);
                } 
                else if (seq[0] == '[' && seq[1] == 'B') 
                {
                    
                    char* poruka = donjaKomanda();
                    char tekuciRadniDirektorijum[100];
                    getcwd(tekuciRadniDirektorijum, 100);
                    printf("\r"); // vrati kursor na pocetak linije
                    printf("\033[K"); // obrisi celu liniju od kursora do kraja
                    printf("%s> %s", tekuciRadniDirektorijum, poruka);
                    fflush(stdout);     
                    strcpy(buffer, poruka);
                    poz = strlen(buffer);
                }
                else if (seq[0] == '[' && seq[1] == 'C')
                {
                    //printf("Kliknuto si desnu strelicu!\n");
                    //printf("\r");
                    printf("\033[%dC", pomeriKursor);
                }
                else if (seq[0] == '[' && seq[1] == 'D')
                {
                    //printf("kliknuo si levu strelicu;\n");
                    //printf("\r");
                    printf("\033[%dD", pomeriKursor);
                }
            }
        } 
        else 
        {
            buffer[poz++] = c;
            write(STDOUT_FILENO, &c, 1);
        }
    }
    postaviNaCannonicMod();
    int duzinaUnosa = strlen(buffer);
    if(duzinaUnosa > 0 && buffer[duzinaUnosa - 1] == '\n')
    {
        buffer[duzinaUnosa - 1] = '\0';
    }


    char* linija = (char*)malloc((strlen(buffer) + 1) * sizeof(char));
    if(linija == NULL)  
    {
        printf("Greska pri alokaciji memorije!\n");
        return NULL;
    }

    strcpy(linija, buffer);
    printf("\n");

    return linija;
}

bool checkForRedirect(char** args)
{

    for (int i = 1; args[i] != NULL; i++)
    {
        if (strcmp(args[i], ">") == 0  || 
            strcmp(args[i], "<") == 0  || 
            strcmp(args[i], ">>") == 0 || 
            strcmp(args[i], "2>") == 0 || 
            strcmp(args[i], "&>") == 0)
        {
            return true; 
        }
    }

    return false; 
}

bool checkForPipe(char** args)
{

    for (int i = 1; args[i] != NULL; i++)
    {
        if (strcmp(args[i], "|") == 0)
            return true; 
    }

    return false; 
}

void zameniArgumente(char** args, char*** izmenjeniArgumenti, int brojArgumenata)
{
    char* pom;
   
    *izmenjeniArgumenti = malloc(brojArgumenata * sizeof(char*));
    for(int i = 0; i < brojArgumenata; i++)
    {
        (*izmenjeniArgumenti)[i] = malloc(strlen(args[i]) + 1);
        strcpy((*izmenjeniArgumenti)[i], args[i]);
    }

    pom = (*izmenjeniArgumenti)[0];
    (*izmenjeniArgumenti)[0] = (*izmenjeniArgumenti)[brojArgumenata - 1];
    (*izmenjeniArgumenti)[brojArgumenata - 1] = pom;
}

void obrsiSveFajloveIzDirektorijuma(char* pathDirektorijuma)
{
    DIR* dp;
    struct dirent* dirp;
    dp = opendir(pathDirektorijuma);
    char putDoFajla[PATH_MAX];
    while((dirp = readdir(dp)) != NULL)
    {
        if (strcmp(dirp->d_name, ".")==0 || strcmp(dirp->d_name, "..")==0)
            continue;

        snprintf(putDoFajla, sizeof(putDoFajla), "%s/%s", pathDirektorijuma, dirp->d_name);
        remove(putDoFajla);
    }
    closedir(dp);
}

void ocistiPreIzlaska(char** args)
{
    for (int i = 0; args[i] != NULL; i++)
        free(args[i]);
    free(args);

    oslobodiIsotriju();
    obrsiSveFajloveIzDirektorijuma("/home/strale/miniShell/logs");
}