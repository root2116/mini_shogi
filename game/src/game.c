#include "../include/game.h"
#include "../include/Board.h"
#include "../include/Player.h"
#include "../include/Referee.h"
#include "../include/utility.h"
#include "../include/list.h"

#include "../../ai/include/game_ai.h"
#include <stdio.h>
#include <stdlib.h>

int file(char input){ //試合で指された手をテキストファイルに保存

    FILE *fout;
    //char name[256];
    //scanf("%s", name);//error;&?
    //fp = fopen("test.txt", "r")) != NULL
    if ((fout = fopen("te.txt", "w")) != NULL){ //error
        fprintf(fout, "%s", input);
        fclose(fout);
    }
    else
        printf("fp2NG");

    
    return 0;
}   

void start_game(){

    

    Board board = new_board();
    Referee ref = new_referee(FIRST);


    Player player0 = new_player(FIRST);
    Player player1 = new_player(SECOND);

    player0->captured_pieces = board->captured_pieces[0];
    player1->captured_pieces = board->captured_pieces[1];
    
    Player players[2] = {player0, player1};

    ref->record_board(ref,board);

    char *input = calloc(6,sizeof(char));
    Move move = {0, 0, 0, 0, 0};
    Drop drop = {0, 0, PAWN};

    while (1)
    {   
        
        if(ref->is_checkmated(ref,board,ref->get_turn(ref))){
            printf("Checkmate!\n");
            break;
        }
        
        init_string(input,'\0');

        board->display_board(board, player0, player1);
        printf("%d's turn\n", ref->get_turn(ref));

        int turn = ref->get_turn(ref);
        
        if(turn == FIRST){
            scanf("%s", input);
        }else{
            random_ai(ref,board,input);
        }
        

        
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

        
        
        
        ref->increment_turn_count(ref);
        ref->record_board(ref,board);

        if (ref->judge_check(ref,board,FIRST)) printf("1の王手です");
        else if(ref->judge_check(ref,board,SECOND)) printf("0の王手です");

        if (ref->check_repetition(ref, board)) printf("千日手です");
        else printf("千日手ではありません");
        
        ref->display_history(ref);
        ref->update_turn(ref);

        file(input);
    }
}