//
// Created by root on 3/3/19.
//

#include "TTTServer.h"
#include "../util/board.h"

player_st *wait_for_player_connection(int sockfd, socklen_t *clilen, player_st *player,
                                      struct sockaddr_in *cli_addr);
void notify_player_start(player_st *player, const char *opponent_name);
win_status_en handle_player_move(const player_st *player);
void notify_game_end(const player_st* player1, const player_st* player2, win_status_en win);

void server_listen(uint16_t portno) {
    int sockfd;
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;

    printf("Server started listening on port %d\n", portno);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        exit_with_error("ERROR opening socket");
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    if (bind(sockfd, (struct sockaddr *) &serv_addr,
             sizeof(serv_addr)) < 0)
        exit_with_error("ERROR on binding");
    listen(sockfd, 2);
    clilen = sizeof(cli_addr);

    player_st *player1 = malloc(sizeof(player_st)), *player2 = malloc(sizeof(player_st));
    player1->buffer = malloc(NET_BUF_SZ);
    player2->buffer = malloc(NET_BUF_SZ);
    player1->x_or_o = _X_;
    player2->x_or_o = _O_;
    player1 = wait_for_player_connection(sockfd, &clilen, player1, &cli_addr);
    if (player1) {
        player2 = wait_for_player_connection(sockfd, &clilen, player2, &cli_addr);
        if (player2) {
            init_board();
            notify_player_start(player1, player2->name);
            notify_player_start(player2, player1->name);


            while (1) {
                if (handle_player_move(player1) == ONGOING)
                    if (handle_player_move(player2) == ONGOING)
                        continue;
                notify_game_end(player1,player2,check_win_condition());
                close(sockfd);
                return;
            }
        }
    }
    close(sockfd);
}

player_st *wait_for_player_connection(int sockfd, socklen_t *clilen, player_st *player,
                                      struct sockaddr_in *cli_addr) {
    player->fd = accept(sockfd,
                        (struct sockaddr *) cli_addr,
                        clilen);
    if (player->fd < 0)
        exit_with_error("ERROR on accept");

    while (safe_sock_read(player->fd, player->buffer, NET_BUF_SZ - 1)) {

        if (EXPECT_COMMAND(player->buffer, CONNECT_REQUEST)) {
            player->name = malloc(NET_BUF_SZ);
            strncpy(player->name, player->buffer + strlen(CONNECT_REQUEST), NET_BUF_SZ - strlen(CONNECT_REQUEST));
            LOG(("DEBUG: Player connected: %s", player->name));
            safe_sock_write(player->fd, CONNECT_RESPONSE, strlen(CONNECT_RESPONSE));
            return player;
        } else {
            safe_sock_write(player->fd, "Expected CONNECT_REQUEST with player's name", 42);
        }

    }
    return NULL;

}

void notify_player_start(player_st *player, const char *opponent_name) {
    bzero(player->buffer, NET_BUF_SZ);
    sprintf(player->buffer,"%s%c%s",NOTIFY_GAME_START,player->x_or_o,opponent_name);
    safe_sock_write(player->fd, player->buffer, strlen(player->buffer));
}

win_status_en handle_player_move(const player_st *player) {
    bzero(player->buffer, NET_BUF_SZ);
    strcpy(player->buffer, NOTIFY_TURN_START);
    export_board_to_msg(player->buffer + strlen(NOTIFY_TURN_START));
    safe_sock_write(player->fd, player->buffer, strlen(player->buffer));

    bzero(player->buffer, NET_BUF_SZ);
    if (safe_sock_read(player->fd, player->buffer, NET_BUF_SZ - 1) && EXPECT_COMMAND(player->buffer, MOVE_REQUEST)) {
        uint8_t pos = (uint8_t) atoi(player->buffer + strlen(MOVE_REQUEST));

        if (update_board_with_move(pos, player->x_or_o)) {
            return check_win_condition();
        } else {

            bzero(player->buffer, NET_BUF_SZ);
            safe_sock_write(player->fd, MOVE_NOT_ALLOWED_RESPONSE, strlen(MOVE_NOT_ALLOWED_RESPONSE));
            return handle_player_move(player);
        }
    } else {
        exit_with_error("Client sent an invalid command");
    }
}

void notify_game_end(const player_st* player1, const player_st* player2, win_status_en win) {
    char* winner;
    switch (win) {
        case X_WINS:
            winner = player1->x_or_o == _X_ ? player1->name : player2->name;
            printf("X won the match!\n");

            break;
        case O_WINS:
            winner = player1->x_or_o == _O_ ? player1->name : player2->name;
            printf("O won the match!\n");
            break;
        case DRAW:
            winner = DRAW_REPR;
            printf("The game resulted in a draw!\n");
            break;
        default:
            exit_with_error("Invalid board status");
            break;
    }

    bzero(player1->buffer, NET_BUF_SZ);
    bzero(player2->buffer, NET_BUF_SZ);
    sprintf(player1->buffer, "%s%s", GAME_END_RESPONSE, winner);
    sprintf(player2->buffer, "%s%s", GAME_END_RESPONSE, winner);
    safe_sock_write(player1->fd, player1->buffer, strlen(player1->buffer));
    safe_sock_write(player2->fd, player2->buffer, strlen(player2->buffer));
};
