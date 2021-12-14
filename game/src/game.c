#include "../include/game.h"
#include "../include/Board.h"
#include "../include/Player.h"
#include "../include/Referee.h"
#include "../include/utility.h"
#include "../include/list.h"

#include <stdio.h>

void start_game(){

    
    


    Board board = new_board();
    Referee ref = new_referee(FIRST);
    
    Player player0 = new_player(FIRST);
    Player player1 = new_player(SECOND);

    player0->captured_pieces = board->captured_pieces[0];
    player1->captured_pieces = board->captured_pieces[1];
    
    Player players[2] = {player0, player1};

    ref->record_board(ref,board);

    char input[6] = "00000";
    Move move = {0, 0, 0, 0, 0};
    Drop drop = {0, 0, PAWN};

    while (1)
    {   
        
        

        init_string(input,'0');

        board->display_board(board, player0, player1);
        printf("%d's turn\n", ref->get_turn(ref));

        scanf("%s", input);

        int turn = ref->get_turn(ref);
        if (input[2] >= '0' && input[2] <= '9')
        {
            parse_move(input, &move);
            if (!players[turn]->move_my_piece(players[turn], move, board,ref))
            {
                printf("Invalid move!\n");
                continue;
            }
        }
        else
        {
            parse_drop(input, &drop);
            if (!players[turn]->drop_my_captured(players[turn], drop, board, ref))
            {
                printf("Invalid drop!\n");
                continue;
            }
        }

        ref->record_board(ref,board);

        if (ref->judge_check(ref,board,FIRST)) printf("1の王手です");
        else if(ref->judge_check(ref,board,SECOND)) printf("0の王手です");
        
        ref->display_history(ref);
        ref->increment_turn_count(ref);
        ref->update_turn(ref);
    }
}