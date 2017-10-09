#include <ncurses.h>
#include "../include/ccall.h"


int main() {
    int ch, count=0;
    MEVENT event;

    /* Initialize ncurses */
    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();

    clear();
    cbreak();

    mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);

    while ((ch = getch()) != 'q') {
        count++;
        mvprintw(1, 1, "Has mouse: %d", has_mouse());
        mvprintw(2, 1, "Key code: %x; mouse code:%x", ch, KEY_MOUSE);
        if (ch == KEY_MOUSE) {
            if (getmouse(&event) == OK) {
                mvprintw(3, 3, "Mouse Event: x=%d, y=%d z=%d",
                         event.x, event.y, event.z);
                mvprintw(4, 3, "Mouse device id: %x", event.id);
                mvprintw(5, 3, "Mouse button mask: %x", event.bstate);

                if(event.bstate == BUTTON1_CLICKED){
                    mvprintw(10, 3, "Left clicked");
                } else if(event.bstate == BUTTON3_CLICKED){
                    mvprintw(10, 3, "Right clicked");
                }
            }
        }
        mvprintw(6, 1, "Event number: %4d",count);
        refresh();
    }
    endwin();
}