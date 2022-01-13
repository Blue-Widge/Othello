//
// Created by bluew on 11/01/2022.
//

#include "AI.h"
struct AI_s* createAI(struct Board_s* p_board, int p_difficulty, int* p_minMax, enum Team_e playingTeam)
{
    if (!p_board)
    {
        printf("ERROR in createAI() : p_board is NULL\n");
        return NULL;
    }
    clearBoardHints(&p_board);

    if (!p_difficulty || !stillPlayable(&p_board))
    {
        (*p_minMax) = whiteCount(p_board) - blackCount(p_board);
        return NULL;
    }
    int width = p_board->m_width;
    int height = p_board->m_height;
    int remaining = p_board->m_nbPossibilites;
    enum Team_e oppositeTeam = playingTeam == BLACK ? WHITE : BLACK;

    struct AI_s* AI = (struct AI_s*) calloc(1, sizeof(struct AI_s));
    if (!AI)
    {
        printf("ERROR while creating AI in createAI()\n");
        return NULL;
    }
    AI->m_children = (struct AI_s**) calloc(remaining, sizeof(struct AI_s*));
    if (!AI->m_children)
    {
        printf("ERROR while creating AI->m_children in createAI()\n");
        destroyAI(&AI);
        return NULL;
    }
    AI->m_possNumber = p_board->m_nbPossibilites;
    AI->m_AIboard = p_board;
    char position[3] = {0};
    struct Board_s* copy = NULL;
    for(int i = 1; i < width; ++i)
    {
        for(int j = 1; j < height; ++j)
        {
            if (AI->m_AIboard->m_board[j][i] == PLAYABLE)
            {
                remaining--;
                position[1] = j + '0';
                position[0] = i + 'A' - 1;
                copy = copyBoard(p_board);
                if (!copy)
                {
                    printf("ERROR while creating copy in createAI() (remaining : %d\n", remaining);
                    destroyBoard(&copy);
                    destroyAI(&AI);
                    return NULL;
                }
                placePiece(&copy, position);
                AI->m_children[remaining] = createAI(copy, p_difficulty - 1, p_minMax, oppositeTeam);
                if (!AI->m_children[remaining])
                {
                    AI->m_MinMax = *p_minMax;
                    destroyBoard(&copy);
                }
                else
                {
                    AI->m_children[remaining]->m_position[0] = position[0];
                    AI->m_children[remaining]->m_position[1] = position[1];
                }
            }
        }
    }
    if (AI->m_children[0])
    {
        AI->m_MinMax = AI->m_children[0]->m_MinMax;
        for(int i = 1; i < AI->m_possNumber; ++i)
        {
            if (AI->m_children[i])
            {
                int AIminMax = AI->m_children[i]->m_MinMax;
                if (playingTeam == WHITE)
                {
                    AI->m_MinMax = AIminMax > AI->m_MinMax ? AIminMax : AI->m_MinMax;
                } else
                {
                    AI->m_MinMax = AIminMax < AI->m_MinMax ? AIminMax : AI->m_MinMax;
                }
            }
        }
    }
    return AI;
}

void destroyAI(struct AI_s** p_AI)
{
    if (!p_AI || !(*p_AI))
        return;
    int nbChildren = (*p_AI)->m_possNumber;
    for(int i = 0; i < nbChildren; ++i)
    {
        if ((*p_AI)->m_children[i] && (*p_AI)->m_children[i]->m_AIboard)
        {
            destroyBoard(&((*p_AI)->m_children[i]->m_AIboard));
            (*p_AI)->m_children[i]->m_AIboard = NULL;
        }
        destroyAI(&((*p_AI)->m_children[i]));
        (*p_AI)->m_children[i] = NULL;
    }
    if ((*p_AI)->m_AIboard)
    {
        destroyBoard(&(*p_AI)->m_AIboard);
        (*p_AI)->m_AIboard = NULL;
    }
    free((*p_AI)->m_children);
    (*p_AI)->m_children = NULL;
    free((*p_AI));
    (*p_AI) = NULL;
}