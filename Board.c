//
// Created by bluew on 11/01/2022.
//

#include "Board.h"

struct Board_s* createBoard(int p_width, int p_height, bool p_withBoard)
{
    struct Board_s* board = (struct Board_s*) calloc(1, sizeof(struct Board_s));
    if (p_width != p_height)
        p_height = p_width;
    if (p_width % 2 != 0)
        p_width++;
    if (p_width > 26)
        p_width = p_height = 26;
    board->m_width = p_width + 1 ;
    board->m_height = p_height + 1;

    board->m_remainingCases = p_width * p_height;

    int middle = p_width / 2;

    if(!p_withBoard)
    {
        board->m_board = NULL;
        return board;
    }

    int** boardMatrix = (int**) calloc(board->m_width, sizeof(int*));
    for(int i = 0; i < board->m_width; ++i)
        boardMatrix[i] = (int*) calloc(board->m_height, sizeof(int));
    board->m_board = boardMatrix;

    board->m_board[0][1] = 'A';
    for(int i = 2; i < board->m_width; ++i)
        board->m_board[0][i] = 'A' - 1 + i;
    for(int i = 1; i < board->m_height; ++i)
        board->m_board[i][0] = i;
    board->m_board[middle][middle] = board->m_board[middle + 1][middle + 1] = WHITE;
    board->m_board[middle][middle + 1] = board->m_board[middle + 1][middle] = BLACK;

    board->m_playingTeam = BLACK;
    return board;
}

struct Board_s* copyBoard(struct Board_s* p_board)
{
    int width = p_board->m_width;
    int height = p_board->m_height;

    struct Board_s* copy = createBoard(width - 1, height - 1, false);
    int** copyMatrix = (int**) calloc(copy->m_width, sizeof(int*));
    for(int i = 0; i < copy->m_width; ++i)
        copyMatrix[i] = (int*) calloc(copy->m_height, sizeof(int));
    int** boardMatrix = p_board->m_board;
    for(int i = 0; i < width; ++i)
    {
        for(int j = 0; j < height; ++j)
            copyMatrix[j][i] = boardMatrix[j][i];
    }
    copy->m_board = copyMatrix;
    copy->m_playingTeam = p_board->m_playingTeam;
    copy->m_remainingCases = p_board->m_remainingCases;
    return copy;
}

void printBoard(struct Board_s* p_board)
{
    int width = p_board->m_width, height = p_board->m_height;
    printf(NORMAL);
    printf("  ");
    for(int i = 0; i < width; ++i)
        printf("%c ", p_board->m_board[0][i]);
    printf("\n");
    for(int i = 1; i < width; ++i)
    {
        if (i > 9)
            printf("%d ", i);
        else
            printf("%d  ", i);
        for(int j = 1; j < height; ++j)
        {
            int toPrint = p_board->m_board[i][j];
                switch (toPrint)
                {
                    case BLANK :
                        (i + j) % 2 == 0 ? EMPTY_TILE_DARK : EMPTY_TILE_LIGHT;
                        break;
                    case BLACK :
                        (i + j) % 2 == 0 ? BLACK_PAWN_DARK : BLACK_PAWN_LIGHT;
                        break;
                    case WHITE :
                        (i + j) % 2 == 0 ? WHITE_PAWN_DARK : WHITE_PAWN_LIGHT;
                        break;
                    case PLAYABLE :
                        POSSIBLE_TILE_DARK;
                        break;
            }
            printf("%d ", toPrint);
        }
        RESET_TEXT;
        printf("\n");

        printf(NORMAL);
    }
}

void placePiece(struct Board_s** p_board, char p_position[3])
{
    int positionX = p_position[0] - 'A' + 1;
    int positionY = p_position[1] - '0';
    while (positionX < 1 || positionX > (*p_board)->m_width - 1 || positionY < 1 || positionY > (*p_board)->m_height - 1)
    {
        CLEAR_CONSOLE;
        printBoard((*p_board));
        printf("Please enter in the correct format %s player : [majLetter][number]\n", (*p_board)->m_playingTeam == BLACK ? "black" : "white");
        scanf("%s", p_position);
        positionY = p_position[1] - '0';
        positionX = p_position[0] - 'A' + 1;
    }
    while (!isPositionCorrect((*p_board), positionX, positionY, true))
    {
        CLEAR_CONSOLE;
        printBoard((*p_board));
        printf("Please enter a correct position %s player [majLetter][number]\n", (*p_board)->m_playingTeam == BLACK ? "black" : "white" );
        scanf("%s", p_position);
        positionY = p_position[1] - '0';
        positionX = p_position[0] - 'A' + 1;
    }
    (*p_board)->m_board[positionY][positionX] = (*p_board)->m_playingTeam;
    (*p_board)->m_remainingCases--;
}


bool isPositionCorrect(struct Board_s* p_board, int positionX, int positionY, bool replace)
{
    bool correct = false;

    int** board = p_board->m_board;
    int width = p_board->m_width;
    int height = p_board->m_height;
    enum Team_e playingTeam = p_board->m_playingTeam;
    enum Team_e oppositeTeam = playingTeam == BLACK ? WHITE : BLACK;


    if (board[positionY][positionX] != BLANK && board[positionY][positionX] != PLAYABLE)
        return false;

    int index = 1;
    if (positionX + 1 < width && board[positionY][positionX + 1] == oppositeTeam)
    {
        for(index = 1; (positionX+1+index < width && board[positionY][positionX+1+index] != playingTeam); ++index)
        {
            if(board[positionY][positionX+1+index] == BLANK || board[positionY][positionX+1+index] == PLAYABLE)
                index = width;
        }
        if (positionX + 1 + index < width)
        {
            correct = true;
            if (replace) returnPieces(&p_board, positionX+1, positionY, RIGHT,  index);
        }
    }
    if (positionX + 1 < width && positionY - 1 > 0 && board[positionY - 1][positionX + 1] == oppositeTeam)
    {
        for(index = 1; ( positionX+1+index < width && positionY-1-index > 0 && board[positionY-1-index][positionX+1+index] != playingTeam); ++index)
        {
            if(board[positionY-1-index][positionX+1+index] == BLANK || board[positionY-1-index][positionX+1+index] == PLAYABLE)
                index = width;
        }
        if (positionX + 1 + index < width && positionY - index - 1 > 0)
        {
            correct = true;
            if (replace) returnPieces(&p_board, positionX + 1, positionY - 1, UPRIGHT,  index);
        }
    }
    if (positionY - 1 > 0 && board[positionY - 1][positionX] == oppositeTeam)
    {
        for(index = 1; (positionY-1-index > 0 && board[positionY-1-index][positionX] != playingTeam); ++index)
        {
            if(board[positionY-1-index][positionX] == BLANK || board[positionY-1-index][positionX] == PLAYABLE)
                index = width;
        }
        if (positionY - index - 1 > 0)
        {
            correct = true;
            if (replace) returnPieces(&p_board, positionX, positionY - 1, UP,  index);
        }
    }
    if (positionX - 1 > 0 && positionY - 1 > 0 && board[positionY - 1][positionX - 1] == oppositeTeam)
    {
        for(index = 1; (positionX-1-index > 0 && positionY-1-index > 0 && board[positionY-1-index][positionX-1-index] != playingTeam) ; ++index)
        {
            if(board[positionY-1-index][positionX-1-index] == BLANK || board[positionY-1-index][positionX-1-index] == PLAYABLE)
                index = width;
        }
        if (positionX - 1 - index > 0 && positionY - index - 1 > 0)
        {
            correct = true;
            if (replace) returnPieces(&p_board, positionX - 1, positionY - 1, UPLEFT,  index);
        }
    }
    if (positionX - 1> 0 && board[positionY][positionX - 1] == oppositeTeam)
    {
        for(index = 1; (positionX-1-index > 0 && board[positionY][positionX-1-index] != playingTeam) ; ++index)
        {
            if(board[positionY][positionX-1-index] == BLANK || board[positionY][positionX-1-index] == PLAYABLE)
                index = width;
        }
        if (positionX - index - 1 > 0)
        {
            correct = true;
            if (replace) returnPieces(&p_board, positionX - 1, positionY, LEFT,  index);
        }
    }
    if (positionX - 1> 0 && positionY + 1 < height && board[positionY + 1][positionX - 1] == oppositeTeam)
    {
        for(index = 1; (positionX-1-index > 0 && positionY+1+index < height && board[positionY+1+index][positionX-1-index] != playingTeam); ++index)
        {
            if(board[positionY + 1][positionX - 1] == BLANK || board[positionY + 1][positionX - 1] == PLAYABLE)
                index = width;
        }
        if (positionX - index - 1 > 0 && positionY + index + 1 < height)
        {
            correct = true;
            if (replace) returnPieces(&p_board, positionX - 1, positionY + 1, DOWNLEFT,  index);
        }
    }
    if (positionY + 1 < height && board[positionY + 1][positionX] == oppositeTeam)
    {
        for(index = 1; (positionY+1+index < height && board[positionY+1+index][positionX] != playingTeam); ++index)
        {
            if(board[positionY+1+index][positionX] == BLANK || board[positionY+1+index][positionX] == PLAYABLE)
                index = width;
        }
        if (positionY + index + 1 < height)
        {
            correct = true;
            if (replace) returnPieces(&p_board, positionX , positionY + 1, DOWN,  index);
        }
    }
    if (positionX + 1 < width && positionY + 1 < height && board[positionY + 1][positionX + 1] == oppositeTeam)
    {
        for(index = 1; (positionX+1+index < width && positionY+1+index < height && board[positionY+1+index][positionX+1+index] != playingTeam); ++index)
        {
            if(board[positionY + 1][positionX + 1] == BLANK || board[positionY + 1][positionX + 1] == PLAYABLE)
                index = width;
        }
        if (positionX + 1 + index < width && positionY + index + 1 < height)
        {
            correct = true;
            if (replace) returnPieces(&p_board, positionX + 1, positionY + 1, DOWNRIGHT,  index);
        }
    }
    if(correct)
        p_board->m_board[positionY][positionX] = PLAYABLE;
    return correct;
}

void returnPieces(struct Board_s** p_board, int positionX, int positionY, enum Direction_e p_direction, int number)
{
    enum Team_e finalTeam = (*p_board)->m_playingTeam == BLACK ? BLACK : WHITE;

    if (p_direction == RIGHT)
    {
        for(int i = 0; i < number; ++i)
        {
            (*p_board)->m_board[positionY][positionX + i] = finalTeam;
        }
        return;
    }
    else if (p_direction == UPRIGHT)
    {
        for(int i = 0; i < number; ++i)
        {
            (*p_board)->m_board[positionY - i][positionX + i] = finalTeam;
        }
        return;
    }
    else if (p_direction == UP)
    {
        for (int i = 0; i < number; ++i)
        {
            (*p_board)->m_board[positionY - i][positionX] = finalTeam;
        }
        return;
    }
    else if (p_direction == UPLEFT)
    {
        for (int i = 0; i < number; ++i)
        {
            (*p_board)->m_board[positionY - i][positionX - i] = finalTeam;
        }
        return;
    }
    else if (p_direction == LEFT)
    {
        for (int i = 0; i < number; ++i)
        {
            (*p_board)->m_board[positionY][positionX - i] = finalTeam;
        }
        return;
    }
    else if (p_direction == DOWNLEFT)
    {
        for (int i = 0; i < number; ++i)
        {
            (*p_board)->m_board[positionY + i][positionX - i] = finalTeam;
        }
        return;
    }
    else if (p_direction == DOWN)
    {
        for (int i = 0; i < number; ++i)
        {
            (*p_board)->m_board[positionY + i][positionX] = finalTeam;
        }
        return;
    }
    else if (p_direction == DOWNRIGHT)
    {
        for (int i = 0; i < number; ++i)
        {
            (*p_board)->m_board[positionY + i][positionX + i] = finalTeam;
        }
    }
}

void destroyBoard(struct Board_s** p_board)
{
    if (!p_board || !(*p_board))
        return;
    int width = (*p_board)->m_width;
    for(int i = 0; i < width; ++i)
    {
        free((*p_board)->m_board[i]);
        (*p_board)->m_board[i] = NULL;
    }
    free((*p_board)->m_board);
    (*p_board)->m_board = NULL;
    free((*p_board));
    (*p_board) = NULL;
}

bool stillPlayable(struct Board_s** p_board)
{
    int width = (*p_board)->m_width;
    int height = (*p_board)->m_height;
    bool possible = false;
    for(int i = 1; i < width; ++i)
    {
        for(int j = 1; j < height; ++j)
        {
            if(isPositionCorrect((*p_board), j, i, false))
            {
                possible = true;
                (*p_board)->m_nbPossibilites++;
            }
        }
    }
    return possible;
}

int whiteCount(struct Board_s* p_board)
{
    if (!p_board)
    {
        printf("ERROR in whiteCount() p_board NULL\n");
        //PAUSE_ONESEC; PAUSE_ONESEC;
    }
    int width = p_board->m_width;
    int height = p_board->m_height;
    int** matrix = p_board->m_board;
    int count = 0;
    for(int i = 1; i < width; ++i)
    {
        for(int j = 1; j < height; ++j)
        {
            if(matrix[j][i] == WHITE)
                count++;
        }
    }
    return count;
}

int blackCount(struct Board_s* p_board)
{
    if (!p_board)
    {
        printf("ERROR in blackCount() p_board NULL\n");
        PAUSE_ONESEC; PAUSE_ONESEC;
    }
    int width = p_board->m_width;
    int height = p_board->m_height;
    int** matrix = p_board->m_board;
    int count = 0;
    for(int i = 1; i < width; ++i)
    {
        for(int j = 1; j < height; ++j)
        {
            if(matrix[j][i] == BLACK)
                count++;
        }
    }
    return count;
}


void clearBoardHints(struct Board_s** p_board)
{
    int width = (*p_board)->m_width;
    int height = (*p_board)->m_height;
    (*p_board)->m_nbPossibilites = 0;

    for(int i = 1; i < width; ++i)
    {
        for(int j = 1; j < height; ++j)
            (*p_board)->m_board[j][i] = (*p_board)->m_board[j][i] == PLAYABLE ? 0 : (*p_board)->m_board[j][i];
    }
}
