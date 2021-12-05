#include "../include/game.h"
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