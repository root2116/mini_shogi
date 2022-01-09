#include "Node.h"
#include "Referee.h"
#include "Game.h"
#include "list.h"

#include "game_ai.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void init(Node this, Game game, ValueNet net, int weight_type){
    
    this->game = game;
    this->net = net;
    this->weight = 0;
    this->n = 0;
    this->child_nodes = NULL;
    this->weight_type = weight_type;
    if(weight_type == Playout){
        this->iter = 10;
    }else if(weight_type == CNN){
        this->iter = 1;
    }
    

    this->evaluate = evaluate;
    this->expand = expand;
    this->next_child_node = next_child_node;
    this->free_node = free_node;
}

int evaluate(Node this){
    if(this->game->is_done(this->game)){
        double value;
        if(this->game->is_lose(this->game)) value = -1;
        else value = 0;

        this->weight += value;
        this->n++;
        return value;
    }

    if(this->child_nodes == NULL){
        Game copy = new_game(SECOND);
        this->game->copy_game(this->game, copy);

        double value;
        if(this->weight_type == Playout){
            value = playout(copy);
        }else if(this->weight_type == CNN){

            if(copy->ref->turn == FIRST){
                //FIRSTから見た今の盤面の価値を返す
                value = this->net->predict_value(this->net, copy);
            }else{
                value = 1 - this->net->predict_value(this->net,copy);
            }
            
        }

        this->weight += value;
        this->n++;

        if(this->n == this->iter){
            this->expand(this);
        }

        copy->free_game(copy);
        
        return value;

    }else{

        Node next_child = this->next_child_node(this);

        double value = -next_child->evaluate(next_child);
        this->weight += value;
        this->n++;
        return value;
    }


    



}

void expand(Node this){
    Game game = this->game;
    List next_actions = game->ref->legal_actions(game->ref, game->board, game->ref->get_turn(game->ref));
    int len = length(next_actions);

    Node *child_nodes = calloc(len,sizeof(Node));
    
    for(int i = 0; i < len; i++){
        child_nodes[i] = calloc(1,sizeof(struct node_t));
    }

    
    for(int i = 0; i < len; i++){
        Game copy = new_game(FIRST);
        game->copy_game(game, copy);
        child_nodes[i]->init = init;
        copy->next_state(copy,*get_nth(next_actions,i));
        child_nodes[i]->init(child_nodes[i],copy,this->net, this->weight_type);
        
    }

    this->n_of_children = len;

    this->child_nodes = child_nodes;

    free_list(next_actions);

}

Node next_child_node(Node this){
    for(int i = 0; i < this->n_of_children; i++){
        if(this->child_nodes[i]->n == 0){
            return this->child_nodes[i];
        }
    }

    int t = 0;
    for(int i = 0; i < this->n_of_children; i++){
        t += this->child_nodes[i]->n;
    }

    double max_ucb1 = -100000000;
    int max_index;

    for(int i = 0; i < this->n_of_children; i++){
        double weight = this->child_nodes[i]->weight;
        int n = this->child_nodes[i]->n;
        double ucb1_value = -(double)weight/n + sqrt((2*log(t)/n));
        if(ucb1_value > max_ucb1){
            max_ucb1 = ucb1_value;
            max_index = i;
        }
    }

    return this->child_nodes[max_index];

}


void free_node(Node this){

    if(this->child_nodes == NULL){
        this->game->free_game(this->game);
        free(this);
        return;
    }


    Node *tmp = this->child_nodes;
    int len = this->n_of_children;
    this->game->free_game(this->game);
    free(this);

    for(int i = 0; i < len; i++){
        free_node(tmp[i]);
    }
    

}

Node new_node(Game game, ValueNet net, int weight_type){
    Node instance = calloc(1,sizeof(*instance));

    instance->game = game;
    instance->net = net;
    instance->weight = 0;
    instance->n = 0;
    instance->child_nodes = NULL;
    instance->n_of_children = 0;
    instance->weight_type = weight_type;
    if(weight_type == Playout){
        instance->iter = 10;
    }else if(weight_type == CNN){
        instance->iter = 1;
    }
    instance->evaluate = evaluate;
    instance->expand = expand;
    instance->next_child_node = next_child_node;
    instance->free_node = free_node;

    return instance;
    

}