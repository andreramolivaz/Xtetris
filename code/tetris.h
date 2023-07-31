/**
 * @file tetris.h
 */

#ifndef TETRIS_H
# define TETRIS_H

/**
 * La struttura del tetris
 */
struct tetris;

/**
 * Ripristina il terminale alle impostazioni di default
 */
void tetris_cleanup_io(void);

/**
 * imposta il terminale in modalità RAW
 */
void tetris_set_ioconfig(void);

/**
 * inizializza i valori della struttura tetris
 * @param t la struttura da inizialiizare
 * @param w la larghezza del campo
 * @param h l'altezza del campo
 * @param    mode la modalità di gioco
 */
void tetris_init(struct tetris *t,int w,int h,int mode);

/**
 * Libera la memoria della struttura
 * @param t la struttura da liberare
 */
void tetris_clean(struct tetris *t);

/**
 * Stampa il campo nella modalità singleplayer
 * @param t la struttura contenente il campo ineressato
 */
void tetris_print(struct tetris *t);

/**
 * Stampa il campo nella modalità multiplayer
 * @param t1 la struttura contenente il primo campo
 * @param t2 la struttura contenente il secondo campo
 */
void tetris_print2(struct tetris *t1,struct tetris *t2);

/**
 * Fa partire il gioco
 * @param width la larghezza del campo
 * @param height l'altezza del campo
 */
int tetris_run(int width, int height);

/**
 * Crea un novo tetramino
 * @param t la struttura dove creare il tetramino
 * @param k l'identificarore del tetramino
 */
void tetris_new_block(struct tetris *t,int k);

/**
 * Stampa il blocco identificato dal flag current
 * @param t la struttura su cui operare
 */
void tetris_print_block(struct tetris *t);

/**
 * Ruota il tetramino identificato dal flag current
 * @param t la struttura su cui operare
 */
void tetris_rotate(struct tetris *t);

/**
 * Fa scendere il tetramino
 * @param t la struttura su cui operare
 */
void tetris_gravity(struct tetris *t);

/**
 * Permette di eliminare la o le righe che sono state completate
 * @param t il campo su cui lavorare
 * @param l il numero di righe da eliminare
 */
void tetris_fall(struct tetris *t, int l);


/**
 Controlla se vengono eliminate le righe e assegnare i punti, nel caso se ne elimino 4, viene invertito il rispettivo campo avversario
 @param t1 il primo campo
 @param t2 il secondo campo
*/
void tetris_check_lines(struct tetris *t1, struct tetris *t2);

/**
 * La funzione principale per la modalità P vs CPU, nella quale sono chiamate le funzioni necessarie per l'esecuzione del gioco
 * @param w la larghezza del campo
 * @param h l'altezza del campo
*/
int tetris_run_MP1(int w, int h);

/**
 * La funzione principale per la modalità P vs P,  nella quale sono chiamate le funzioni necessarie per l'esecuzione del gioco
 * @param w la larghezza del campo
 * @param h l'altezza del campo
 */
int tetris_run_MP2(int w, int h);

#endif //TETRIS_H
