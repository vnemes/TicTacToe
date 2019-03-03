cmake_minimum_required(VERSION 3.13)
project(TicTacToe C)

set(CMAKE_C_STANDARD 99)

add_executable(TicTacToe main.c server/TTTServer.c server/TTTServer.h client/TTTClient.c client/TTTClient.h util/utils.c util/utils.h socket_messages.h)