#ifndef INCLUDED_AI
#define INCLUDED_AI

struct game_t;
typedef struct game_t *Game;

struct ai_t;
typedef struct ai_t *AI;


struct ai_t
{
    char* name;
    void (*next_action)(Game game);
};

AI new_ai(char*, void (*func)());

#endif