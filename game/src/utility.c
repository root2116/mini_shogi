#include "../include/game.h"
#include "../include/Piece.h"
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

Vector* convert_array_into_vectors(int array[][2], int len){
    Vector *vecs = calloc(len,sizeof(Vector));

    for(int i = 0; i < len; i++){
        vecs[i].x = array[i][0];
        vecs[i].y = array[i][1];
    }

    return vecs;
}

void add_vec_to_point(Point a, Vector v, Point *b)
{
    
    b->x = a.x + v.x;
    b->y = a.y + v.y;
    
    
}

bool is_same_point(Point a, Point b){
    if(a.x == b.x && a.y == b.y) return true;
    else return false;
}

int gcd(int x, int y){

    if (y == 0){
        return x;
    }
    else{
        return gcd(y, x % y);
    }
}

int ctoi(char c)
{
    if (c >= '0' && c <= '9')
    {
        return c - '0';
    }
    return 0;
}

void parse_move(char *input, Move *move)
{

    move->start.x = input[1] - 65;
    move->start.y = 5 - ctoi(input[0]);
    move->end.x = input[3] - 65;
    move->end.y = 5 - ctoi(input[2]);
    
    if(input[4] == 'N'){
        move->will_promote = true;
    }else{
        move->will_promote = false;
    }
    
}

void parse_drop(char *input, Drop *drop)
{
    drop->loc.x = input[1] - 65;
    drop->loc.y = 5 - ctoi(input[0]);

    char *kind = &input[2];
    if(strcmp(kind,"HI") == 0){
        drop->kind = ROOK;
    }else if(strcmp(kind,"KK") == 0){
        drop->kind = BISHOP;
    }else if(strcmp(kind,"KI") == 0){
        drop->kind = GOLD;
    }else if(strcmp(kind,"GI") == 0){
        drop->kind = SILVER;
    }else{
        drop->kind = PAWN;
    }
}


void multiply_array_by_minus(int array[][2], int len){
    for(int i = 0; i < len; i++){
        array[i][0] = -array[i][0];
        array[i][1] = -array[i][1];
    }

}

void inverse_vectors(Vector *vecs, int len){
    for(int i = 0; i < len; i++){
        vecs[i].x = -vecs[i].x;
        vecs[i].y = -vecs[i].y;
    }
}

void init_string(char* str,char init){
    while(*str != '\0'){
        *str = init;
        str++;
    }
}

void swap_piece_attributes(Piece this)
{
    // abilityとidle_abilityをswap.
    Ability tmp_ability = this->m->ability;
    this->m->ability = this->m->idle_ability;
    this->m->idle_ability = tmp_ability;

    PieceKind tmp_kind = this->m->kind;
    this->m->kind = this->m->idle_kind;
    this->m->idle_kind = tmp_kind;

    // display_boardを使わなければこの処理はいらない
    char *tmp_name = this->m->eng_name;
    this->m->eng_name = this->m->idle_eng_name;
    this->m->idle_eng_name = tmp_name;
}