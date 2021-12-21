#ifndef INCLUDED_NODE
#define INCLUDED_NODE



struct game_t;
typedef struct game_t *Game;

struct node_t;
typedef struct node_t *Node;



struct node_t
{
    Game game;
    int weight;
    int n;
    Node *child_nodes;
    int n_of_children;
    void (*init)(Node, Game);
    int (*evaluate)(Node);
    void (*expand)(Node);
    Node (*next_child_node)(Node);
    void (*free_node)(Node);
};

void init(Node this, Game game);
int evaluate(Node this);
void expand(Node this);
Node next_child_node(Node this);
void free_node(Node this);
Node new_node(Game);

#endif