/**
 * @file tetris.c
 */

#include <stdio.h>
#include <stdlib.h>
#include "tetris.h"

#include <termios.h>

#include <time.h>

#include <fcntl.h>


#include <AudioToolbox/AudioServices.h>

/**
 * Indica la modalità di gioco selezionata
 */
enum mode {
    SINGLE_PLAYER, /**< Giocatore Singolo*/
    MULTI_PLAYER   /**< Multi Player*/
};

/**
 * Per ogni tetramino, viene indicato il numero di pezzi disponibili
 */
int num_current[5];


/**
 * E' la struttura del gioco
 */
struct tetris {
    char **game;
    int w;          /**< La larghezza del campo */
    int h;          /**< L'altezza del campo */
    int gameover;   /**< flag che indica se il gioco è terminato o meno */
    int score;      /**< contatore del punteggio */
    /**
     Indica il blocco
     */
    struct tetris_block {
        char data[5][5]; /**< il blocco */
        int w;           /**< la larghezza del blocco */
        int h;           /**< l'altezza del blocco */
    } current;
    int x; /**< posizione x */
    int y; /**< posizione y */
};

/**
 * Indica i blocchi del gioco
 */
struct tetris_block blocks[] =
{
    {{"00", "00"}, 2, 2},       /**< quadrato, larghezza, altezza */
    {{" 1 ", "111"}, 3, 2},     /**< T, larghezza, altezza */
    {{"2222"}, 4, 1 },          /**< Linea, larghezza, altezza */
    {{"33", "3 ", "3 "}, 2, 3}, /**< L, larghezza, altezza */
    {{"44 ", " 44"}, 3, 2}      /**< S, larghezza, altezza */
    
};




/**
 * Struttura che rappresenta il terminale
 */
struct termios save;


/**
 * Disattiva la modalità RAW del terminale
 */
void tetris_cleanup_io()
{
    ///salva dentro save le informazioni del terminale in uso
    tcsetattr(0,TCSANOW,&save);
    ///tcsetattr() sets the parameters associated with the terminal from the termios structure. optional_actions specifies when the changes take effect: TCSANOW the change occurs immediately.
    
    
    int fd = fileno(stdin);
    int flags = fcntl(fd, F_GETFL);
    ///non aspetta che la funzione read() riceva un input
    flags &= ~O_NONBLOCK;
    fcntl(fd, F_SETFL, flags);
    
}

/**
 * Attiva la modalità RAW
 */

void tetris_set_ioconfig()
{
    ///il terminale da modificare
    struct termios custom;
    
    int fd = fileno(stdin);///stdin =  variabile puntatore a file che si riferisce allo stream di standard input, tastiera
    
    
    tcgetattr(fd, &save);///gets the parameters associated with the object referred by fd and stores them in the termios structure referenced by termios_p.  This function may be invoked from a background process; however, the terminal attributes may be subsequently changed by a foreground process.
    custom = save;
    ///disattivo la modalità canonica e l'echo
    custom.c_lflag &= ~(ICANON | ECHO);/// The setting of the ICANON canon flag in c_lflag determines whether the terminal is operating in canonical mode (ICANON set) or noncanonical mode (ICANON unset).  By default, ICANON is set. In noncanonical mode input is available immediately (without the user having to type a line-delimiter character), no input processing is performed, and line editing is disabled.
    
    ///imposto il nuovo terminale
    tcsetattr(fd, TCSANOW, &custom);
    
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL, 0) | O_NONBLOCK);
}

/**
 * Inizializza i valori della struttura tetris
 * @param t la struttura da inizializzare
 * @param w la larghezza del campo di gioco
 * @param h l'alatezza del campo di gioco
 * @param mode la modalità di gioco (0 singleplayer, 1 multiplayer)
 */
void tetris_init(struct tetris *t, int w, int h,int mode)
{
    int x, y, j;
    t->score = 0;
    t->gameover = 0;
    t->w = w;
    t->h = h;
    t->game = malloc(sizeof(char *)*w);
    for (x=0; x<w; x++) {
        t->game[x] = malloc(sizeof(char)*h);
        for (y=0; y<h; y++)
            t->game[x][y] = ' ';
    }
    ///se il gioco è single player si avranno 20 tetramini per tipo, 40 altrimenti
    if(mode == SINGLE_PLAYER) mode = 1;
    else mode = 2;
    for(j=0;j<5;j++){
        num_current[j]=20 * mode;
    }
}

/**
 * Stampa il numero di blocchi per la struttura corrente
 * @param t la struttura per la quale vogliamo stampare il numero di blocchi resanti
 */
void print_blocks(struct tetris *t)
{
    printf("Opzioni:\n");;
    printf("0->🟩 [x%d] 1->🟨 [x%d] 2->🟧 [x%d] 3->🟥 [x%d] 4->🟪 [x%d]\n",num_current[0], num_current[1], num_current[2], num_current[3], num_current[4] );
}

/**
 * Libera la memoria
 * @param t la struttura da liberare
 */
void tetris_clean(struct tetris *t)
{
    int x;
    for (x=0; x<t->w; x++) {
        free(t->game[x]);
    }
    free(t->game);
}

/**
 * Stampa le opzioni dei tetramini
 * @param x indica la riga
 */
void print_option(int x){
    if(x==0)
        printf ("°⬛⬛⬛⬛⬛⬛°");
    if(x==1)
        printf ("°⬛🟩🟩⬛⬛⬛°");
    if(x==2)
        printf ("°⬛🟩🟩⬛⬛⬛°");
    if(x==3)
        printf ("°⬛⬛⬛⬛⬛⬛°");
    if(x==4)
        printf ("°⬛⬛🟨⬛⬛⬛°");
    if(x==5)
        printf ("°⬛🟨🟨🟨⬛⬛°");
    if(x==6)
        printf ("°⬛⬛⬛⬛⬛⬛°");
    if(x==7)
        printf ("°⬛🟧🟧🟧🟧⬛°");
    if(x==8)
        printf ("°⬛⬛⬛⬛⬛⬛°");
    if(x==9)
        printf ("°⬛🟥🟥⬛⬛⬛°");
    if(x==10)
        printf ("°⬛🟥⬛⬛⬛⬛°");
    if(x==11)
        printf ("°⬛🟥⬛⬛⬛⬛°");
    if(x==12)
        printf ("°⬛⬛⬛⬛⬛⬛°");
    if(x==13)
        printf ("°⬛🟪🟪⬛⬛⬛°");
    if(x==14)
        printf ("°⬛⬛🟪🟪⬛⬛°");
    if(x==15)
        printf ("°-------------°");
    
}

/**
 * Converte le informazioni relative ai tetrami al'interno della struttura in tetramino coolorati
 * @param x l'identificatore del tetramino
 */
void awesome_cube(char x){
    switch (x)
    {
        case ' ':
            printf("⬛");
            break;
        case '0':
            printf("🟩");
            break;
        case '1':
            printf("🟨");
            break;
        case '2':
            printf("🟧");
            break;
        case '3':
            printf("🟥");
            break;
        case '4':
            printf("🟪");
            break;
        case '5':
            printf("🟦");
            break;
        default:
            break;
    }
}

/**
 * Stampa il campo nella modalità single player
 * @param t la struttura che contiene il campo desiderato
 */
void tetris_print(struct tetris *t)
{
    int x,y;
    for (x=0; x<30; x++)
        printf("\n");
    printf("     SCORE: [%d]\n", t->score);
    printf("\n");
    for (y=0; y<t->h; y++) {
        printf("     °");
        for (x=0; x<t->w; x++) {
            if (x >= t->x && y >= t->y && x < (t->x + t->current.w) && y < (t->y + t->current.h) && t->current.data[y - t->y][x - t->x] != ' ')
            {
                char r = t->current.data[y - t->y][x - t->x];
                awesome_cube(r);
                
                // printf("%c ", t->current.data[y - t->y][x - t->x]);
            }
            else
            {
                awesome_cube(t->game[x][y]);
                // printf("%c ", t->game[x][y]);
                
            }
            
        }
        print_option(y);
        printf("\n");
        // printf ("°\n");
        
    }
    printf("     °");
    for (x=0; x<2*t->w+2; x++)
        printf("-");
    printf ("°-------------°\n");
    printf("\n");
}

/**
 * Stampa i campi nella modalià multiplayer
 * @param t1 la prima struttura contenente il campo interessato
 * @param t2 la seconda struttura contenente il campo interessato
 */
void tetris_print2(struct tetris *t1,struct tetris *t2)
{
    int x,y;
    for (x=0; x<30; x++)
        printf("\n");
    printf("     SCORE player1: [%d]                       SCORE player2: [%d]    \n", t1->score,t2->score);
    printf("\n");
    for (y=0; y<t1->h; y++) {
        printf("     °");
        for (x=0; x<t1->w; x++) {
            
            if (x >= t1->x && y >= t1->y && x < (t1->x + t1->current.w) && y < (t1->y + t1->current.h) && t1->current.data[y - t1->y][x - t1->x] != ' ')
            {
                char r = t1->current.data[y - t1->y][x - t1->x];
                // mettere uno switch
                awesome_cube(r);
                
                // printf("%c ", t->current.data[y - t->y][x - t->x]);
            }
            else
            {
                awesome_cube(t1->game[x][y]);
                // printf("%c ", t->game[x][y]);
                
            }
            
        }
        
        print_option(y);
        
        for (x=0; x<t2->w; x++) {
            
            if (x >= t2->x && y >= t2->y && x < (t2->x + t2->current.w) && y < (t2->y + t2->current.h) && t2->current.data[y - t2->y][x - t2->x] != ' ')
            {
                char r = t2->current.data[y - t2->y][x - t2->x];
                // mettere uno switch
                awesome_cube(r);
                
                // printf("%c ", t->current.data[y - t->y][x - t->x]);
            }
            else
            {
                awesome_cube(t2->game[x][y]);
                // printf("%c ", t->game[x][y]);
                
            }
            
        }
        
        
        
        printf("°\n");
        // printf ("°\n");
        
    }
    printf("     ");
    printf("°");
    for (x=0; x<2*t1->w+2; x++)
        printf("-");
    printf ("°-------------°");
    for (x=0; x<2*t1->w; x++)
        printf("-");
    printf("°\n");
}

/**
 * Controlla la collisione con un tetramino o con la base
 * @param t la struttura sulla quale fare il controllo
 * @return 0 non è stata rilevata collisione
 * @return 1 è stata rilevata una collisione
 */
int tetris_hittest(struct tetris *t)
{
    int x,y,X,Y;
    struct tetris_block b=t->current;
    for (x=0; x<b.w; x++)
        for (y=0; y<b.h; y++) {
            X=t->x+x;
            Y=t->y+y;
            if (X<0 || X>=t->w)
                return 1;
            if (b.data[y][x]!=' ') {
                if ((Y >= t->h) ||
                    (X >= 0 && X < t->w && Y >= 0 && t->game[X][Y] != ' '))
                {
                    return 1;
                }
            }
        }
    return 0;
}

/**
 * Crea un nuovo tetramino
 * @param t la struttura dove creare il tetramino
 * @param k il tipo di tetramino da creare
 */
void tetris_new_block(struct tetris *t, int k)
{
    t->current = blocks[k];
    t->x=(t->w/2) - (t->current.w/2);
    t->y=0;
    if (tetris_hittest(t)) {
        t->gameover=1;
    }
}


/**
 * Stampa i blocchi del tetris corrente
 * @param t il la struttura con i blocchi da stampare
 */
void tetris_print_block(struct tetris *t)
{
    int x,y,X,Y;
    struct tetris_block b=t->current;
    for (x=0; x<b.w; x++)
        for (y=0; y<b.h; y++) {
            if (b.data[y][x]!=' ')
                t->game[t->x+x][t->y+y]=b.data[y][x];
        }
}

/**
 * Ruota il tetramino
 * @param t la struttura contenente il tetramino da ruotare
 */
void tetris_rotate(struct tetris *t)
{
    struct tetris_block b=t->current;
    struct tetris_block s=b;
    int x,y;
    b.w=s.h;
    b.h=s.w;
    for (x=0; x<s.w; x++)
        for (y=0; y<s.h; y++) {
            b.data[x][y]=s.data[s.h-y-1][x];
        }
    x=t->x;
    y=t->y;
    t->x-=(b.w-s.w)/2;
    t->y-=(b.h-s.h)/2;
    t->current=b;
    if (tetris_hittest(t)) {
        t->current=s;
        t->x=x;
        t->y=y;
    }
}

/**
 * Fa scendere il tetramino
 * @param t la struttura di riferimento
 */

void tetris_gravity(struct tetris *t)
{
    int x, y, k = 0;
    t->y++;
    if (tetris_hittest(t)) {
        t->y--;
        tetris_print_block(t);
        
        print_blocks(t);
        tetris_cleanup_io();
    label4:
        printf("Scegli il tetramino:");
        scanf("%d", &k);
        if((k!=0 && k!=1 && k!=2 && k!=3 && k!=4) || num_current[k]==0){
            printf("!OPZIONE NON DISPONIBILE, ritenta.\n");
            goto label4;
        }
        tetris_set_ioconfig();
        
        
        num_current[k]--;
        
        
        tetris_new_block(t, k);
    }
}
int count_ply=0;
int count_pc=0;

/**
 * Fa scendere i tetramini nella modalità P vs CPU
 * @param t1 il primo giocatore
 * @param t2 a CPU
 */
void tetris_gravity_random(struct tetris *t1, struct tetris *t2 )
{
    int x, y, k = 0;
    
    
    
    t1->y++;
    t2->y++;
    if (tetris_hittest(t2) ) {
        count_pc++;
        
        t2->y--;
        tetris_print_block(t2);
        
        x=rand()%5;
        num_current[x]--;
        
        tetris_new_block(t2, x);
    }
    
    if (tetris_hittest(t1) ){
        count_ply++;
        
        t1->y--;
        tetris_print_block(t1);
        
        print_blocks(t1);
        
        tetris_cleanup_io();
    label3:
        printf("Scegli il tetramino:");
        scanf("%d", &k);
        if((k!=0 && k!=1 && k!=2 && k!=3 && k!=4) || num_current[k]==0){
            printf("!OPZIONE NON DISPONIBILE, ritenta.\n");
            goto label3;
        }
        tetris_set_ioconfig();
        num_current[k]--;
        
        
        
        tetris_new_block(t1, k);
        
        
    }
    
    
    
}

/**
 * Fa scendere i tetramini nella modalità P vs P
 * @param t1 il primo giocatore
 * @param t2 il secondo giocatore
 */
void tetris_gravity_random2(struct tetris *t1, struct tetris *t2 )
{
    int x, y, k = 0;
    
    
    
    t1->y++;
    t2->y++;
    if (tetris_hittest(t2) ) {
        count_pc++;
        
        t2->y--;
        tetris_print_block(t2);
        print_blocks(t2);
        tetris_cleanup_io();
    label6:
        printf("Scegli il tetramino (P2): ");
        scanf("%d", &x);
        if(x!=0 && x!=1 && x!=2 && x!=3 && x!=4 ){
            printf("!OPZIONE NON DISPONIBILE, ritenta.\n");
            goto label6;
        }
        tetris_set_ioconfig();
        
        num_current[x]--;
        
        tetris_new_block(t2, x);
    }
    
    if (tetris_hittest(t1) ){
        count_ply++;
        
        t1->y--;
        tetris_print_block(t1);
        
        print_blocks(t1);
        
        tetris_cleanup_io();
    label3:
        printf("Scegli il tetramino (P1): ");
        scanf("%d", &k);
        if(k!=0 && k!=1 && k!=2 && k!=3 && k!=4 ){
            printf("!OPZIONE NON DISPONIBILE, ritenta.\n");
            goto label3;
        }
        tetris_set_ioconfig();
        num_current[k]--;
        
        
        
        tetris_new_block(t1, k);
        
        
    }
    
    
    
}

/**
 * Permette di eliminare la o le righe che sono state completate
 * @param t la struttura su cui lavorare
 * @param l il numero di righe da eliminare
 */
void tetris_fall(struct tetris *t, int l){
    int x,y;
    for (y=l; y>0; y--) {
        for (x=0; x<t->w; x++)
            t->game[x][y]=t->game[x][y-1];
    }
    for (x=0; x<t->w; x++)
        t->game[x][0]=' ';
}

/**
 * Controlla se vengono eliminate le righe e assegnare i punti, nel caso se ne elimino 4, viene invertito il rispettivo campo avversario
 * @param t1 il primo campo
 * @param t2 il secondo campo
 */
void tetris_check_lines(struct tetris *t1, struct tetris *t2)
{
    int count=0;
    int x,y,l;
    int p=1;
    for (y=t1->h-1; y>=0; y--) {
        l=1;
        for (x=0; x<t1->w && l; x++) {
            if (t1->game[x][y]==' ') {
                l=0;
            }
        }
        if (l) {
        
            count++;
            tetris_fall(t1, y);
            y++;
        }
    }
    if (count==1)
        t1->score += p;
    if (count==2)
        t1->score += 3;
    if (count==3)
        t1->score += 6;
    if (count==4){
        t1->score += 12;
        int x,y;
        for (y=t2->h-4; y<t2->h; y++) {
            for (x=0; x<t2->w; x++){
                if(t2->game[x][y]==' ')
                    t2->game[x][y]='5';
                else
                    t2->game[x][y]=' ';
            }
        }
    }
}

/**
 * La funzione principale per il singleplayer, nella quale sono chiamate le funzione necessarie per far funzionare il gioco
 * @param w la larghezza del campo
 * @param h l'altezza del campo
 */
int tetris_run(int w, int h)
{
    struct timespec tm;
    
    struct tetris t;
    
    char cmd;
    int k;
    int count=0;
    tetris_init(&t, w, h,SINGLE_PLAYER);
    print_blocks(&t);
label2:
    printf("Scegli il tetramino:");
    
    scanf("%d", &k);
    if((k!=0 && k!=1 && k!=2 && k!=3 && k!=4) || num_current[k]==0 ){
        printf("\n OPZIONE NON DISPONIBILE, ritenta.\n");
        goto label2;
    }
    
    num_current[k]--;
    
    tetris_set_ioconfig();
    
    srand(time(NULL));
    
    tm.tv_sec=0;
    tm.tv_nsec=1000000;/// 1 millisec
    
    tetris_new_block(&t, k);
    
    while (!t.gameover) {
        
        nanosleep(&tm, NULL);
        count++;
        if (count%50 == 0) {
            tetris_print(&t);
        }
        if (count%350 == 0) {
            tetris_gravity(&t);
            AudioServicesPlayAlertSound(kSystemSoundID_UserPreferredAlert);
            
            tetris_check_lines(&t,NULL);
        }
        while ((cmd=getchar())>0) {
            
            switch (cmd) {
                case 'a':
                    t.x--;
                    if (tetris_hittest(&t))
                        t.x++;
                    break;
                case 'd':
                    t.x++;
                    if (tetris_hittest(&t))
                        t.x--;
                    break;
                case 's':
                    tetris_gravity(&t);
                    break;
                case ' ':
                    tetris_rotate(&t);
                    break;
            }
        }
        if(num_current[0]==0 && num_current[1]==0 && num_current[2]==0 && num_current[3]==0 && num_current[4]==0)
            t.gameover=1;
    }
    
    tetris_print(&t);
    
    
    printf("     °⬛⬛⬛⬛⬛⬛⬛⬛ °GAME-OVER PLAYER 1° ⬛⬛⬛⬛⬛⬛⬛⬛°\n");
    tetris_clean(&t);
    
    tetris_cleanup_io();
    
    return EXIT_SUCCESS;
}

/**
 * La funzione principale per la modalità P vs CPU, nella quale sono chiamate le funzioni necessarie per l'esecuzione del gioco
 * @param w la larghezza del campo
 * @param h l'altezza del campo
 */
int tetris_run_MP1(int w, int h)
{
    struct timespec tm;
    struct tetris t1;
    struct tetris t2;
    char cmd;
    int k;
    int count=0;
    tetris_init(&t1, w, h,MULTI_PLAYER);
    tetris_init(&t2, w, h, MULTI_PLAYER);
    print_blocks(&t1);
label1:
    printf("Scegli il tetramino:");
    
    scanf("%d", &k);
    if((k!=0 && k!=1 && k!=2 && k!=3 && k!=4) || num_current[k]==0){
        printf("!OPZIONE NON DISPONIBILE, ritenta.\n");
        goto label1;
    }
    num_current[k]--;
    
    tetris_set_ioconfig();
    
    
    
    
    srand(time(NULL));
    int s=rand()%5;
    num_current[s]--;
    
    tm.tv_sec=0;
    tm.tv_nsec=1000000;
    
    tetris_new_block(&t1, k);
    
    tetris_new_block(&t2, s);
    
    
    while (!t1.gameover ) {
        if(t2.gameover)
            break;
        nanosleep(&tm, NULL);
        count++;
        if (count%50 == 0) {
            tetris_print2(&t1,&t2);
        }
        if (count%350 == 0) {
           AudioServicesPlayAlertSound(kSystemSoundID_UserPreferredAlert);
            
            //tetris_gravity(&t1);
            tetris_gravity_random(&t1,&t2);
            
            
            
            tetris_check_lines(&t1,&t2);
            
            tetris_check_lines(&t2,&t1);
        }
        
        if(count%350==0){
            int random_num=rand()%4;
            printf("%d",random_num);
            switch (random_num) {
                case 1:
                    
                    t2.x--;
                    if (tetris_hittest(&t2))
                        t2.x++;
                    
                    break;
                case 2:
                    t2.x++;
                    if (tetris_hittest(&t2))
                        t2.x--;
                    break;
                    
                case 0:
                    tetris_rotate(&t2);
                    break;
                    
            }
        }
        
        
        while ((cmd=getchar())>0) {
            
            switch (cmd) {
                case 'a':
                    t1.x--;
                    if (tetris_hittest(&t1))
                        t1.x++;
                    
                    break;
                case 'd':
                    t1.x++;
                    if (tetris_hittest(&t1))
                        t1.x--;
                    break;
                case 's':
                    tetris_gravity(&t1);
                    break;
                case ' ':
                    tetris_rotate(&t1);
                    break;
            }
            
            
            
            
        }
        
        
        
    }
    tetris_print2(&t1,&t2);
    
    if(t1.gameover)
        printf("     °⬛⬛⬛⬛⬛⬛⬛⬛ °GAME-OVER PLAYER 1° ⬛⬛⬛⬛⬛⬛⬛⬛°\n");
    if(t2.gameover)
        printf("     °⬛⬛⬛⬛⬛⬛⬛⬛ °GAME-OVER PLAYER 2° ⬛⬛⬛⬛⬛⬛⬛⬛°\n");
    
    
    
    tetris_clean(&t1);
    tetris_clean(&t2);
    
    tetris_cleanup_io();
    
    return EXIT_SUCCESS;
}


/**
 * La funzione principale per la modalità P vs P,  nella quale sono chiamate le funzioni necessarie per l'esecuzione del gioco
 * @param w la larghezza del campo
 * @param h l'altezza del campo
 */
int tetris_run_MP2(int w, int h){
    struct timespec tm;
    struct tetris t1;
    struct tetris t2;
    char cmd;
    int k1, k2;
    int count=0;
    tetris_init(&t1, w, h, MULTI_PLAYER);
    tetris_init(&t2, w, h, MULTI_PLAYER);
    print_blocks(&t1);
label1:
    printf("Scegli il tetramino (P1):");
    
    scanf("%d", &k1);
    if(k1!=0 && k1!=1 && k1!=2 && k1!=3 && k1!=4 ){
        printf("!OPZIONE NON DISPONIBILE, ritenta.\n");
        goto label1;
    }
    num_current[k1]--;
    
    
label2:
    printf("Scegli il tetramino (P2):");
    
    scanf("%d", &k2);
    if(k2!=0 && k2!=1 && k2!=2 && k2!=3 && k2!=4 ){
        printf("!OPZIONE NON DISPONIBILE, ritenta.\n");
        goto label2;
    }
    num_current[k2]--;
    
    
    
    tetris_set_ioconfig();
    
    
    
    
    srand(time(NULL));
    tm.tv_sec=0;
    tm.tv_nsec=1000000;
    
    tetris_new_block(&t1, k1);
    
    tetris_new_block(&t2, k2);
    
    
    while (!t1.gameover ) {
        if(t2.gameover)
            break;
        nanosleep(&tm, NULL);
        count++;
        if (count%50 == 0) {
            tetris_print2(&t1,&t2);
        }
        if (count%350 == 0) {
            //tetris_gravity(&t1);
            tetris_gravity_random2(&t1,&t2);
            
            AudioServicesPlayAlertSound(kSystemSoundID_UserPreferredAlert);
            
            
            tetris_check_lines(&t1,&t2);
            
            tetris_check_lines(&t2,&t1);
        }
        
        
        
        
        while ((cmd=getchar())>0) {
            
            //          movimento P1
            
            switch (cmd) {
                    
                case 'a': // sinistra
                    t1.x--;
                    if (tetris_hittest(&t1)) t1.x++;
                    break;
                    
                case 'd': // destra
                    t1.x++;
                    if (tetris_hittest(&t1)) t1.x--;
                    break;
                    
                case 's': // giù
                    tetris_gravity(&t1);
                    break;
                    
                case 'w': // ruota
                    tetris_rotate(&t1);
                    break;
                    
                    //                movimento P2
                    
                case 'j': // sinistra
                    t2.x--;
                    if(tetris_hittest(&t2)) t2.x++;
                    break;
                    
                case 'l': // destra
                    t2.x++;
                    if(tetris_hittest(&t2)) t2.x--;
                    break;
                    
                case 'k': // giù
                    tetris_gravity(&t2);
                    break;
                    
                    
                case 'i': // ruota
                    tetris_rotate(&t2);
                    break;
                    
            }
            
        }
        
    }
    
    tetris_print2(&t1,&t2);
    if(t1.gameover)
        printf("     °⬛⬛⬛⬛⬛⬛⬛⬛ °GAME-OVER PLAYER 1° ⬛⬛⬛⬛⬛⬛⬛⬛°\n");
    if(t2.gameover)
        printf("     °⬛⬛⬛⬛⬛⬛⬛⬛ °GAME-OVER PLAYER 2° ⬛⬛⬛⬛⬛⬛⬛⬛°\n");
    tetris_clean(&t1);
    tetris_clean(&t2);
    
    tetris_cleanup_io();
    
    return EXIT_SUCCESS;
}



