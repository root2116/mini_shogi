cmake_minimum_required(VERSION 3.2)
project(game C)
add_library(game STATIC ./src/Game.c ./src/Board.c ./src/Bishop.c ./src/Gold.c ./src/King.c ./src/Pawn.c ./src/Piece.c ./src/Player.c ./src/Rook.c ./src/Silver.c ./src/utility.c ./src/Referee.c ./src/list.c)

target_include_directories(game PUBLIC ${CMAKE_SOURCE_DIR}/ai/game/include)

