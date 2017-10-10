#include <ncurses.h>
#include "../include/ccall.h"

static bool_t running=TRUE;
static void stop(int signum){
    running=FALSE;
}

void ccall_usage(){
    fprintf(stdout, "CCALL Usage");
    fprintf(stdout, "ccall <sip uri>");
}

ccall_error make_call(WINDOW *ncurses_window, MEVENT *event, const char *config, const char *dest_num) {
    running=TRUE;
    ccall_error ret = CCALL_SUCCESS;
    int ch = 0;
    int event_count = 0;

    LinphoneFactory *factory = linphone_factory_get();
    LinphoneCoreCbs *cbs = linphone_factory_create_core_cbs(factory);
    LinphoneCore *lp_config = linphone_factory_create_core(factory, cbs, config, NULL);

    // Place an outgoing call
    LinphoneCall *call = linphone_core_invite(lp_config, dest_num);
    linphone_core_set_in_call_timeout(lp_config, 10);

    call = linphone_call_ref(call);
    if (call == NULL) {
        wmove(ncurses_window,30,0);
        wclrtoeol(ncurses_window);
        mvprintw(30, 5, "Could not place call to %s\n", dest_num);
        ret = CCALL_FAILURE;
    } else {
        wmove(ncurses_window,30,0);
        wclrtoeol(ncurses_window);
        mvprintw(30, 5, "Call to %s is in progress...", dest_num);

        linphone_call_ref(call);

        while(running){
            linphone_core_iterate(lp_config);
            ms_usleep(50000);

            // Watch for attempts to quit
//            if (((ch = getch()) == 'q') || (ch == KEY_MOUSE) ) {
//                if((getmouse(event) == OK) && (event->bstate == BUTTON3_CLICKED)) {
//                    wmove(ncurses_window, 20, 0);
//                    wclrtoeol(ncurses_window);
//                    mvprintw(20, 3, "Hanging up...");
//                    running = FALSE;
//                }
//            }

            if (linphone_call_get_state(call) == LinphoneCallEnd){
                running = FALSE;
                clear();
            }

            mvprintw(30, 5, "Running... %i", event_count++);
            refresh();
        }
        if (linphone_call_get_state(call) != LinphoneCallEnd){

            wmove(ncurses_window,30,0);
            wclrtoeol(ncurses_window);
            mvprintw(30, 5, "Terminating the call...\n");

            linphone_core_terminate_call(lp_config,call);
            linphone_call_unref(call);
        }

        linphone_core_unref(lp_config);
    }
    clear();
    return ret;
}

int main(int argc, char **argv) {
    const char *dest_num=NULL;
    const char *config = "/home/john/.linphonerc";

    // Get destination sip uri
    if ((argc > 1) && (argc <= 2)){
        if(strcmp(argv[1], "-h") == 0){
            ccall_usage();
            exit(EXIT_SUCCESS);
        }
        dest_num=argv[1];
    } else {
        ccall_usage();
        exit(EXIT_FAILURE);
    }

    signal(SIGINT,stop);

    int ch, count=0;
    MEVENT event;
    WINDOW *ncurses_window = NULL;


    // Initialize ncurses
    if((ncurses_window = initscr()) == NULL){
        fprintf(stderr, "Error initializing ncurses");
        exit(EXIT_FAILURE);
    }

    // Set ncurses configuration
    raw();
    keypad(ncurses_window, TRUE);
    noecho();
    clear();
    cbreak();
    mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);


    // Print top info
    mvprintw(1, 15, "CCALL Help Phone");
    mvprintw(5, 1, "Usage:");
    mvprintw(7, 1, "Phone - Left click:");
    mvprintw(8, 1, "Hang up - Right click:");


    // Begin main section, wait for mouse press
    while ((ch = getch()) != 'q') {
        count++;
        mvprintw(10, 1, "Has mouse: %d", has_mouse());
        mvprintw(12, 1, "Key code: %x; mouse code:%x", ch, KEY_MOUSE);


        // Mouse button pressed
        if ((ch == KEY_MOUSE) && (getmouse(&event)) == OK) {
                mvprintw(13, 3, "Mouse Event: x=%d, y=%d z=%d",
                         event.x, event.y, event.z);
                mvprintw(14, 3, "Mouse device id: %x", event.id);
                mvprintw(15, 3, "Mouse button mask: %x", event.bstate);

                if(event.bstate == BUTTON1_CLICKED){
                    wmove(ncurses_window,20,0);
                    wclrtoeol(ncurses_window);
                    make_call(ncurses_window, &event, config, dest_num);

                    mvprintw(1, 15, "CCALL Help Phone");
                    mvprintw(5, 1, "Usage:");
                    mvprintw(7, 1, "Phone - Left click:");
                    mvprintw(8, 1, "Hang up - Right click:");

                } else if(event.bstate == BUTTON3_CLICKED){
                    wmove(ncurses_window,20,0);
                    wclrtoeol(ncurses_window);
                    mvprintw(20, 3, "Hanging up...");
                }
            ch = 0;
        }
        mvprintw(16, 1, "Event number: %4d",count);
        refresh();
    }
    endwin();

}