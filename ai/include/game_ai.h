#ifndef INCLUDED_GAME_AI
#define INCLUDED_GAME_AI

struct action_t;
typedef struct action_t Action;

struct game_t;
typedef struct game_t *Game;

void random_ai(Game ,char*);
void mcs_ai(Game, char*);
int playout(Game);
void evaluate_strength(void (*game_ai0)(), void (*game_ai1)());
#endif
