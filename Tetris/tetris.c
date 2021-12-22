#include <stdio.h>
#include <stdlib.h>
#include "tetris.h"
#include <termios.h>
#include <signal.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <windows.h>

struct tetris_level {
    int score;
    int nsec;
};

struct tetris
{
    char **game;
    int w;
    int h;
    int level;
    int gameover;
    int score;
    struct tetris_block
    {
        char data[5][5];
        int w;
        int h;
        int count;
    } current;
    int x;
    int y;
};

struct tetris_block blocks[] =
    {
        {{"00", "00"}, 2, 2, 20},
        {{" 1 ", "111"}, 3, 2, 20},
        {{"2222"}, 4, 1, 20},
        {{"33", "3 ", "3 "}, 2, 3, 20},
        {{"44 ", " 44"}, 3, 2, 20}};

#define TETRIS_PIECES (sizeof(blocks) / sizeof(struct tetris_block))

struct termios save;

void print_blocks()
{
    printf("Opzioni:\n");
    printf("0->🟩 1->🟨 2->🟧 3->🟥 4->🟪 \n");
}

void tetris_cleanup_io()
{
    tcsetattr(fileno(stdin), TCSANOW, &save);
    sleep(4);
}

void tetris_signal_quit(int s)
{
    tetris_cleanup_io();
}

void tetris_set_ioconfig()
{
    struct termios custom;
    int fd = fileno(stdin);
    tcgetattr(fd, &save);
    custom = save;
    custom.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(fd, TCSANOW, &custom);
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL, 0) | O_NONBLOCK);
}

void tetris_init(struct tetris *t, int w, int h)
{
    int x, y;
    t->level = 1;
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
}

void tetris_clean(struct tetris *t)
{
    int x;
    for (x=0; x<t->w; x++) {
        free(t->game[x]);
    }
    free(t->game);
}

void print_option(int x)
{
    if (x == 0)
        printf("°⬛⬛⬛⬛⬛⬛\n");
    if (x == 1)
        printf("°⬛🟩🟩⬛⬛⬛\n");
    if (x == 2)
        printf("°⬛🟩🟩⬛⬛x%d\n", blocks[0].count);
    if (x == 3)
        printf("°⬛⬛⬛⬛⬛⬛\n");
    if (x == 4)
        printf("°⬛⬛🟨⬛⬛⬛\n");
    if (x == 5)
        printf("°⬛🟨🟨🟨⬛x%d\n", blocks[1].count);
    if (x == 6)
        printf("°⬛⬛⬛⬛⬛⬛\n");
    if (x == 7)
        printf("°⬛🟧🟧🟧🟧x%d\n", blocks[2].count);
    if (x == 8)
        printf("°⬛⬛⬛⬛⬛⬛\n");
    if (x == 9)
        printf("°⬛🟥🟥⬛⬛⬛\n");
    if (x == 10)
        printf("°⬛🟥⬛⬛⬛x%d\n", blocks[3].count);
    if (x == 11)
        printf("°⬛🟥⬛⬛⬛⬛\n");
    if (x == 12)
        printf("°⬛⬛⬛⬛⬛⬛\n");
    if (x == 13)
        printf("°⬛🟪🟪⬛⬛⬛\n");
    if (x == 14)
        printf("°⬛⬛🟪🟪⬛x%d\n", blocks[4].count);
    if (x == 15)
        printf("°\n");
}
void awesome_cube(char x)
{
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
    default:
        break;
    }
}

void tetris_print(struct tetris *t)
{
    int x, y;
    for (x = 0; x < 30; x++)
        printf("\n");
    printf("     SCORE: [%d]\n", t->score);
    printf("\n");
    for (y = 0; y < t->h; y++)
    {
        printf("     °");
        for (x = 0; x < t->w; x++)
        {
            if (x >= t->x && y >= t->y && x < (t->x + t->current.w) && y < (t->y + t->current.h) && t->current.data[y - t->y][x - t->x] != ' ')
            {
                char r = t->current.data[y - t->y][x - t->x];
                // mettere uno switch
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
        // printf ("°\n");
    }
    printf("     ");
    for (x=0; x<2*t->w+2; x++)
        printf("-");

    printf("\n");
}

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

void tetris_new_block(struct tetris *t, int k)
{
    t->current = blocks[k];
    t->x=(t->w/2) - (t->current.w/2);
    t->y=0;
    if (tetris_hittest(t)) {
        t->gameover=1;
    }
}

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

void tetris_rotate(struct tetris *t)
{
    struct tetris_block b = t->current;
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

void tetris_gravity(struct tetris *t)
{

    int x, y, k = 0;
    t->y++;
    if (tetris_hittest(t)) {
        t->y--;
        tetris_print_block(t);
        print_blocks();
        printf("Scegli il tetramino:");
        sleep(3);
        scanf("%d", &k);
        blocks[k].count--;
        tetris_new_block(t, k);
    }
}

void tetris_fall(struct tetris *t, int l)
{
    int x,y;
    for (y=l; y>0; y--) {
        for (x=0; x<t->w; x++)
            t->game[x][y]=t->game[x][y-1];
    }
    for (x=0; x<t->w; x++)
        t->game[x][0]=' ';
}

void tetris_check_lines(struct tetris *t)
{
    int count = 0;
    int x,y,l;
    int p = 1;
    for (y=t->h-1; y>=0; y--) {
        l=1;
        for (x=0; x<t->w && l; x++) {
            if (t->game[x][y]==' ') {
                l=0;
            }
        }
        if (l) {

            count++;
            tetris_fall(t, y);
            y++;
        }
    }
    if (count == 1)
        t->score += p;
    if (count == 2)
        t->score += 3;
    if (count == 3)
        t->score += 6;
    if (count == 4)
        t->score += 12;
}

int tetris_run(int w, int h)
{
    struct timespec tm;
    struct tetris t;
    char cmd;
    int k;
    int count=0;

    print_blocks();

    printf("Scegli il tetramino:");

    scanf("%d", &k);
    blocks[k].count--;
    tetris_set_ioconfig();
    tetris_init(&t, w, h);
    srand(time(NULL));

    tm.tv_sec=0;
    tm.tv_nsec=1000000;
    tetris_new_block(&t, k);

    while (!t.gameover) {
        nanosleep(&tm, NULL);
        count++;
        if (count%50 == 0) {
            tetris_print(&t);
        }
        if (count%350 == 0) {
            tetris_gravity(&t);
            tetris_check_lines(&t);
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
    }

    tetris_print(&t);
    printf("*** GAME OVER ***\n");

    tetris_clean(&t);

    tetris_cleanup_io();

    return EXIT_SUCCESS;
}
