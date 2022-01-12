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
    if (!p_difficulty || !p_board->m_nbPossibilites)
    {

        (*p_minMax) = whiteCount(p_board) - blackCount(p_board);
        return NULL;
    }
    int width = p_board->m_width;
    int height = p_board->m_height;
    int remaining = p_board->m_nbPossibilites;

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
        destroyAI(AI);
        return NULL;
    }
    AI->m_board = (struct Board_s*) calloc(remaining, sizeof(struct Board_s*));
    if (!AI->m_board)
    {
        printf("ERROR while creating AI->m_board in createAI()\n");
        destroyAI(AI);
        return NULL;
    }

    return AI;
}

void destroyAI(struct AI_s* p_AI)
{
    int nbChildren = p_AI->m_possNumber;
    for(int i = 0; i < nbChildren; ++i)
    {
        destroyBoard((p_AI->m_children[i]->m_board));
        free(p_AI->m_children[i]);
    }
    destroyBoard(p_AI->m_board);
    free(p_AI->m_children);
    free(p_AI);
}