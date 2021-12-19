#include "Game.h"
#include "Board.h"
#include "Player.h"
#include "Referee.h"
#include "utility.h"
#include "list.h"

#include <stdio.h>
#include <stdlib.h>

void user_vs_cpu(Game game, void (*game_ai)()){

    Referee ref = game->ref;
    Board board = game->board;

    Player players[2] = {game->players[0], game->players[1]};
    char input[6];
    Move move = {0, 0, 0, 0, 0};
    Drop drop = {0, 0, NONE};

    while (1)
    {   
        
        if(ref->is_checkmated(ref,board,ref->get_turn(ref))){
            printf("Checkmate!\n");
            break;
        }
        
        init_string(input,'\0');

        board->display_board(board, players[0], players[1]);
        printf("%d's turn\n", ref->get_turn(ref));

        int turn = ref->get_turn(ref);
        
        if(turn == game->user_num){
            scanf("%s", input);
        }else{
            game_ai(game,input);
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
    }
}

void user_vs_user(Game game){

    Referee ref = game->ref;
    Board board = game->board;

    Player players[2] = {game->players[0],game->players[1]};
    char input[6]; 
    Move move = {0, 0, 0, 0, 0};
    Drop drop = {0, 0, NONE};

    while (1)
    {   
        
        if(ref->is_checkmated(ref,board,ref->get_turn(ref))){
            printf("Checkmate!\n");
            break;
        }
        
        init_string(input,'\0');

        board->display_board(board, players[0], players[1]);
        printf("%d's turn\n", ref->get_turn(ref));

        int turn = ref->get_turn(ref);
        
        
        scanf("%s", input);
    
    
        
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
    }
}


int cpu_vs_cpu(Game game, void (*game_ai0)(), void (*game_ai1)()){
    Board board = game->board;
    Referee ref = game->ref;
    Player players[2] = {game->players[0],game->players[1]};

    int first_turn = ref->get_turn(ref);

    
    char input[6];
    Move move = {0, 0, 0, 0, 0};
    Drop drop = {0, 0, NONE};
    

    while(true){
        
        int turn = ref->get_turn(ref);
        if(game->user_num == FIRST){
            board->display_board(board, players[0], players[1]);
            printf("%d: %d's turn\n", ref->get_turn_count(ref),turn);
            
        }


        if(ref->is_checkmated(ref,board,turn)){
            if(turn == first_turn){
                return -1;
            }else{
                return 1;
            }
            
        }

        if(ref->get_turn_count(ref) > 150){
            return 0;
        }
        
        init_string(input,'\0');

        if(turn == FIRST){
            game_ai0(game,input);
        }else{
            game_ai1(game,input);
        }

        if(game->user_num == FIRST){

            printf("%s\n",input);
        }
        

        

        if (input[2] >= '0' && input[2] <= '9'){
            parse_move(input, &move);
            players[turn]->move_my_piece(players[turn], move, board,ref);
        }else{
            parse_drop(input, &drop);
            players[turn]->drop_my_captured(players[turn], drop, board, ref);
        }

        // printf("%d\n",ref->get_turn_count(ref));
        ref->increment_turn_count(ref);
        ref->record_board(ref,board);
        ref->update_turn(ref);

    }
}

void free_game(Game game)
{
    free(game->players[0]);
    free(game->players[1]);
    game->ref->free_referee(game->ref);
    game->board->free_board(game->board);
    free(game);
}


void next_state(Game game, Action action){
    int turn = game->ref->get_turn(game->ref);
    

    if (action.drop.kind == NONE)
    {
       
        game->players[turn]->move_my_piece(game->players[turn], action.move, game->board, game->ref);
    }
    else
    {
        game->players[turn]->drop_my_captured(game->players[turn], action.drop, game->board, game->ref);
    }
}
//ゲームオブジェクトを複製する。初期状態をクローンしてはいけない
Game clone_game(Game game){
    Game new = malloc(sizeof(*new_game));
    
    Board board = game->board->clone_board(game->board);
    Referee ref = game->ref->clone_referee(game->ref);

    Player player0 = new_player(FIRST);
    Player player1 = new_player(SECOND);

    player0->captured_pieces = board->captured_pieces[0];
    player1->captured_pieces = board->captured_pieces[1];

    new->board = board;
    new->ref = ref;
    new->players[0] = player0;
    new->players[1] = player1;

    new->user_vs_cpu = user_vs_cpu;
    new->user_vs_user = user_vs_user;
    new->cpu_vs_cpu = cpu_vs_cpu;
    new->clone_game = clone_game;
    new->free_game = free_game;
    new->next_state = next_state;

    return new;

}





Game new_game(int side){

    Game instance = malloc(sizeof(*instance));

    Board board = new_board();
    Referee ref = new_referee(side);


    Player player0 = new_player(FIRST);
    Player player1 = new_player(SECOND);

    player0->captured_pieces = board->captured_pieces[0];
    player1->captured_pieces = board->captured_pieces[1];
    
    ref->record_board(ref,board);


    instance->user_num = side;
    instance->board = board;
    instance->ref = ref;
    instance->players[0] = player0;
    instance->players[1] = player1;
    


    instance->user_vs_cpu = user_vs_cpu;
    instance->user_vs_user = user_vs_user;
    instance->cpu_vs_cpu = cpu_vs_cpu;
    instance->clone_game = clone_game;
    instance->free_game = free_game;
    instance->next_state = next_state;



    return instance;
}