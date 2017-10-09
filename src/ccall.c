#include <ncurses.h>
#include "../include/ccall.h"

static bool_t running=TRUE;
static void stop(int signum){
    running=FALSE;
}
/*
 * Call state notification callback
 */
static void call_state_changed(LinphoneCore *lc, LinphoneCall *call, LinphoneCallState cstate, const char *msg){
    switch(cstate){
        case LinphoneCallOutgoingRinging:
            printf("It is now ringing remotely !\n");
            break;
        case LinphoneCallOutgoingEarlyMedia:
            printf("Receiving some early media\n");
            break;
        case LinphoneCallConnected:
            printf("We are connected !\n");
            break;
        case LinphoneCallStreamsRunning:
            printf("Media streams established !\n");
            break;
        case LinphoneCallEnd:
            printf("Call is terminated.\n");
            break;
        case LinphoneCallError:
            printf("Call failure !");
            break;
        default:
            printf("Unhandled notification %i\n",cstate);
    }
}

void ccall_usage(){
    fprintf(stdout, "CCALL Usage");
    fprintf(stdout, "ccall <sip uri>");
}



ccall_error make_call(WINDOW *ncurses_window, ccall_authentication_t *authentication, const char *dest_num) {
    ccall_error ret = CCALL_SUCCESS;
    LinphoneCoreVTable vtable={0};



    //mvprintw(20, 3, "Calling %s with %s@%s", dest_num, authentication->username, authentication->domain);
    printf("Calling %s with %s@%s", dest_num, authentication->username, authentication->domain);

    /* Make call */

    /* Setup Linphone
     * https://www.linphone.org/docs/liblinphone/group__initializing.html */
    LinphoneFactory *factory = linphone_factory_get();
    //LinphoneCoreCbs *cbs = linphone_factory_create_core_cbs(factory);
    // LinphoneConfig *config;

    /* Create a linphone config:
     * https://www.linphone.org/docs/liblinphone/group__initializing.html#gacf77ea8002867d2dd93f197eda80f22b
     * Alternatively give linphone config path and factory to store settings */
    LinphoneCore *lp_config = linphone_factory_create_core(factory, NULL, NULL, NULL);

    /* Setup authentication */
    LinphoneAuthInfo *lp_auth_info = linphone_auth_info_new(authentication->username,
                                                            authentication->userid,
                                                            authentication->passwd,
                                                            authentication->ha1,
                                                            authentication->realm,
                                                            authentication->domain);
    linphone_core_add_auth_info(lp_config, lp_auth_info);
    //linphone_core_new(&vtable,NULL,NULL,NULL);
    /*
     Place an outgoing call
    */
    LinphoneCall *call = linphone_core_invite(lp_config, dest_num);
    call = linphone_call_ref(call);
    if (call == NULL) {
//        wmove(ncurses_window,30,0);
//        wclrtoeol(ncurses_window);
        //mvprintw(30, 5, "Could not place call to %s\n", dest_num);
        printf("Could not place call to %s\n", dest_num);
        ret = CCALL_FAILURE;
    } else {
//        wmove(ncurses_window,30,0);
//        wclrtoeol(ncurses_window);
        //mvprintw(30, 5, "Call to %s is in progress...", dest_num);
        printf("Call to %s is in progress...", dest_num);
        linphone_call_ref(call);

        /* main loop for receiving notifications and doing background linphonecore work: */
        while(running){
            linphone_core_iterate(lp_config);
            ms_usleep(50000);
        }
        if (linphone_call_get_state(call) != LinphoneCallEnd){
            /* terminate the call */
//            wmove(ncurses_window,30,0);
//            wclrtoeol(ncurses_window);
            //mvprintw(30, 5, "Terminating the call...\n");
            printf("Terminating the call...\n");
            linphone_core_terminate_call(lp_config,call);
            /*at this stage we don't need the call object */
            linphone_call_unref(call);
        }
//        wmove(ncurses_window,30,0);
//        wclrtoeol(ncurses_window);
        printf("Shutting down...\n");
        linphone_core_unref(lp_config);
    }

    return ret;
}

int main(int argc, char **argv) {
    ccall_authentication_t authentication = {"17788055363", "17788055363", "<pass>", NULL,
                                             "voip.freephoneline.ca", "voip.freephoneline.ca"};
    /* Handle input */
    const char *dest_num=NULL;
    /* Get destination sip uri */
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

    make_call(NULL, &authentication, dest_num);

/*    int ch, count=0;
    MEVENT event;
    WINDOW *ncurses_window = NULL;

    *//* Initialize ncurses *//*
    if((ncurses_window = initscr()) == NULL){
        fprintf(stderr, "Error initializing ncurses");
        exit(EXIT_FAILURE);
    }

    *//* Set ncurses configuration *//*
    raw();
    keypad(ncurses_window, TRUE);
    noecho();
    clear();
    cbreak();
    mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);

    *//* Print top info *//*
    mvprintw(1, 15, "CCALL Help Phone");
    mvprintw(5, 1, "Usage:");
    mvprintw(7, 1, "Phone - Left click:");
    mvprintw(8, 1, "Hang up - Right click:");

    *//* Begin main section, wait for mouse press *//*
    while ((ch = getch()) != 'q') {
        count++;
        mvprintw(10, 1, "Has mouse: %d", has_mouse());
        mvprintw(12, 1, "Key code: %x; mouse code:%x", ch, KEY_MOUSE);

        *//* Mouse button pressed *//*
        if ((ch == KEY_MOUSE) && (getmouse(&event)) == OK) {
                mvprintw(13, 3, "Mouse Event: x=%d, y=%d z=%d",
                         event.x, event.y, event.z);
                mvprintw(14, 3, "Mouse device id: %x", event.id);
                mvprintw(15, 3, "Mouse button mask: %x", event.bstate);

                if(event.bstate == BUTTON1_CLICKED){
                    wmove(ncurses_window,20,0);
                    wclrtoeol(ncurses_window);
                    make_call(ncurses_window, &authentication, dest_num);
                } else if(event.bstate == BUTTON3_CLICKED){
                    wmove(ncurses_window,20,0);
                    wclrtoeol(ncurses_window);
                    mvprintw(20, 3, "Hanging up...");
                }
        }
        mvprintw(16, 1, "Event number: %4d",count);
        refresh();
    }
    endwin();*/
}