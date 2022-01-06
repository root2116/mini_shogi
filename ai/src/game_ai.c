#include "game_ai.h"
#include "Board.h"
#include "Player.h"
#include "Referee.h"
#include "list.h"
#include "utility.h"
#include "Game.h"

#include "AI.h"
#include "Node.h"

#include <stdio.h>
#include <stdlib.h>

ValueNet value_net;

double playout(Game game){
    
    game->user_num = SECOND;
    double value = game->cpu_vs_cpu(game, random_ai, random_ai, false);


    return value;
    
}

void load_value_net(){
    load_params(value_net);
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
        for(int j = 0; j < 5; j++){
            game->copy_game(game,copy);
            copy->next_state(copy, *action);
            values[i] += -playout(copy);
        }
    }



    Action *best_action = get_nth(next_actions,argmax(values,len));
    convert_action_into_string(*best_action, output);
    free_list(next_actions);
    free(values);
    copy->free_game(copy);
    
}

void mcts_ai(Game game, char* output){

    
    Game copy = new_game(SECOND);
    game->copy_game(game, copy);

    Node root_node = new_node(copy,NULL, Playout);
    root_node->expand(root_node);

    for(int i = 0; i < 100; i++){
        root_node->evaluate(root_node);
    }

    List next_actions = copy->ref->legal_actions(copy->ref,copy->board, copy->ref->get_turn(copy->ref));
    int max_n = 0;
    int max_index;

    for(int i = 0; i < root_node->n_of_children; i++){
        if(max_n < root_node->child_nodes[i]->n){
            max_n = root_node->child_nodes[i]->n;
            max_index = i;
        }
    }

    Action *best_action = get_nth(next_actions,max_index);

    convert_action_into_string(*best_action,output);
    free_list(next_actions);
    root_node->free_node(root_node);
    


}

void cnn_mcts_ai(Game game, char *output)
{

    Game copy = new_game(SECOND);
    game->copy_game(game, copy);


    Node root_node = new_node(copy, value_net, CNN);
    root_node->expand(root_node);

    for (int i = 0; i < 100; i++)
    {
        root_node->evaluate(root_node);
    }

    List next_actions = copy->ref->legal_actions(copy->ref, copy->board, copy->ref->get_turn(copy->ref));
    int max_n = 0;
    int max_index;

    for (int i = 0; i < root_node->n_of_children; i++)
    {
        if (max_n < root_node->child_nodes[i]->n)
        {
            max_n = root_node->child_nodes[i]->n;
            max_index = i;
        }
    }

    Action *best_action = get_nth(next_actions, max_index);

    convert_action_into_string(*best_action, output);
    free_list(next_actions);
    root_node->free_node(root_node);
}

void evaluate_strength(AI ai0, AI ai1){
    int win = 0;
    int lose = 0;
    int draw = 0;
    for(int i = 0; i < 50; i++){
        Game game = new_game(FIRST);

        printf("round %d\n",(i+1));
        printf("%s: %d\n", ai0->name,win);
        printf("%s: %d\n", ai1->name,lose);
        printf("draw: %d\n", draw);

        int result = game->cpu_vs_cpu(game,ai0->next_action,ai1->next_action,false);
        if(result == 1){
            win++;
        }else if(result == -1){
            lose++;
        }else{
            draw++;
        }
    }

    printf("Final result\n");
    printf("%s: %d\n", ai0->name, win);
    printf("%s: %d\n", ai1->name, lose);
    printf("draw: %d\n", draw);
    
}


