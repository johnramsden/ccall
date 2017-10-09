//
// Created by john on 10/8/17.
//

#ifndef CCALL_CCALL_H
#define CCALL_CCALL_H

#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <curses.h>
#include <signal.h>

#include "linphone/core.h"

typedef enum ccall_error {
    CCALL_SUCCESS,
    CCALL_FAILURE
} ccall_error;

typedef struct ccall_authentication {
    const char * 	username;
    const char * 	userid;
    const char * 	passwd;
    const char * 	ha1;
    const char * 	realm;
    const char * 	domain;
} ccall_authentication_t;

ccall_error make_call(WINDOW *ncurses_window, ccall_authentication_t *authentication, const char *dest_num);

#endif //CCALL_CCALL_H
