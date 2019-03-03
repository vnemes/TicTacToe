//
// Created by root on 3/3/19.
//


#include "utils.h"

void exit_with_error(const char *msg) {
    perror(msg);
    exit(0);
}
