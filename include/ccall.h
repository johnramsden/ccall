//
// Created by john on 10/8/17.
//

#ifndef CCALL_CCALL_H
#define CCALL_CCALL_H

#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <curses.h>

typedef enum ccall_error {
    CCALL_SUCCESS,
    CCALL_FAILURE
} ccall_error;

ccall_error ccall_init();
ccall_error ccall_fini();

#endif //CCALL_CCALL_H
