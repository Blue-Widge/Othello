//
// Created by bluew on 11/01/2022.
//

#ifndef OTHELLO_AI_H
#define OTHELLO_AI_H
#include "Board.h"

struct AI_s
{
    int m_possNumber;
    int m_minMax;
    char m_position[3];
    struct AI_s** m_children;
    struct Board_s* m_AIboard;
    int m_alpha;
    int m_beta;
    bool m_leaf;
};


struct AI_s* createAI(struct Board_s* p_board, int p_difficulty, int* p_minMax, enum Team_e playingTeam, int p_alpha, int p_beta, const enum Team_e originalTeam);

int getPositionPoints(int p_width, int p_line, int p_row);

void destroyAI(struct AI_s** p_AI);
#endif //OTHELLO_AI_H
