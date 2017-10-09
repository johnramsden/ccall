#include "../include/ccall.h"

ccall_error ccall_init(){
    ccall_error ret = CCALL_SUCCESS;

    WINDOW *ncurses_success = NULL;
    ncurses_success = initscr();

    if(ncurses_success == NULL){
        ret = CCALL_FAILURE;
        fprintf(stderr, "Failed to initialize ncurses");
    } else {
        /* Don't echo printed characters to screen */
        noecho();
    }

    return ret;
}

ccall_error ccall_fini(){
    ccall_error ret = CCALL_SUCCESS;

    if(endwin() == ERR){
        ret = CCALL_FAILURE;
        fprintf(stderr, "Failed to end ncurses");
    }

    return ret;
}

ccall_error ccall_run(){
    printw("-- CCALL Help Phone --");
    while(getch() != 'q') {
        refresh();
    }
}

int main() {
    /* Initialize ncurses */
    ccall_error ret = CCALL_SUCCESS;
    ret = ccall_init();

    /* Make sure initialization and run are successful */
    if((ccall_init() != CCALL_SUCCESS) || (ccall_run() != CCALL_SUCCESS)){
        exit(EXIT_FAILURE);
    }


    /* Clean up ncurses */
    if(ccall_fini() != CCALL_SUCCESS){
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}