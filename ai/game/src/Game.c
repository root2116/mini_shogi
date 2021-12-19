#include "Game.h"
#include "Board.h"
#include "Player.h"
#include "Referee.h"
#include "utility.h"
#include "list.h"

#include <stdio.h>
#include <stdlib.h>

int file(char input)
{ //試合で指された手をテキストファイルに保存

    FILE *fout;
    // char name[256];
    // scanf("%s", name);//error;&?
    // fp = fopen("test.txt", "r")) != NULL
    if ((fout = fopen("te.txt", "w")) != NULL)
    { // error
        fprintf(fout, "%s", input);
        fclose(fout);
    }
    else
        printf("fp2NG");

    return 0;
}


int count_pieces(Game game){
    int count = 0;

    for(int i = 0; i < 5; i++){
        for(int j = 0; j < 5; j++){
            if(game->board->board[i][j] != NULL) count++;
        }
    }

    for(int i = 0; i < 2; i++){
        for(int j =0; j < 10; j++){
            if(game->board->captured_pieces[i][j] != NULL) count++;
        }
    }

    return count;
}
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
        if(count_pieces(game) != 12){
            printf("Error!!!\n");
            break;
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

    return 0;
}

void free_game(Game game)
{
    free(game->players[0]);
    free(game->players[1]);
    free(game->ref);
    game->board->free_board(game->board);
    free(game->board);
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
    game->ref->update_turn(game->ref);
    game->ref->increment_turn_count(game->ref);
    game->ref->record_board(game->ref, game->board);
    
}

void copy_game(Game this, Game copy){
    this->board->copy_board(this->board,copy->board);
    this->ref->copy_referee(this->ref, copy->ref);

    copy->players[FIRST]->captured_pieces = copy->board->captured_pieces[FIRST];
    copy->players[SECOND]->captured_pieces = copy->board->captured_pieces[SECOND];
    copy->players[FIRST]->turn = this->players[FIRST]->turn;
    copy->players[SECOND]->turn = this->players[SECOND]->turn;


}


bool is_done(Game this){
    return this->is_lose(this) || this->is_draw(this);
}

bool is_lose(Game this){
    return this->ref->is_checkmated(this->ref,this->board,this->ref->get_turn(this->ref));
}

bool is_draw(Game this){
    if(this->ref->get_turn_count(this->ref) > 150){
        return true;
    }else{
        return false;
    }
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
    instance->free_game = free_game;
    instance->next_state = next_state;
    instance->copy_game = copy_game;
    instance->is_done = is_done;
    instance->is_lose = is_lose;
    instance->is_draw = is_draw;



    return instance;
}