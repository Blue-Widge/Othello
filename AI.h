//
// Created by bluew on 11/01/2022.
//

#ifndef OTHELLO_AI_H
#define OTHELLO_AI_H
#include "Board.h"

struct AI_s
{
    int m_possNumber;
    int m_MinMax;
    struct AI_s** m_children;
    struct Board_s** m_board;
};

struct AI_s* createAI(struct Board_s* p_board, int p_difficulty, int* p_minMax);

void updateAI(struct Board_s* p_board, struct AI_s** p_AI);

void destroyAI(struct AI_s* p_AI);
#endif //OTHELLO_AI_H
