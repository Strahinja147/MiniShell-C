#include <stdbool.h>
#ifndef HISTORY_H
#define HISTORY_H

void dodajKomandu(char* komanda);
char* gornjaKomanda(void);
char* donjaKomanda(void);
void prikaziIstoriju(void);
void oslobodiIsotriju(void);
void inicijalizujIstoriju(void);

#endif