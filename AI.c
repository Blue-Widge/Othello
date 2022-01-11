//
// Created by bluew on 11/01/2022.
//

#include "AI.h"

struct AI_s* createAI(struct Board_s* p_board, int p_difficulty, int* p_minMax)
{
    if (!p_difficulty)
    {
        // remonter les infos avec minMax;
    }
    int width = p_board->m_width;
    int height = p_board->m_height;
    int remaining = p_board->m_nbPossibilites;

    struct AI_s* AI = (struct AI_s*) calloc(1, sizeof(struct AI_s));
    AI->m_children = (struct AI_s**) calloc(remaining, sizeof(struct AI_s*));
    AI->m_board = (struct Board_s**) calloc(remaining, sizeof(struct Board_s*));
    for(int i = 0; i < remaining; ++i)
        AI->m_board[i] = copyBoard(p_board);

    int** copyBoard = p_board->m_board;
    char position[2] = "\0\0";

    for(int i = 1; i < width; ++i)
    {
        for(int j = 1; j < height; ++j)
        {
            if (copyBoard[j][i] == PLAYABLE)
            {
                remaining--;
                position[0] = i + 'A' - 1;
                position[1] = j + '0';

                placePiece(&(AI->m_board[remaining]), position);
                AI->m_children[remaining] = createAI(AI->m_board[remaining], p_difficulty - 1, p_minMax);
            }
        }
    }
    return AI;
}

void destroyAI(struct AI_s* p_AI)
{
    int nbChildren = p_AI->m_possNumber;
    for(int i = 0; i < nbChildren; ++i)
        free(p_AI->m_children[i]);
    free(p_AI->m_children);
    free(p_AI);
}