//
// Created by root on 3/3/19.
//

#include "TTTClient.h"

int client_connect(int portno, char *server_host) {
    int sockfd, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];

    printf("Client attempting connection on port to host %s:%d\n",server_host,portno);
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
    bcopy((char *) server->h_addr,
          (char *) &serv_addr.sin_addr.s_addr,
          server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        exit_with_error("ERROR connecting");

    printf("Please enter the message: ");
    fflush(stdin);
    bzero(buffer, 256);
    fgets(buffer, 255, stdin);

    while (strncmp(buffer, "exit", 4)) {

        n = write(sockfd, buffer, strlen(buffer));
        if (n < 0)
            exit_with_error("ERROR writing to socket");
        bzero(buffer, 256);
        n = read(sockfd, buffer, 255);
        if (n < 0)
            exit_with_error("ERROR reading from socket");
        printf("%s\n", buffer);

        printf("Please enter the message: ");
        fflush(stdin);
        bzero(buffer, 256);
        fgets(buffer, 255, stdin);
    }
    close(sockfd);
    return 0;
}
