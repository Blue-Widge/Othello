#include "AI.h"
int main()
{
#if 1
    system(" ");
    int width = 8, height = 8;
    int difficulty = 3;
    struct Board_s* board = createBoard(width, height, true);
    char* userEntry = (char*) calloc(128, sizeof(char));
    char position[3] = {0};
    while(1)
    {
        clearBoardHints(&board);
        if (!stillPlayable(&board))
        {
            board->m_playingTeam = board->m_playingTeam == BLACK ? WHITE : BLACK;
            if (!stillPlayable(&board))
            {
                //PAUSE_ONESEC;
                board->m_winner = whiteCount(board) > blackCount(board) ? WHITE : BLACK;
                break;
            }
        }
        printBoard(board);
        printf("Where will you place your pawn %s player ?\n", board->m_playingTeam == BLACK ?  "black"  :  "white" );
        if (//board->m_playingTeam == WHITE
    1)
        {
            struct Board_s* copy = copyBoard(board);
            if (board->m_nbPossibilites == 1)
            {
                for (int i = 1; i < board->m_width; ++i)
                {
                    for (int j = 1; j < board->m_height; ++j)
                    {
                        if (board->m_board[j][i] == PLAYABLE)
                        {
                            position[1] = j + '0';
                            position[0] = i + 'A' - 1;
                            i = board->m_width;
                            j = board->m_height;
                        }
                    }
                }
            }
            else
            {
                int minMax = 0;
                struct AI_s* AI = createAI(copy, difficulty, &minMax, board->m_playingTeam);
                for (int i = 0; i < board->m_nbPossibilites; ++i)
                {
                    if (AI->m_children[i] && AI->m_MinMax == AI->m_children[i]->m_MinMax)
                    {
                        position[0] = AI->m_children[i]->m_position[0];
                        position[1] = AI->m_children[i]->m_position[1];
                    }
                }
                destroyAI(&AI);
            }
        }
        else
        {
            scanf("%s", userEntry);
            position[0] = userEntry[0];
            position[1] = userEntry[1];
        }
        placePiece(&board, position);
        board->m_playingTeam = board->m_playingTeam == BLACK ? WHITE : BLACK;
        CLEAR_CONSOLE;
    }
    printBoard(board);
    switch (board->m_winner)
    {
        case BLANK :
            printf("There is no more cases left. Nobody wins ! \n");
            break;
        case BLACK:
            printf("Black controls the most part of the board, black player wins ! \n");
            break;
        case WHITE:
            printf("White controls the most part of the board, white player wins ! \n");
            break;
        default:
            printf("I dunno who wins\n");
            break;
    }
    destroyBoard(&board);
    free(userEntry);
#else
    struct Board_s* board = createBoard(8,8,true);
    struct Board_s* copy = copyBoard(board);
    stillPlayable(&board);
    printBoard(board);
    int oui = 0;
    struct AI_s* AI = createAI(copy, 3, &oui, WHITE);
    destroyAI(&AI);
    destroyBoard(&board);
#endif
}