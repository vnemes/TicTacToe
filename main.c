/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include "TTTServer.h"
#include "TTTClient.h"


int main(int argc, char *argv[]) {
    printf("Please select run mode: \n"); 
    printf("1. Server\n");
    printf("2. Client\n");
    int choice;
    scanf("%d", &choice);
    if (choice == 1)
        server_listen(2020);
    else
        client_connect(2020, "localhost");
    return 0;
}