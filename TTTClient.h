//
// Created by root on 3/3/19.
//

#ifndef TICTACTOE_TTTCLIENT_H
#define TICTACTOE_TTTCLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "util/utils.h"

//http://www.linuxhowtos.org/C_C++/socket.htm

int client_connect(int portno, char *server_host);

#endif //TICTACTOE_TTTCLIENT_H
