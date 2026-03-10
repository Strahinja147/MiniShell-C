#include <stdbool.h>
#ifndef REDIRECTION_H
#define REDIRECTION_H

bool isRedirect(char* imeKomandeZaRedirekciju, int* brojArgumenataKomandeZaRedirekciju, int brojArgumenata);
bool executeRedirect(char** arg);
bool redirectOutputOverwrite(char** args, int brojArgumenata);// >
bool redirectOutputAppend(char** args, int brojArgumenata);// >>
bool redirectErrorOutput(char** args, int brojArgumenata);// 2>
bool redirectErrorAndOutput(char** args, int brojArgumenata);// &>


#endif