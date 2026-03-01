#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "utils.h"
#include "builtin.h"
#include "history.h"
#include "redirection.h"
#include "external.h"
#include "pipe.h"
#include "backgroundProcess.h"

int main(int argc, char* argv[])
{
    inicijalizujIstoriju();
    //signal(SIGCHLD, SIG_IGN);
    inicijalizujPutanje();
    while(1)
    {
        printPrompt();
        char* linija = novoCitanjeLinija();
        dodajKomandu(linija);
        char** args = splitLine(linija);
        int trenutniBrojArgumenata = brojArgumenata(args);
        
        if(args[0] == NULL) continue; 

        if(isBackgroundProcess(args, trenutniBrojArgumenata))
        {
            args[trenutniBrojArgumenata-1] = NULL;
            trenutniBrojArgumenata--;
            if(!executeBackgroundProcess(args, trenutniBrojArgumenata))
            {
                printf("Nije mogao da se pokrene background process.\n");
                continue;
            }
        }
        else if(checkForPipe(args) && !isBackgroundProcess(args, trenutniBrojArgumenata))
        {
            if(!executePipe(args, trenutniBrojArgumenata))
            {
                printf("Pipe nije mogao uspesno da se obavi.\n");
                continue;
            }
        }
        else if(checkForRedirect(args) && !isBackgroundProcess(args, trenutniBrojArgumenata))
        {
            if(!executeRedirect(args, trenutniBrojArgumenata))
            {
                printf("Redirekcija nije mogla uspesno da se obavi.\n");
                continue;
            }
        }
        else if(isBuiltin(args[0]) && !isBackgroundProcess(args, trenutniBrojArgumenata))
        {
            if(!executeBuiltin(args, trenutniBrojArgumenata))
            {
                printf("%s nije moglo uspesno da se uradi!\n", args[0]);
                continue;
            }
        }
        else if(isExternal(args[0]) && !isBackgroundProcess(args, trenutniBrojArgumenata))
        {
            if(!executeExternal(args, trenutniBrojArgumenata))
            {
                printf("%s nije moglo uspesno da se uradi!\n", args[0]);
                continue;
            }
        }
        else
        {
            printf("\nNije dobro uneo si %s\n", args[0]);
        }
        if (args != NULL) {
            for (int i = 0; args[i] != NULL; i++) {
                free(args[i]);
            }
            free(args); 
        }
        
        free(linija);
    }
    
    return 0;
}