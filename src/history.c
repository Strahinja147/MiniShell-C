#include "history.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>

#define HISTORY_MAX 2048

static char** istorijaKomandi;
static int trenutniBrojKomandi = 0;
static int pozicijaZaPregled = -1;

void inicijalizujIstoriju(void)
{
    istorijaKomandi  = (char**)malloc(HISTORY_MAX * sizeof(char*));
}    

void dodajKomandu(char* komanda)
{
    if(trenutniBrojKomandi < HISTORY_MAX)
    {
        istorijaKomandi[trenutniBrojKomandi] = malloc(strlen(komanda) + 1);
        if(istorijaKomandi[trenutniBrojKomandi] == NULL)
        {
            printf("Greska pri alokociji memorije!\n");
            exit(1);
        }
        
        strcpy(istorijaKomandi[trenutniBrojKomandi], komanda);
        trenutniBrojKomandi++;
        pozicijaZaPregled = trenutniBrojKomandi - 1;
    }
    else
    {
        printf("Dosli ste do maksimalnog broja komandi koje mogu da se sacuvaju u istoriji!\n");
    }
}

char* gornjaKomanda(void)
{
    if(trenutniBrojKomandi == 0)
        return NULL;

    char* poruka = istorijaKomandi[pozicijaZaPregled];

    pozicijaZaPregled--;
    if(pozicijaZaPregled < 0)
    {
        pozicijaZaPregled = 0;
    } 
    return poruka;
}
char* donjaKomanda(void)
{

    pozicijaZaPregled++;
    if(pozicijaZaPregled > trenutniBrojKomandi - 2)
    {
        pozicijaZaPregled = trenutniBrojKomandi - 1;
    }
    char* poruka = istorijaKomandi[pozicijaZaPregled];
    return poruka;
}
void prikaziIstoriju(void)
{
    if(trenutniBrojKomandi == 0)
    {
        printf("Trenutno nema nicega u istoriji\n");
        return;
    }
    for(int i = 0; i < trenutniBrojKomandi; i++)
    {
        printf("%d: %s\n", i, istorijaKomandi[i]);
    }
    printf("Trenutni broj komandi je: %d\n", trenutniBrojKomandi);
    printf("pozicija za pregled je: %d\n", pozicijaZaPregled);
}

void oslobodiIsotriju(void)
{
    for(int j = 0; j < trenutniBrojKomandi; j++)
    {
        free(istorijaKomandi[j]);
    }
    free(istorijaKomandi);
}