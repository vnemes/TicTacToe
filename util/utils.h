//
// Created by root on 3/3/19.
//

#ifndef TICTACTOE_UTILS_H
#define TICTACTOE_UTILS_H


#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

//#define DEBUG_MODE

#ifdef DEBUG_MODE
#define LOG(MSG) printf MSG
#else
#define LOG(MSG)
#endif

#define clear() printf("\033[H\033[J")
#define NET_BUF_SZ 256
#define EXPECT_COMMAND(BUF,COMM) (strstr((BUF),(COMM)) == (BUF))


void exit_with_error(const char *msg);

int safe_sock_read(int __fd, void *__buf, size_t __nbytes);
void safe_sock_write(int __fd, void *__buf, size_t __nbytes);

#endif //TICTACTOE_UTILS_H
