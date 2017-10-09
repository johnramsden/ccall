#include "../include/ccall.h"

ccall_error
ccall_init(WINDOW *ncurses_window){
    ccall_error ret = CCALL_SUCCESS;
    ncurses_window = initscr();

    if(ncurses_window == NULL){
        ret = CCALL_FAILURE;
        fprintf(stderr, "Failed to initialize ncurses");
    } else {
        /* Don't echo printed characters to screen */
        noecho();

        /* Get all the mouse events */
        mousemask(ALL_MOUSE_EVENTS, NULL);
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

ccall_error ccall_run(WINDOW *ncurses_window){
    int c;
    int count =0;
    MEVENT event;

    raw();
    clear();
    cbreak();	//Line buffering disabled. pass on everything

    printw("-- CCALL Help Phone --");
    while((c = getch()) != 'q') {
        count++;
        mvprintw(1, 50, "Has mouse: %d", has_mouse());
        mvprintw(10, 50, "Event: %i", count);

        if (c == KEY_MOUSE) {
                if (getmouse(&event) == OK) {    /* When the user clicks left mouse button */
                        mvprintw(100, 1, "%s, %s", event.x + 1, event.y + 1);
                }
        }
        refresh();
    }
}

int main() {
    /* Initialize ncurses */
    ccall_error ret = CCALL_SUCCESS;
    WINDOW *ncurses_window = NULL;

    /* Make sure initialization and run are successful */
    if((ccall_init(ncurses_window) != CCALL_SUCCESS) || (ccall_run(ncurses_window) != CCALL_SUCCESS)){
        ccall_fini();
        exit(EXIT_FAILURE);
    }


    /* Clean up ncurses */
    if(ccall_fini() != CCALL_SUCCESS){
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}