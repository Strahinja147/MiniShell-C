#include <stdbool.h>
#ifndef PIPE_H
#define PIPE_H

bool isPipe(char* arg1, char* arg2, int* brojArgumenataKomandeZaUlazUPipe, int* brojArgumenataKomandeZaIzlazIzPipe);
bool executePipe(char** args, int brojArgumenata);
bool mojPipe(char** args, int brojArgumenataKomandeZaUlazUPipe, int brojArgumenataKomandeZaIzlazIzPipe);

#endif