#include "AI.h"

int main()
{
    int width = 8, height = 8;
    struct Board_s* board = createBoard(width, height);
    char position[2] = {0};
    while(1)
    {
        clearBoardHints(&board);
        if (!stillPlayable(&board))
        {
            board->m_playingTeam = board->m_playingTeam == BLACK ? WHITE : BLACK;
            if (!stillPlayable(&board))
            {
                board->m_winner = whiteCount(board) > blackCount(board) ? WHITE : BLACK;
                break;
            }
        }
        printBoard(board);
        printf("Where will you place your pawn %s player ?\n", board->m_playingTeam == BLACK ? "black" : "white");
        scanf("%s", position);
        placePiece(&board, position);
        board->m_playingTeam = board->m_playingTeam == BLACK ? WHITE : BLACK;
        CLEAR_CONSOLE;
    }
    switch (board->m_winner)
    {
        case BLANK :
            printf("There is no more cases left. Nobody wins ! \n");
            break;
        case BLACK:
            printf("Black ate all white pawns, Black player wins ! \n");
            break;
        case WHITE:
            printf("White ate all black pawns, White player wins ! \n");
            break;
        default:
            printf("I dunno who wins\n");
            break;
    }
    destroyBoard(board);
}
