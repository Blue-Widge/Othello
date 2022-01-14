//
// Created by bluew on 11/01/2022.
//

#include "AI.h"

int getPositionPoints(int p_width, int p_line, int p_row);

struct AI_s* createAI(struct Board_s* p_board, int p_difficulty, int* p_minMax, enum Team_e playingTeam, int p_alpha, int p_beta, const enum Team_e originalTeam)
{
    if (!p_board)
    {
        printf("ERROR in createAI() : p_board is NULL\n");
        return NULL;
    }
    clearBoardHints(&p_board);

    struct AI_s* AI = (struct AI_s*) calloc(1, sizeof(struct AI_s));
    if (!AI)
    {
        printf("ERROR while creating AI in createAI()\n");
        return NULL;
    }
// If the tree is on the bottom or there isn't any possible move after return an empty child with the minMax value
    if (!p_difficulty || !stillPlayable(&p_board))
    {
        int black = blackCount(p_board), white = whiteCount(p_board);
        (*p_minMax) = originalTeam == WHITE ? (white * white) - (black * black) : (black * black)  - (white * white);
        AI->m_minMax = (*p_minMax);
        AI->m_leaf = true;
        AI->m_children = NULL;
        AI->m_AIboard = NULL;
        return AI;
    }
    int width = p_board->m_width;
    int height = p_board->m_height;
    int remaining = p_board->m_nbPossibilites;
    enum Team_e oppositeTeam = playingTeam == BLACK ? WHITE : BLACK;

    //Initialize the children
    AI->m_children = (struct AI_s**) calloc(remaining, sizeof(struct AI_s*));
    if (!AI->m_children)
    {
        printf("ERROR while creating AI->m_children in createAI()\n");
        destroyAI(&AI);
        return NULL;
    }
    AI->m_possNumber = p_board->m_nbPossibilites;
    AI->m_AIboard = p_board;
    AI->m_alpha = p_alpha;
    AI->m_beta = p_beta;
    char position[3] = {0};
    struct Board_s* copy = NULL;

    //Search through the board, if a position is possible, create an AI based on what if we played here
    for(int i = 1; i < width; ++i)
    {
        for(int j = 1; j < height; ++j)
        {
            if (AI->m_AIboard->m_board[j][i] == PLAYABLE)
            {
                remaining--;
                AI->m_children[remaining] = NULL;
                if (AI->m_alpha < AI->m_beta)
                {
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
                    AI->m_children[remaining] = createAI(copy, p_difficulty - 1, p_minMax, oppositeTeam, AI->m_alpha, AI->m_beta, originalTeam);

                    // if the child is a leaf, we set its position and calculate the minMax
                    if (AI->m_children[remaining]->m_leaf)
                    {
                        AI->m_children[remaining]->m_position[0] = position[0];
                        AI->m_children[remaining]->m_position[1] = position[1];

                        //Value of the position on the board : the most on the exterior the better
                        int positionPoints = getPositionPoints(width, i, j);
                        (*p_minMax) += positionPoints * positionPoints * positionPoints;

                        // if it's the original AI team, set max, else set min
                        if (playingTeam == originalTeam)
                        {
                            if (AI->m_alpha <= (*p_minMax))
                            {
                                AI->m_alpha = (*p_minMax);
                                AI->m_position[0] = position[0];
                                AI->m_position[1] = position[1];
                            }
                            AI->m_minMax = AI->m_alpha;
                        }
                        else
                        {
                            if (AI->m_beta >= (*p_minMax))
                            {
                                AI->m_beta = *p_minMax;
                                AI->m_position[0] = position[0];
                                AI->m_position[1] = position[1];
                            }
                            AI->m_minMax = AI->m_beta;
                        }
                        destroyBoard(&copy);
                    }
                    //if it's not a leaf, set the child position and get the alpha if max, beta if min
                    else
                    {
                        AI->m_children[remaining]->m_position[0] = position[0];
                        AI->m_children[remaining]->m_position[1] = position[1];
                        if (playingTeam == originalTeam)
                        {
                            int childBeta = AI->m_children[remaining]->m_beta;
                            AI->m_minMax = AI->m_alpha = AI->m_alpha < childBeta ? childBeta : AI->m_alpha;
                        }
                        else
                        {
                            int childAlpha = AI->m_children[remaining]->m_alpha;
                            AI->m_minMax = AI->m_beta = AI->m_beta > childAlpha ? childAlpha : AI->m_beta;
                        }
                    }
                }
            }
        }
    }
    // for each none leaf, compare the min max of the children and put it in AI->m_minMax -> max if originalTeam, min otherwise

    if (!AI->m_leaf && AI->m_children[0])
    {
        AI->m_minMax = AI->m_children[0]->m_minMax;
        AI->m_position[0] = AI->m_children[0]->m_position[0];
        AI->m_position[1] = AI->m_children[0]->m_position[1];

        for (int i = 1; i < AI->m_possNumber; ++i)
        {
            if (AI->m_children[i])
            {
                int AIminMax = AI->m_children[i]->m_minMax;
                if (playingTeam == originalTeam)
                {
                    if (AIminMax >= AI->m_minMax)
                    {
                        AI->m_minMax = AIminMax;
                        AI->m_position[0] = AI->m_children[i]->m_position[0];
                        AI->m_position[1] = AI->m_children[i]->m_position[1];
                    }
                } else
                {
                    if (AIminMax <= AI->m_minMax)
                    {
                        AI->m_minMax = AIminMax;
                        AI->m_position[0] = AI->m_children[i]->m_position[0];
                        AI->m_position[1] = AI->m_children[i]->m_position[1];
                    }
                }
            }
        }
    }
    //return the AI address
    return AI;
}

int getPositionPoints(int p_width, int p_line, int p_row)
{
    int middle = p_width / 2;
    if (p_line <= middle)
    {
        if (p_row <= middle)
            return middle - p_line - p_row;
        else
            return middle - p_line + p_row - (p_width + 1) / 2 ;
    }
    else
    {
        if (p_row <= middle)
            return p_line - (p_width + 1) / 2 + middle - p_row;
        else
            return p_line + p_row - p_width - 1;

    }
}

void destroyAI(struct AI_s** p_AI)
{
    if (!p_AI || !(*p_AI))
        return;
    int nbChildren = (*p_AI)->m_possNumber;
    for(int i = 0; i < nbChildren; ++i)
    {
        if ((*p_AI)->m_children[i] && (*p_AI)->m_children[i]->m_AIboard && !(*p_AI)->m_children[i]->m_leaf)
        {
            destroyBoard(&((*p_AI)->m_children[i]->m_AIboard));
            (*p_AI)->m_children[i]->m_AIboard = NULL;
        }
        destroyAI(&((*p_AI)->m_children[i]));
        (*p_AI)->m_children[i] = NULL;
    }
    if ((*p_AI)->m_AIboard && !(*p_AI)->m_leaf)
    {
        destroyBoard(&(*p_AI)->m_AIboard);
        (*p_AI)->m_AIboard = NULL;
    }
    free((*p_AI)->m_children);
    (*p_AI)->m_children = NULL;
    free((*p_AI));
    (*p_AI) = NULL;
}