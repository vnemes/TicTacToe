cmake_minimum_required(VERSION 3.13)
project(TicTacToe C)

set(CMAKE_C_STANDARD 99)

add_executable(TicTacToe main.c TTTServer.c TTTServer.h TTTClient.c TTTClient.h)