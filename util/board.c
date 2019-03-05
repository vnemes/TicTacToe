//
// Created by root on 3/4/19.
//

#include <stdint.h>
#include <stdio.h>
#include "board.h"
#include "utils.h"
#include <string.h>


void init_board() {
    for (uint8_t i = 0; i < 3; i++)
        for (uint8_t j = 0; j < 3; j++)
            board[i][j] = ' ';
};

void print_board() {
    putc('\n',stdout);
    for (uint8_t i = 0; i < 3; i++) {
        putc('\t',stdout);
        for (uint8_t j = 0; j < 3; j++) {
            putc(board[i][j], stdout);
            if (j < 2)
                putc('|', stdout);
        }
        printf("\t\t%d|%d|%d\n",i*3, i*3+1, i*3+2);
    }
    putc('\n',stdout);
};

void extract_board_from_msg(const char* msg){
    if (strlen(msg) < 9)
        exit_with_error("Invalid board format in message");

    for (uint8_t i = 0; i < 3; i++)
        for (uint8_t j = 0; j < 3; j++)
            board[i][j] = msg[i*3 + j];
}

void export_board_to_msg(char* msg){
    for (uint8_t i = 0; i < 3; i++)
        for (uint8_t j = 0; j < 3; j++)
            msg[i*3 + j] = board[i][j];
}

win_status_en check_win_condition() {
    if (checkWinForSymbol(_X_)) return X_WINS;
    if (checkWinForSymbol(_O_)) return O_WINS;
    for (uint8_t i = 0; i < 3; i++)
        for (uint8_t j = 0; j < 3; j++)
            if (board[i][j] == _empty_)
                return ONGOING;
    return DRAW;
}

int is_move_allowed(uint8_t pos) {
    return board[pos / 3][pos % 3] == _empty_;
}

int update_board_with_move(uint8_t pos, char c) {
    if (is_move_allowed(pos)) {
        board[pos / 3][pos % 3] = c;
        return 1;
    } else return 0;

}

int checkWinForSymbol(char c) {
    for (uint8_t i = 0; i < 3; i++) {
        if ((board[i][0] == c && board[i][1] == c && board[i][2] == c) ||
            (board[0][i] == c && board[1][i] == c && board[2][i] == c))
            return 1;

    }
    if ((board[0][0] == c && board[1][1] == c && board[2][2] == c) ||
        (board[0][2] == c && board[1][1] == c && board[2][0] == c))
        return 1;
    return 0;
}


