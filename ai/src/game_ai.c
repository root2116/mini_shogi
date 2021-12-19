#include "game_ai.h"
#include "Board.h"
#include "Player.h"
#include "Referee.h"
#include "list.h"
#include "utility.h"
#include "Game.h"

#include <stdio.h>
#include <stdlib.h>


int playout(Game game){
    
    
    
    game->user_num = SECOND;
    int value = -game->cpu_vs_cpu(game, random_ai, random_ai);


    return value;
    
}

void random_ai(Game game, char* output){

    Board board = game->board;
    Referee ref = game->ref;

    List next_actions = ref->legal_actions(ref,board,ref->get_turn(ref));

    
    int len = length(next_actions);

    int rand_num = rand() % len;

    for(int i = 0; i < len; i++){
        if(i == rand_num){
            convert_action_into_string(next_actions->action,output);
            break;
        }
        next_actions = next_actions->next;
    }

    free_list(next_actions);
}


void mcs_ai(Game game, char* output){
    Board board = game->board;
    Referee ref = game->ref;
    Game copy = new_game(SECOND);


    List next_actions = ref->legal_actions(ref, board, ref->get_turn(ref));
    int len = length(next_actions);
    int *values = calloc(len,sizeof(int));
    for(int i = 0; i < len; i++){
        Action *action = get_nth(next_actions,i);
        for(int j = 0; j < 8; j++){
            game->copy_game(game,copy);
            copy->next_state(copy, *action);
            values[i] += playout(copy);
        }
    }

    Action *best_action = get_nth(next_actions,argmax(values,len));
    convert_action_into_string(*best_action, output);
    free_list(next_actions);
    free(values);
    
}

void evaluate_strength(void (*game_ai0)(), void (*game_ai1)()){
    int win = 0;
    int lose = 0;
    int draw = 0;
    for(int i = 0; i < 100; i++){
        Game game = new_game(FIRST);
        printf("round %d\n",(i+1));
        int result = game->cpu_vs_cpu(game,game_ai0,game_ai1);
        if(result == 1){
            win++;
        }else if(result == -1){
            lose++;
        }else{
            draw++;
        }
    }

    printf("game_ai0: %d\n",win);
    printf("game_ai1: %d\n",lose);
    printf("draw: %d\n",draw);
}


