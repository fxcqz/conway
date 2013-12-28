#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>
#include <unistd.h>

#define GRIDX 10
#define GRIDY 10

int *makegrid(int width, int height){
    int *ret = (int*) malloc(sizeof(int)*(width+2)*(height+2));
    int i;
    for(i = 0; i < (width*height); i++){
        ret[i] = 0;
    }
    return ret;
}

void addlife(int *grid, int width, int height){
    int c = 0, v;
    // initial number of life cells given by
    // width * height / 4 => for 10x10 = 25 living
    while(c < (int)((width*height)/4)){
        v = width+(rand()%(width*height));
        if(grid[v] != 1 && v < (width*height)){
            // give life
            grid[v] = 1;
            c++;
        }
    }
}

int ingrid(int *grid, int loc){
    /* check that cell isnt in buffer row or col */
    if((loc%(GRIDX-1) != 0 || loc%GRIDX != 0)
    && loc > GRIDX && loc <= ((GRIDX*GRIDY)+GRIDX))
        return 1;
    return 0;
}

int updatecell(int *grid, int x, int y){
    int count = 0;
    /* check adjacent cells, ugly but lazy way */
    if(ingrid(grid, ((GRIDX*y+x)-GRIDX)-1)          /* top left */
        && grid[((GRIDX*y+x)-GRIDX)-1]) count++;
    if(ingrid(grid, ((GRIDX*y+x)-GRIDX))            /* top */
        && grid[((GRIDX*y+x)-GRIDX)]) count++;
    if(ingrid(grid, ((GRIDX*y+x)-GRIDX)+1)          /* top right */
        && grid[((GRIDX*y+x)-GRIDX)+1]) count++;
    if(ingrid(grid, (GRIDX*y+x)-1)                  /* left */
        && grid[(GRIDX*y+x)-1]) count++;
    if(ingrid(grid, (GRIDX*y+x)+1)                  /* right */
        && grid[(GRIDX*y+x)+1]) count++;
    if(ingrid(grid, ((GRIDX*y+x)+GRIDX)-1)          /* bot left */
        && grid[((GRIDX*y+x)+GRIDX)-1]) count++;
    if(ingrid(grid, ((GRIDX*y+x)+GRIDX))            /* bot */
        && grid[((GRIDX*y+x)+GRIDX)]) count++;
    if(ingrid(grid, ((GRIDX*y+x)+GRIDX)+1)          /* bot right */
        && grid[((GRIDX*y+x)+GRIDX)+1]) count++;
    //
    if(grid[GRIDX*y+x]){
        // will it die?
        if(count < 2 || count > 3)
            grid[GRIDX*y+x] = 0;
    } else {
        // will it come alive?
        if(count == 3)
            grid[GRIDX*y+x] = 1;
    }
}

void nextgen(int *grid, int width, int height){
    int i, j;
    for(j = 1; j < height+1; j++){
        for(i = 1; i < width+1; i++){
            updatecell(grid, i, j);
        }
    }
}

void printgrid(int *grid, int width, int height){
    int i, j;
    printw(" + ");
    for(i = 0; i < width; i++){
        printw("= ");
    }
    printw("+\n");
    start_color();
    init_pair(1, COLOR_MAGENTA, COLOR_BLACK);
    for(j = 1; j < height+1; j++){
        printw("|| ");
        for(i = 1; i < width+1; i++){
            if(grid[width*j+i]) attron(COLOR_PAIR(1));
            printw("%c ", grid[width*j+i]?43:32);
            if(grid[width*j+i]) attroff(COLOR_PAIR(1));
        }
        printw("||\n");
    }
    printw(" + ");
    for(i = 0; i < width; i++){
        printw("= ");
    }
    printw("+\n");
}

int main(){
    srand(time(NULL));
    int *grid = makegrid(GRIDX, GRIDY);
    addlife(grid, GRIDX, GRIDY);
    initscr();
    clear();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    curs_set(0);
    printgrid(grid, GRIDX, GRIDY);
    while(1){
        nextgen(grid, GRIDX, GRIDY);
        clear();
        printgrid(grid, GRIDX, GRIDY);
        refresh();
        // no tearing
        usleep(500000);
    }
    getch();
    endwin();
    return 0;
}
