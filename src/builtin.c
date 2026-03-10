#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "builtin.h"
#include "utils.h"
#include "history.h"
#include <sys/stat.h>
#include <stdbool.h>

typedef struct
{
    const char* imeKomande;
    bool (*funkcija)(char**, int);
} KomandaSaDvaArgumentaBuiltin;

typedef struct
{
    const char* imeKomande;
    bool (*funkcija)(int);
} KomandaSaJednimArgumentomBuiltin;


KomandaSaDvaArgumentaBuiltin komandeSaDvaArgumentaBuiltin[] = {
    {"cd", executeCd},
    {"exit", executeExit},
    {"help", executeHelp}
};


KomandaSaJednimArgumentomBuiltin komandeSaJednimArgumentomBuiltin[] = {
    {"clear", executeClear},
    {"history", executeHistory}
};


bool isBuiltin(char* arg)
{

    for(size_t i = 0; i < sizeof(komandeSaDvaArgumentaBuiltin) / sizeof(KomandaSaDvaArgumentaBuiltin); i++)
    {
        if(strcmp(arg, komandeSaDvaArgumentaBuiltin[i].imeKomande) == 0)
            return true;
    }
    for(size_t i = 0; i < sizeof(komandeSaJednimArgumentomBuiltin) / sizeof(KomandaSaJednimArgumentomBuiltin); i++)
    {
        if(strcmp(arg, komandeSaJednimArgumentomBuiltin[i].imeKomande) == 0)
            return true;
    }

    return false;
}

bool executeBuiltin(char** args, int brojArgumenata)
{
    for(size_t i = 0; i < sizeof(komandeSaDvaArgumentaBuiltin) / sizeof(KomandaSaDvaArgumentaBuiltin); i++)
    {
        if(strcmp(args[0], komandeSaDvaArgumentaBuiltin[i].imeKomande) == 0)
            return komandeSaDvaArgumentaBuiltin[i].funkcija(args, brojArgumenata);
    }
    for(size_t i = 0; i < sizeof(komandeSaJednimArgumentomBuiltin) / sizeof(KomandaSaJednimArgumentomBuiltin); i++)
    {
        if(strcmp(args[0], komandeSaJednimArgumentomBuiltin[i].imeKomande) == 0)
            return komandeSaJednimArgumentomBuiltin[i].funkcija(brojArgumenata);
    }
    return false;
}

bool executeCd(char** sviArgumenti, int brojArgumenata)
{
    bool uspesnoOdradjenaFunkcija = false;
    int validanBrojArgumenata = 2;
    if(!brojArgumenataValidan(brojArgumenata, validanBrojArgumenata))
    {
        printf("Nije adekvatan broj argumenata, ova metoda mora da ima dva argumenta.\nFormat komande mora da bude 'cd zeljeniDirektorijum'!\n");
    }
    else
    {
        const char* zeljeniDirektorijum = sviArgumenti[1];

        if(chdir(zeljeniDirektorijum) == -1)
        {
            char poruka[200];
            snprintf(poruka, sizeof(poruka), "Menjanje direktorijuma u %s nije uspelo", zeljeniDirektorijum);
            perror(poruka);
        }
        else
        {
            uspesnoOdradjenaFunkcija = true;
        }

        //printf("%s\n", getcwd(s, 100));
    }
    return uspesnoOdradjenaFunkcija;
}


bool executeClear(int brojArgumenata)
{
    bool uspesnoOdradjenaFunkcija = false;
    int validanBrojArgumenata = 1;
    if(!brojArgumenataValidan(brojArgumenata, validanBrojArgumenata))
    {
        printf("Nije adekvatan broj argumenata, ova metoda mora da ima jedan argument.\nFormat komande mora da bude samo 'clear'.\n");
    }      
    else
    {
        //system("clear"); u sustini moze i ovo cudo ali ovo dole je vise profi jer direktno pricas sa terminalom i ne zoves vec napravljeni clear
        printf("\033[H\033[J");// ANSI Escape Sequence
        uspesnoOdradjenaFunkcija = true;
    }
    return uspesnoOdradjenaFunkcija;
}

bool executeExit(char** args, int brojArgumenata)
{
    bool uspesnoOdradjenaFunkcija = false;
    int validanBrojArgumenata = 1;
    if(!brojArgumenataValidan(brojArgumenata, validanBrojArgumenata))
    {
        printf("Nije adekvatan broj argumenata, ova metoda mora da ima jedan argument.\nFormat komande mora da bude samo 'exit'.\n");
        //uspesnoOdradjenaFunkcija = false;// ovde moze i da se napise return false nego ajde ovako za sad
    }
    else
    {
        ocistiPreIzlaska(args);
        uspesnoOdradjenaFunkcija = true;
        exit(1);
    }

    return uspesnoOdradjenaFunkcija;
}

bool executeHelp(char** sviArgumenti, int brojArgumenata)
{
    int validanBrojArgumenata = 2;
    if(!brojArgumenataValidan(brojArgumenata, validanBrojArgumenata))
    {
        printf("Nije adekvatan broj argumenata, ova metoda mora da ima dva argumenta.\nFormat komande mora da bude 'help zeljenaKomanda'!\n");
    }
    typedef struct
    {
        const char* imeKomande;
        const char* helpTekstZaKomandu;
    } HelpPoruka;

    HelpPoruka helpPoruke[] = {
        {"cd", "cd [direktorijum] - Menja trenutni radni direktorijum.\n"},
        {"exit", "exit - Zatvara shell i zavrsava program.\n"},
        {"clear", "clear - Brise sve sa ekrana.\n"},
        {"mkdir", "mkdir [ime_direktorijuma] - Kreira novi direktorijum sa zadatim imenom.\n"},
        {"rmdir", "rmdir [ime_direktorijuma] - Brise postojeci prazan direktorijum.\n"},
        {"cat", "cat [ime_fajla] - Ispisuje sadrzaj fajla na standardni izlaz.\n"},
        {"echo", "echo [tekst] - Ispisuje zadati tekst na standardni izlaz.\n"},
        {"ls", "ls [direktorijum] - Prikazuje listu fajlova i direktorijuma (ako se ne navede, koristi trenutni direktorijum).\n"},
        {"whoami", "whoami - Prikazuje korisnicko ime trenutno prijavljenog korisnika.\n"},
        {"date", "date - Prikazuje trenutni datum i vreme.\n"},
        {"pwd", "pwd - Ispisuje punu putanju trenutnog radnog direktorijuma.\n"},
        {"ps", "ps - Prikazuje informacije o trenutno aktivnim procesima.\n"},
        {"touch", "touch [ime_fajla] - Kreira novi prazan fajl sa zadatim imenom.\n"},
        {"rm", "rm [ime_fajla] - Brise fajl sa zadatim imenom.\n"},
        {"cp", "cp [izvorni_fajl] [odredisni_fajl] - Kopira sadrzaj iz jednog fajla u drugi.\n"},
        {"head", "head [ime_fajla] - Prikazuje prvih 10 linija iz fajla.\n"},
        {"tail", "tail [ime_fajla] - Prikazuje poslednjih 10 linija iz fajla.\n"},
        {"hisotry", "history - Prikazuje isotriju svih komandi koje su do sada koriscene.\n"}
    };
    char* helpKomanda = sviArgumenti[1];
    bool nadjenaKomanda = false;
    for(size_t i = 0; i < sizeof(helpPoruke) / sizeof(HelpPoruka); i++)
    {
        if(strcmp(helpKomanda, helpPoruke[i].imeKomande) == 0)
        {
            nadjenaKomanda = true;
            printf("%s", helpPoruke[i].helpTekstZaKomandu);
            return true;
        }
    }
    if(!nadjenaKomanda)
    {
        printf("%s komanda ne postoji!\n", helpKomanda);
    }

    return false;
}

bool executeHistory(int brojArgumenata)
{
    bool uspesnoOdradjenaFunkcija = false;
    int validanBrojArgumenata = 1;
    if(!brojArgumenataValidan(brojArgumenata, validanBrojArgumenata))
    {
        printf("Nije adekvatan broj argumenata, ova metoda mora da ima jedan argument.\nFormat komande mora da bude 'history'!\n");
    }
    else
    {
        prikaziIstoriju();
        uspesnoOdradjenaFunkcija = true;
    }
    return uspesnoOdradjenaFunkcija;
}
