#include "../include/game.h"
#include "../include/Board.h"
#include "../include/Player.h"
#include "../include/utility.h"

#include <stdio.h>

void start_game(){

    Board board = new_board(FIRST);

    Player player0 = new_player(FIRST);
    Player player1 = new_player(SECOND);

    Player players[2] = {player0, player1};

    char input[6] = "00000";

    Move move = {0, 0, 0, 0, 0};
    Drop drop = {0, 0, NONE};

    while (1)
    {   
        
        board->increment_turn_count(board);

        init_string(input,'0');

        board->display_board(board, player0, player1);
        printf("%d's turn\n", board->get_turn(board));

        scanf("%s", input);

        int turn = board->get_turn(board);
        if (input[2] >= '0' && input[2] <= '9')
        {
            parse_move(input, &move);
            if (!players[turn]->move_my_piece(players[turn], move, board))
            {
                printf("Invalid move!\n");
                continue;
            }
        }
        else
        {
            parse_drop(input, &drop);
            if (!players[turn]->drop_my_captured(players[turn], drop, board))
            {
                printf("Invalid drop!\n");
                continue;
            }
        }
        if (board->judge_check(board)) printf("王手です");

        board->update_turn(board);
    }
}