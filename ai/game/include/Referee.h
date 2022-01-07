#ifndef INCLUDED_REFEREE
#define INCLUDED_REFEREE



#include <stdbool.h>


#define HISTORY_ROW_NUM 152
#define HISTORY_COL_NUM 46

struct point_t;
typedef struct point_t Point;

struct list_t;
typedef struct list_t *List;

struct piece_t;
typedef struct piece_t *Piece;

struct player_t;
typedef struct player_t *Player;

struct board_t;
typedef struct board_t *Board;

struct referee_t;
typedef struct referee_t *Referee;

struct referee_t
{

    char history[HISTORY_ROW_NUM][HISTORY_COL_NUM];
    int turn;
    int turn_count;

    void (*display_history)(Referee);
    void (*update_turn)(Referee);
    void (*increment_turn_count)(Referee);
    int (*get_turn)(Referee);
    int (*get_turn_count)(Referee);
    bool (*is_legal_move)(Referee,Board, Piece, Point, bool);
    bool (*is_legal_drop)(Referee, Board, Piece, Point);
    bool (*can_promote)(Referee, Piece, Point);
    void (*record_board)(Referee , Board);
    bool (*check_repetition)(Referee, Board);
    bool (*judge_check)(Referee, Board, int);
    bool (*check_double_pawn)(Referee, Board, Piece, Point);
    bool (*will_be_checked)(Referee, Board, Piece, Point);
    bool (*will_checkmate)(Referee, Board, Piece, Point);
    List (*legal_actions)(Referee,Board, int);
    bool (*is_checkmated)(Referee, Board, int);
    void (*copy_referee)(Referee, Referee);
    
};

Referee new_referee(int turn);

#endif