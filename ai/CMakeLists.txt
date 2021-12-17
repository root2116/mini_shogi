cmake_minimum_required(VERSION 3.2)
project(ai C)
add_library(ai STATIC ./src/game_ai.c)

target_include_directories(ai INTERFACE ./include)
