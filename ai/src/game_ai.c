#include "game_ai.h"
#include "Board.h"
#include "Player.h"
#include "Referee.h"
#include "list.h"
#include "utility.h"
#include "Game.h"

#include <stdio.h>
#include <stdlib.h>


int playout(Game game, Action action){
    Game clone = game->clone_game(game);
    clone->next_state(clone,action);
    clone->user_num = SECOND;

    int value = -clone->cpu_vs_cpu(clone, random_ai, random_ai);
    clone->free_game(clone);
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

    List next_actions = ref->legal_actions(ref, board, ref->get_turn(ref));
    int len = length(next_actions);
    int *values = calloc(len,sizeof(int));
    for(int i = 0; i < len; i++){
        Action *action = get_nth(next_actions,i);
        for(int j = 0; j < 3; j++){
            values[i] += playout(game,*action);
        }
    }

    Action *best_action = get_nth(next_actions,argmax(values,len));
    convert_action_into_string(*best_action, output);
    free_list(next_actions);
    free(values);
    
}


