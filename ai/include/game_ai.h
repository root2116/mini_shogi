#ifndef INCLUDED_GAME_AI
#define INCLUDED_GAME_AI

struct ai_t;
typedef struct ai_t *AI;

struct action_t;
typedef struct action_t Action;

struct game_t;
typedef struct game_t *Game;

void random_ai(Game ,char*);
void mcs_ai(Game, char*);
int playout(Game);
void evaluate_strength(AI ai0, AI ai1);
void mcts_ai(Game, char*);
#endif
