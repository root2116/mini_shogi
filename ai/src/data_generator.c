#include "data_generator.h"
#include "Game.h"
#include "Board.h"
#include "game_ai.h"
#include "Referee.h"
#include "Piece.h"

#include "Pawn.h"
#include "King.h"
#include "Bishop.h"
#include "Gold.h"
#include "Rook.h"
#include "Silver.h"

#include "matrix.h"
#include "tensor.h"

#include "util.h"

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define CHANNEL_NUM 40



void genenrate_data(char* file_name1, char* file_name2, int num){
    
    int count = 0;
    while(count < num){
        
        Game game = new_game(FIRST);
        printf("%d round\n", count);
        int result = game->cpu_vs_cpu(game, mcts_ai, mcts_ai, true);

        if(result == 0) continue;

       


        FILE *fp1 = fopen(file_name1,"a");
        save_data(fp1,game);
        FILE *fp2 = fopen(file_name2, "a");

        if(result == 1){
            for(int i = 0; i < game->ref->turn_count; i++){
                fprintf(fp2, "%d\n", 1);
            }
        }else{
            for(int i = 0; i < game->ref->turn_count; i++){
                fprintf(fp2, "%d\n", 0);
            }
        }
        
        fclose(fp1);
        fclose(fp2);
        game->free_game(game);

        count++;
    }
        
    
}

void save_data(FILE *fp, Game game){


    for(int i = 1; i <= game->ref->turn_count; i++){
        

        Board board = restore_board_from_str(game->ref->history[i]);

        save_board_data(fp,board, PAWN, FIRST);
        save_board_data(fp,board, SILVER, FIRST);
        save_board_data(fp,board, GOLD, FIRST);
        save_board_data(fp,board, BISHOP, FIRST);
        save_board_data(fp,board, ROOK, FIRST);
        save_board_data(fp,board, KING, FIRST);
        save_board_data(fp,board, PROMOTED_PAWN, FIRST);
        save_board_data(fp,board, PROMOTED_SILVER, FIRST);
        save_board_data(fp,board, HORSE, FIRST);
        save_board_data(fp,board, DRAGON, FIRST);

        save_board_data(fp,board, PAWN, SECOND);
        save_board_data(fp,board, SILVER, SECOND);
        save_board_data(fp,board, GOLD, SECOND);
        save_board_data(fp,board, BISHOP, SECOND);
        save_board_data(fp,board, ROOK, SECOND);
        save_board_data(fp,board, KING, SECOND);
        save_board_data(fp,board, PROMOTED_PAWN, SECOND);
        save_board_data(fp,board, PROMOTED_SILVER, SECOND);
        save_board_data(fp,board, HORSE, SECOND);
        save_board_data(fp,board, DRAGON, SECOND);

        save_captured_data(fp, board, PAWN, FIRST);
        save_captured_data(fp, board, PAWN, SECOND);
        save_captured_data(fp, board, SILVER, FIRST);
        save_captured_data(fp, board, SILVER, SECOND);
        save_captured_data(fp, board, GOLD, FIRST);
        save_captured_data(fp, board, GOLD, SECOND);
        save_captured_data(fp, board, BISHOP, FIRST);
        save_captured_data(fp, board, BISHOP, SECOND);
        save_captured_data(fp, board, ROOK, FIRST);
        save_captured_data(fp, board, ROOK, SECOND);
        
        board->free_board(board);
    }
}

void save_channel(FILE *fp, int data[5][5]){

    for(int i = 0; i < 5; i++){
            for(int j = 0; j < 5; j++){
                fprintf(fp,"%d ",data[i][j]);
            }
            fprintf(fp,"\n");
        }
}

void save_board_data(FILE *fp, Board board, PieceKind kind, int side)
{   
    int data[5][5];
    
    for(int i = 0; i < 5; i++){
        for(int j = 0; j < 5; j++){
            Piece piece = board->board[i][j];
            if(piece == NULL){
                data[i][j] = 0;
            }else if(piece->kind == kind && piece->side == side){
                data[i][j] = 1;
            }else{
                data[i][j] = 0;
            }
        }
    }
    
    save_channel(fp, data);

}

void save_captured_data(FILE *fp, Board board, PieceKind kind, int side){
    int ones[5][5];

    for(int i = 0; i < 5; i++){
        for(int j = 0; j <5 ; j++){
            ones[i][j] = 1;
        }
    }

    int zeros[5][5] = {0};

    int count = 0;

    for(int i = 0; i < 10; i++){
        Piece piece = board->captured_pieces[side][i];
        if (piece == NULL)
            continue;

        if (piece->kind == kind)
        {
            count++;
        }
    }

    if(count == 2){
       
        save_channel(fp, ones);
        save_channel(fp, ones);

    }else if(count == 1){
        save_channel(fp,ones);
        save_channel(fp,zeros);

    }else{
        save_channel(fp,zeros);
        save_channel(fp,zeros);
    }

  

    

}


Board restore_board_from_str(char board_str[]){
    Board board = new_board();

    for(int i = 0; i < 5; i++){
        for(int j = 0; j < 5; j++){
            Piece piece = board->board[i][j];
            if(piece != NULL){
                free(piece);
            }
        }
    }


    for(int i = 0; i < 5; i++){
        for(int j = 0; j < 5; j++){
            char piece_str = board_str[i * 5 + j];
            if(piece_str == '.'){
                board->board[i][j] = NULL;
            }else if(isupper(piece_str)){
                Piece piece;
                switch (piece_str)
                {
                case 'P':
                    board->board[i][j] = new_pawn(FIRST);
                    break;
                
                case 'K':
                    board->board[i][j] = new_king(FIRST);
                    break;
                
                case 'G':
                    board->board[i][j] = new_gold(FIRST);
                    break;
                
                case 'S':
                    board->board[i][j] = new_silver(FIRST);
                    break;
                
                case 'B':
                    board->board[i][j] = new_bishop(FIRST);
                    break;
                
                case 'R':
                    board->board[i][j] = new_rook(FIRST);
                    break;

                case 'T':
                    piece = new_pawn(FIRST);
                    piece->promote(piece);
                    board->board[i][j] = piece;
                    break;
                
                case 'N':
                    piece = new_silver(FIRST);
                    piece->promote(piece);
                    board->board[i][j] = piece;
                    break;
                
                case 'H':
                    piece = new_bishop(FIRST);
                    piece->promote(piece);
                    board->board[i][j] = piece;
                    break;
                
                case 'D':
                    piece = new_rook(FIRST);
                    piece->promote(piece);
                    board->board[i][j] = piece;
                    break;
                
                default:
                    fprintf(stderr, "Invalid character.\n");
                    break;
                }

                board->board[i][j]->cur_loc.x = j;
                board->board[i][j]->cur_loc.y = i;

            }else{

                Piece piece;
                switch (piece_str)
                {
                case 'p':
                    board->board[i][j] = new_pawn(SECOND);
                    break;

                case 'k':
                    board->board[i][j] = new_king(SECOND);
                    break;

                case 'g':
                    board->board[i][j] = new_gold(SECOND);
                    break;

                case 's':
                    board->board[i][j] = new_silver(SECOND);
                    break;

                case 'b':
                    board->board[i][j] = new_bishop(SECOND);
                    break;

                case 'r':
                    board->board[i][j] = new_rook(SECOND);
                    break;

                case 't':
                    piece = new_pawn(SECOND);
                    piece->promote(piece);
                    board->board[i][j] = piece;
                    break;

                case 'n':
                    piece = new_silver(SECOND);
                    piece->promote(piece);
                    board->board[i][j] = piece;
                    break;

                case 'h':
                    piece = new_bishop(SECOND);
                    piece->promote(piece);
                    board->board[i][j] = piece;
                    break;

                case 'd':
                    piece = new_rook(SECOND);
                    piece->promote(piece);
                    board->board[i][j] = piece;
                    break;

                default:
                    fprintf(stderr, "Invalid character.\n");
                    break;
                }

                board->board[i][j]->cur_loc.x = j;
                board->board[i][j]->cur_loc.y = i;
            }
        }
    }


    for(int i = 0; i < 2; i++){
        for(int j = 0; j < 10; j++){
            char piece_str = board_str[25 + i * 10 + j];

            if(piece_str == '.'){
                board->captured_pieces[i][j] = NULL;
            }else{

                switch (piece_str)
                {
                case 'P':
                    board->captured_pieces[i][j] = new_pawn(FIRST);
                    break;

                case 'K':
                    board->captured_pieces[i][j] = new_king(FIRST);
                    break;

                case 'G':
                    board->captured_pieces[i][j] = new_gold(FIRST);
                    break;

                case 'S':
                    board->captured_pieces[i][j] = new_silver(FIRST);
                    break;

                case 'B':
                    board->captured_pieces[i][j] = new_bishop(FIRST);
                    break;

                case 'R':
                    board->captured_pieces[i][j] = new_rook(FIRST);
                    break;

                case 'p':
                    board->captured_pieces[i][j] = new_pawn(SECOND);
                    break;

                case 'k':
                    board->captured_pieces[i][j] = new_king(SECOND);
                    break;

                case 'g':
                    board->captured_pieces[i][j] = new_gold(SECOND);
                    break;

                case 's':
                    board->captured_pieces[i][j] = new_silver(SECOND);
                    break;

                case 'b':
                    board->captured_pieces[i][j] = new_bishop(SECOND);
                    break;

                case 'r':
                    board->captured_pieces[i][j] = new_rook(SECOND);
                    break;

                default:
                    fprintf(stderr, "Invalid character.\n");
                    break;
                }

                board->captured_pieces[i][j]->cur_loc.x = -1;
                board->captured_pieces[i][j]->cur_loc.y = -1;

            }
        }
    }


    return board;
}


Tensor load_x_train_data(char *file_name){

    FILE *fp = fopen(file_name, "r");
    
    int line_count = count_lines(file_name);

    int train_size = line_count / (5*CHANNEL_NUM);
    Tensor R = create_tensor(train_size,CHANNEL_NUM, 5, 5);
    
    int i = 0, j = 0, k = 0, l = 0;

    char data[32];
    
    
    while(i < train_size){
        fgets(data,32,fp);
        char *p = data;
        char *head = data;

        l = 0;
        while (*p != '\n')
        {
            head = p;
            while (*p != ' ')
            {
                p++;
            }
            *p = '\0';
            write_tensor(atof(head),R,i,j,k,l);
            p++;
            l++;
        }
        k++;
        if(k == 5){
            k = 0;
            j++;
            if(j == CHANNEL_NUM){
                j = 0;
                i++;
            }
        }
    }

    fclose(fp);

    return R;
}

Vector load_x_label_data(char *file_name){
    FILE *fp = fopen(file_name, "r");

    char data[32];

    int train_size = count_lines(file_name);
   
    
    Vector r = create_vector(train_size);

    
    int i = 0;
    while (i < train_size)
    {
        fgets(data, 32, fp);
        r->elements[i] = atof(data);
        i++;
    }



    return r;
}

Tensor load_t_test_data(char* file_name){
    FILE *fp = fopen(file_name, "r");
    int i = 0, j = 0, k = 0, l = 0;

    char data[32];

   int line_count = count_lines(file_name);

    int test_size = line_count / (5*CHANNEL_NUM);

    Tensor R = create_tensor(test_size, CHANNEL_NUM, 5, 5);
    
    while(i < test_size){
        fgets(data,32,fp);
        char *p = data;
        char *head = data;

        l = 0;
        while (*p != '\n')
        {
            head = p;
            while (*p != ' ' )
            {
                p++;
            }
            *p = '\0';
            write_tensor(atof(head),R,i,j,k,l);
            p++;
            l++;
        }
        k++;
        if(k == 5){
            k = 0;
            j++;
            if(j == CHANNEL_NUM){
                j = 0;
                i++;
            }
        }
    }

    fclose(fp);

    return R;

}

Vector load_t_label_data(char *file_name){
    FILE *fp = fopen(file_name, "r");

    char data[32];
    int test_size = count_lines(file_name);
    

    Vector r = create_vector(test_size);

    int i = 0;
    while (i < test_size)
    {
        fgets(data, 32, fp);
        r->elements[i] = atof(data);
        i++;
    }

    return r;
}


void print_data(Tensor x){
    for(int i = 0; i < x->num; i++){
        printf("%d :n\n",i);
        for(int j = 0; j < x->chs; j++){
            printf("%d :ch\n",j);
            for(int k = 0; k < x->rows; k++){
                for(int l = 0; l < x->cols; l++){
                    printf("%lf ",read_tensor(x,i,j,k,l));
                }
                printf("\n");
            }
            printf("\n");
        }
        printf("\n");
    }
}


void convert_piece_into_channel(Board board,Tensor T, int ch, PieceKind kind, int side)
{

    for(int i = 0; i < 5; i++){
        for(int j = 0; j < 5; j++){
            Piece piece = board->board[i][j];
            if(piece == NULL){
                write_tensor(0, T, 1, ch, i,  j);
            }else if(piece->kind == kind && piece->side == side){
                write_tensor(1, T, 1, ch, i, j);
            }else{
                write_tensor(0, T, 1, ch, i, j);
            }
        }
    }

}

void convert_captured_into_channel(Board board, Tensor T, int ch, PieceKind kind, int side){

    int count = 0;
    for(int i = 0; i < 10; i++){
        Piece piece = board->captured_pieces[side][i];
        if(piece == NULL) continue;

        if(piece->kind == kind){
            count++;
        }
    }

    if(count == 2){
        for(int i =0; i < 5; i++){
            for(int j = 0; j < 5; j++){
                write_tensor(1, T, 1, ch, i, j);
                write_tensor(1, T, 1, ch+1, i, j);
            }
        }
    }else if(count == 1){
        for(int i = 0; i < 5; i++){
            for(int j = 0; j < 5; j++){
                write_tensor(1, T, 1, ch, i , j);
                write_tensor(0, T, 1, ch+1, i, j);
            }
        }
    }else{
        for(int i = 0; i < 5; i++){
            for(int j = 0; j < 5; j++){
                write_tensor(0, T, 1, ch, i , j);
                write_tensor(0, T, 1, ch+1, i, j);
            }
        }
        
    }


}

Tensor convert_board_into_tensor(Board board){
    Tensor T = create_tensor(1,CHANNEL_NUM, 5, 5);

    

    convert_piece_into_channel(board, T, 0, PAWN, FIRST);
    convert_piece_into_channel(board, T, 1, SILVER, FIRST);
    convert_piece_into_channel(board, T, 2, GOLD, FIRST);
    convert_piece_into_channel(board, T, 3, BISHOP, FIRST);
    convert_piece_into_channel(board, T, 4, ROOK, FIRST);
    convert_piece_into_channel(board, T, 5, KING, FIRST);
    convert_piece_into_channel(board, T, 6, PROMOTED_PAWN, FIRST);
    convert_piece_into_channel(board, T, 7, PROMOTED_SILVER, FIRST);
    convert_piece_into_channel(board, T, 8, HORSE, FIRST);
    convert_piece_into_channel(board, T, 9, DRAGON, FIRST);
    convert_piece_into_channel(board, T, 10, PAWN, SECOND);
    convert_piece_into_channel(board, T, 11, SILVER, SECOND);
    convert_piece_into_channel(board, T, 12, GOLD, SECOND);
    convert_piece_into_channel(board, T, 13, BISHOP, SECOND);
    convert_piece_into_channel(board, T, 14, ROOK, SECOND);
    convert_piece_into_channel(board, T, 15, KING, SECOND);
    convert_piece_into_channel(board, T, 16, PROMOTED_PAWN, SECOND);
    convert_piece_into_channel(board, T, 17, PROMOTED_SILVER, SECOND);
    convert_piece_into_channel(board, T, 18, HORSE, SECOND);
    convert_piece_into_channel(board, T, 19, DRAGON, SECOND);

    convert_captured_into_channel(board, T, 20, PAWN, FIRST);
    convert_captured_into_channel(board, T, 22, PAWN, SECOND);
    convert_captured_into_channel(board, T, 24, SILVER, FIRST);
    convert_captured_into_channel(board, T, 26, SILVER, SECOND);
    convert_captured_into_channel(board, T, 28, GOLD, FIRST);
    convert_captured_into_channel(board, T, 30, GOLD, SECOND);
    convert_captured_into_channel(board, T, 32, BISHOP, FIRST);
    convert_captured_into_channel(board, T, 34, BISHOP, SECOND);
    convert_captured_into_channel(board, T, 36, ROOK, FIRST);
    convert_captured_into_channel(board, T, 38, ROOK, SECOND);


    return T;

}

