#ifndef EXTERNAL_H
#define EXTERNAL_H

#include <stdbool.h>

void inicijalizujPutanje(void);
bool isExternal(char* arg);
bool executeExternal(char** args, int brojArgumenata);

#endif