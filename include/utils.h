#include <stdbool.h>
#ifndef UTILS_H
#define UTILS_H

char* readLine(void);
char** splitLine(char* line);
int brojArgumenata(char** argumenti);
void printPrompt(void);
bool brojArgumenataValidan(int brojArgumenata, int brojZaProveru);
void postaviRawMod(void);
void postaviNaCannonicMod(void);
char* novoCitanjeLinija(void);
bool checkForRedirect(char** args);
bool checkForPipe(char** args);
void zameniArgumente(char** args, char*** izmenjeniArgumenti, int brojArgumenata);
void ocistiPreIzlaska(char** args);
void obrsiSveFajloveIzDirektorijuma(char* imeDirektorijuma);


#endif