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

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

void genenrate_data(char* file_name1, char* file_name2, int num){
    

    for(int i = 0; i < num; i++ ){
        Game game = new_game(FIRST);
        int result = game->cpu_vs_cpu(game, random_ai, random_ai, false);

        if(result == 0) continue;


        FILE *fp1 = fopen(file_name1,"a");
        save_data(fp1,game);
        FILE *fp2 = fopen(file_name2, "a");

        if(result == 1){
            fprintf(fp2, "%d\n", 1);
        }else{
            fprintf(fp2,"%d\n",0);
        }
        
        fclose(fp1);
        fclose(fp2);
        game->free_game(game);
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
    for(int i = 0; i < 2; i++){
        for(int j = 0; j < 10;j++){
            Piece piece = board->captured_pieces[i][j];
            if(piece == NULL) continue;

            if(piece->kind == kind && piece->side == side){
                count++;
                
            }
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
            char piece_str = board_str[25 + i * 2 + j];

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
