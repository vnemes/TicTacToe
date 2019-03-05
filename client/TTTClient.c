//
// Created by root on 3/3/19.
//

#include "TTTClient.h"
#include "../util/board.h"


void prompt_new_move(int sockfd, char *buffer, char symbol, const char *opponent_name);

void start_client() {
    char *player_name = malloc(NET_BUF_SZ);
    printf("Please enter the player's name: ");
    fgets(player_name, NET_BUF_SZ - 1, stdin);
    player_name[strcspn(player_name, "\n")] = 0;

//    printf("Please enter the host and port of the server: ");
    client_connect(player_name, 2020, "localhost");
}

void client_connect(char *player_name, int portno, char *server_host) {
    int sockfd;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char *buffer = malloc(NET_BUF_SZ);


    printf("Client attempting connection to host %s:%d\n", server_host, portno);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        exit_with_error("ERROR opening socket");
    server = gethostbyname(server_host);
    if (server == NULL) {
        fprintf(stderr, "ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy(server->h_addr,
          (char *) &serv_addr.sin_addr.s_addr,
          server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        exit_with_error("ERROR connecting");
    bzero(buffer,NET_BUF_SZ);
    sprintf(buffer,"%s%s",CONNECT_REQUEST,player_name);
    safe_sock_write(sockfd, buffer, strlen(buffer));
    bzero(buffer, NET_BUF_SZ);
    if (safe_sock_read(sockfd, buffer, strlen(CONNECT_RESPONSE))) {
        if (EXPECT_COMMAND(buffer, CONNECT_RESPONSE)) {
            printf("Waiting for your opponent to join...\n");
            bzero(buffer, NET_BUF_SZ);
            if (safe_sock_read(sockfd, buffer, NET_BUF_SZ - 1) && EXPECT_COMMAND(buffer, NOTIFY_GAME_START)) {
                char symbol;
                char *opponent_name = malloc(NET_BUF_SZ);
                strncpy(&symbol, buffer + strlen(NOTIFY_GAME_START), 1);
                strncpy(opponent_name, buffer + strlen(NOTIFY_GAME_START) + 1,
                        NET_BUF_SZ - 2 - strlen(NOTIFY_GAME_START));
                clear();
                printf("Playing as %c against %s. Good luck!\n", symbol, opponent_name);
                init_board();
                print_board();
                printf("Opponent's turn..\n");

                bzero(buffer, NET_BUF_SZ);
                while (safe_sock_read(sockfd, buffer, NET_BUF_SZ - 1)) {

                    if (EXPECT_COMMAND(buffer, NOTIFY_TURN_START)) {
                        clear();
                        extract_board_from_msg(buffer + strlen(NOTIFY_TURN_START));
                        prompt_new_move(sockfd, buffer, symbol, opponent_name);
                    } else if (EXPECT_COMMAND(buffer, MOVE_NOT_ALLOWED_RESPONSE)) {
                        clear();
                        printf("You entered an invalid move!\n");
                        prompt_new_move(sockfd, buffer, symbol, opponent_name);
                    } else if (EXPECT_COMMAND(buffer, GAME_END_RESPONSE)) {
                        char* winner = &buffer[strlen(GAME_END_RESPONSE)];
                        if (!strncmp(winner,DRAW_REPR, strlen(DRAW_REPR)))
                            printf("Game ended in a draw!\n");
                        else printf("Player %s won this round!\n", winner);
                        return;
                    }
                    bzero(buffer, NET_BUF_SZ);
                }
            }

        } else {
            printf("Cannot join the server right now! Please retry later");
        }
    }

    printf("Your opponent has disconnected. Exiting..");
    close(sockfd);
    return;
}

void prompt_new_move(int sockfd, char *buffer, char symbol, const char *opponent_name) {
    printf("Playing as %c against %s. Good luck!\n", symbol, opponent_name);
    print_board();
    printf("Select a move (0-8): ");
    uint8_t pos;
    scanf("%hhu", &pos);
    if (pos < 9 && update_board_with_move(pos, symbol)) {
        sprintf(buffer, "%s%hhu", MOVE_REQUEST, pos);
        safe_sock_write(sockfd, buffer, strlen(buffer));
    } else {
        clear();
        printf("You selected an invalid move!\n");
        prompt_new_move(sockfd, buffer, symbol, opponent_name);
    }
    clear();
    printf("Playing as %c against %s. Good luck!\n", symbol, opponent_name);
    print_board();
    printf("Opponent's turn..\n");
    bzero(buffer, NET_BUF_SZ);
}
