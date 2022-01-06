#ifndef INCLUDED_NODE
#define INCLUDED_NODE

#include "value_net.h"

enum
{
    Playout,
    CNN
};


struct game_t;
typedef struct game_t *Game;

struct node_t;
typedef struct node_t *Node;



struct node_t
{
    Game game;
    ValueNet net;
    double weight;
    int n;
    Node *child_nodes;
    int n_of_children;
    int weight_type;
    int iter;
    
    void (*init)(Node, Game,ValueNet, int);
    int (*evaluate)(Node);
    void (*expand)(Node);
    Node (*next_child_node)(Node);
    void (*free_node)(Node);
};

void init(Node this, Game game, ValueNet net, int weight_type);
int evaluate(Node this);
void expand(Node this);
Node next_child_node(Node this);
void free_node(Node this);
Node new_node(Game, ValueNet net, int weight_type );

#endif