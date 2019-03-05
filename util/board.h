//
// Created by root on 3/4/19.
//

#ifndef TICTACTOE_BOARD_H
#define TICTACTOE_BOARD_H

#define _X_ 'X'
#define _O_ 'O'
#define _empty_ ' '


typedef enum _win_status {
    X_WINS,
    O_WINS,
    DRAW,
    ONGOING
} win_status_en;

char board[3][3];

void init_board();
void print_board();
void extract_board_from_msg(const char* msg);
void export_board_to_msg(char* msg);
win_status_en check_win_condition();
int is_move_allowed(uint8_t pos);
int update_board_with_move(uint8_t pos, char c);

static int checkWinForSymbol(char c);

#endif //TICTACTOE_BOARD_H
