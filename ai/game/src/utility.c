#include "Game.h"
#include "Piece.h"

#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

void convert_array_into_vectors(int array[][2], int len, Vector* vecs){
    

    for(int i = 0; i < len; i++){
        vecs[i].x = array[i][0];
        vecs[i].y = array[i][1];
    }

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


void convert_action_into_string(Action action, char* str){
    

    if(action.drop.kind == NONE){
        Point start = action.move.start;
        Point end = action.move.end;

        str[0] = '5' - start.y;
        str[1] = 'A' + start.x;
        str[2] = '5' - end.y;
        str[3] = 'A' + end.x;

        if(action.move.will_promote){
            str[4] = 'N';
        }



    }else{
        Point loc = action.drop.loc;
        PieceKind kind = action.drop.kind;

        str[0] = '5' - loc.y;
        str[1] = 'A' + loc.x;
        switch (kind)
        {
        case BISHOP:
            strcpy(&str[2],"KK");
            break;
        case ROOK:
            strcpy(&str[2],"HI");
            break;
        case GOLD:
            strcpy(&str[2],"KI");
            break;
        case SILVER:
            strcpy(&str[2],"GI");
            break;
        case PAWN:
            strcpy(&str[2],"FU");
            break;
        default:
            break;
        }

    }
}

int argmax(int *list, int len){
    int max_index = 0;
    int max = list[0];
    for(int i = 0; i < len; i++){
        if(max < list[i]){
            max = list[i];
            max_index = i;
        }

    }

    return max_index;
}

void swap_str(char *str1, char *str2, int len){
    char tmp;
    for(int i = 0; i < len; i++){
        tmp = str1[i];
        str1[i] = str2[i];
        str2[i] = tmp;
    }
    
}