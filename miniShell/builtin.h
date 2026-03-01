#include <stdbool.h>
#ifndef BUILTIN_C
#define BUILTIN_C

bool isBuiltin(char* arg);
bool executeBuiltin(char** args, int brojArgumenata);
bool executeCd(char** sviArgumenit, int brojArgumenata);
bool executeClear(int brojArgumenata);
bool executeExit(char** args, int brojArgumenata);
bool executeHelp(char** args, int brojArgumenata);
bool executeHistory(int brojArgumenata);


#endif