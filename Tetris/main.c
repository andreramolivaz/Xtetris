#include <stdio.h>
#include <stdlib.h>
#include "tetris.h"
#include <termios.h>
#include <signal.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <windows.h>

void stat_match(void){
    
}

void title(void)
{
    system("clear");
    printf("\n------------------------------------------------");
    printf("\n-   |T|                |R|          |S|        -");
    printf("\n-              |T|           |I|               -");
    printf("\n-       |E|                         Project°   -");
    printf("\n------------------------------------------------");
    printf("\n");
}

void istruction(void)
{
    title();
    printf("Comandi di gioco: \n");
    printf("d-> destra  \n");
    printf("a-> sinistra  \n");
    printf("s-> velocizza la discesa \n");
}

int main(void) {
    int scelta;
    do{
        system("clear");

        printf("------------------------------------------------\n");
        printf("-               TETRIS Project°                -\n");
        printf("------------------------------------------------\n");
        printf("- 1) Singleplayer                              -\n");
        printf("- 2) Multiplayerv                              -\n");
        printf("- 3) Statistiche                               -\n");
        printf("- 4) Istruzioni                                -\n");
        printf("- 0) Esci                                      -\n");
        printf("------------------------------------------------\n");

        printf("- Scegli:");
        sleep(3);
        scanf("%d", &scelta);
        switch (scelta) {
            case 1:
                system("clear");
                tetris_run(10, 15);

                break;
            case 2:
                system("clear");
                tetris_run(10, 15);
                break;
            case 3:
                system("clear");
                stat_match();
                break;
            case 4:
                system("clear");
                istruction();
                break;
            case 0:
                system("clear");
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
        sleep(3);
        scanf("%d", &scelta);
    }while(scelta!=0);
    system("clear");
    printf("------------------------------------------------\n");
    printf("-               TETRIS Project°                -\n");
    printf("------------------------------------------------\n");
    printf("- Grazie per aver utilizzato questo software   -\n");
    printf("-                                              -\n");
    printf("- Edited by: The Tetris Terminator team        -\n");
    printf("------------------------------------------------\n");
    return 0;
}

