#include <stdio.h>
#include <stdlib.h>
#include "tetris.h"
#include <termios.h>
#include <signal.h>
#include <time.h>
#include <fcntl.h>

void stat_match(void){
    
}

void istruction(void){
    
}

void title(void){
    system("clear");
    printf("\n------------------------------------------------");
    printf("\n-   |T|                |R|          |S|        -");
    printf("\n-              |T|           |I|               -");
    printf("\n-       |E|                         Project°   -");
    printf("\n------------------------------------------------");
    printf("\n");
}

int main(void) {
    int scelta;
    do{
        printf("------------------------------------------------\n");
        printf("-               TETRIS Project°                -\n");
        printf("------------------------------------------------\n");
        printf("- 1) Singleplayer                              -\n");
        printf("- 2) Multiplayer                               -\n");
        printf("- 3) Stat match precedenti                     -\n");
        printf("- 4) Istruzioni                                -\n");
        printf("- 0) Esci                                      -\n");
        printf("------------------------------------------------\n");
        printf("- Scegli:");
        scanf("%d", &scelta);
        switch (scelta) {
            case 1:
                tetris_run(10, 15);
                break;
            case 2:
                tetris_run(10, 15);
                break;
            case 3:
                stat_match();
                break;
            case 4:
                istruction();
                break;
            case 0:
                printf("- Sicuro di voler uscire? \n");
                break;
            default:
                printf("- Opzione non disponibile.(Premere (1,2,3,4,0) per accedere alle voci descritte) \n");
                break;
        }
        
        printf("------------------------------------------------\n");
        printf("- 1) Continua                                  -\n");
        printf("- 0) Esci                                      -\n");
        printf("------------------------------------------------\n");
        printf("- Scegli: ");
        scanf("%d", &scelta);   
    }while(scelta!=0);
    
    printf("------------------------------------------------\n");
    printf("-               TETRIS Project°                -\n");
    printf("------------------------------------------------\n");
    printf("- Grazie per aver utilizzato questo software   -\n");
    printf("-                                              -\n");
    printf("- Edited by: Ramolivaz André                   -\n");
    printf("------------------------------------------------\n");
    return 0;
}

