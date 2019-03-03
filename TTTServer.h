//
// Created by root on 3/3/19.
//

#ifndef TICTACTOE_TTTSERVER_H
#define TICTACTOE_TTTSERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "util/utils.h"

int server_listen(int portno);

#endif //TICTACTOE_TTTSERVER_H
