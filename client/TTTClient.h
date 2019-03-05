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
#include "../util/utils.h"
#include "../socket_messages.h"

//http://www.linuxhowtos.org/C_C++/socket.htm

void start_client();

static void client_connect(char* player_name, int portno, char *server_host);

#endif //TICTACTOE_TTTCLIENT_H
