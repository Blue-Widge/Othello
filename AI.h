//
// Created by bluew on 11/01/2022.
//

#ifndef OTHELLO_AI_H
#define OTHELLO_AI_H
#include "Board.h"

struct AI_s
{
    int m_possNumber;
    int MinMax;
    struct AI_s** m_children;
};

void updateAI(struct Board_s* p_board, struct AI_s** p_AI);

#endif //OTHELLO_AI_H
