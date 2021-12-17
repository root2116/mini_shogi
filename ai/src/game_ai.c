
#include "../../game/include/Board.h"
#include "../../game/include/Referee.h"
#include "../../game/include/list.h"
#include "../../game/include/utility.h"

#include <stdio.h>
#include <stdlib.h>

char* random_ai(Referee ref, Board board, char* output){
    List next_actions = ref->legal_actions(ref,board,ref->get_turn(ref));

    
    int len = length(next_actions);

    int rand_num = rand() % len;

    for(int i = 0; i < len; i++){
        if(i == rand_num){
            convert_action_into_string(next_actions->action,output);
            break;
        }
        next_actions = next_actions->next;
    }


}

