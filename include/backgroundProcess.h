#ifndef BACKGROUNDPROCESS_H
#define BACKGROUNDPROCESS_H

#include <stdbool.h>

bool isBackgroundProcess(char** args, int brojArgumenata);
bool executeBackgroundProcess(char** args, int brojArgumenata);

#endif