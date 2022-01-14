#include "AI.h"

int main()
{
    //Setup the game cmd and variables
    system(" ");
    CLEAR_CONSOLE;
    int width = 8, height = 8;
    int difficultyAI1 = 3, difficultyAI2 = 3;
    struct Board_s* board = createBoard(width, height, true);
    char* userEntry = (char*) calloc(128, sizeof(char));
    char position[3] = {0};
    enum Team_e AITeam = BLANK;
    enum GameChoice_e gameChoice;
    bool fastForward = false;

    printf("Hello user, please choose how would you like to play the game : \n"
           "1 - Player VS Player\n"
           "2 - Player VS AI\n"
           "3 - AI VS AI\n");
    scanf("%128s",userEntry);
    while(userEntry[0] < '1' || userEntry[0] > '3')
    {
        CLEAR_CONSOLE;
        printf("Please enter the correct number between 1 and 3 :\n"
               "1 - Player VS Player\n"
               "2 - Player VS AI\n"
               "3 - AI VS AI");
        scanf("%128s",userEntry);
    }
    if (userEntry[0] == '1')
        gameChoice = PlayerVSPlayer;
    else if (userEntry[0] == '2')
    {
        gameChoice = PlayerVSAI;
        CLEAR_CONSOLE;
        printf("Do you want to be (1) BLACK or (2) WHITE ? \n");
        scanf("%128s", userEntry);
        while(userEntry[0] < '1' || userEntry[0] > '2')
        {
            CLEAR_CONSOLE;
            printf("Please enter the correct number between 1 and 2 :\n"
                   "Do you want to be (1) BLACK or (2) WHITE ? \n");
            scanf("%128s",userEntry);
        }
        AITeam = userEntry[0] == '1' ? WHITE : BLACK;
        printf("Which difficulty would you like ? [1 - 9] (from easy to very difficult\n");
        scanf("%128s", userEntry);
        while(userEntry[0] < '1' || userEntry[0] > '9')
        {
            CLEAR_CONSOLE;
            printf("Please enter the correct number between 1 and 9 :\n"
                   "Which difficulty would you like ? [1 - 9] (from easy to very difficult\n");
            scanf("%128s",userEntry);
        }
        difficultyAI1 = userEntry[0] - '0';
    }
    else
    {
        gameChoice = AIVSAI;
        CLEAR_CONSOLE;
        printf("Which difficulty would you like for the Black AI ? [1 - 9] \n");
        scanf("%128s",userEntry);
        while(userEntry[0] < '1' || userEntry[0] > '9')
        {
            CLEAR_CONSOLE;
            printf("Please enter the correct number between 1 and 9 :\n"
                   "Which difficulty would you like for the Black AI ? [1 - 9] (from easy to very difficult)\n");
            scanf("%128s",userEntry);
        }
        difficultyAI1 = userEntry[0] - '0';

        CLEAR_CONSOLE;
        printf("Which difficulty would you like for the White AI ? [1 - 9] \n");
        scanf("%128s",userEntry);
        while(userEntry[0] < '1' || userEntry[0] > '9')
        {
            CLEAR_CONSOLE;
            printf("Please enter the correct number between 1 and 9 :\n"
                   "Which difficulty would you like for the White AI ? [1 - 9] (from easy to very difficult)\n");
            scanf("%128s",userEntry);
        }
        difficultyAI2 = userEntry[0] - '0';

        CLEAR_CONSOLE;
        printf("Fast forward ? (Yes - 1) (No - 0)\n");
        scanf("%128s", userEntry);
        while(userEntry[0] != '1' && userEntry[0] != '0')
        {
            CLEAR_CONSOLE;
            printf("Please enter the correct number between 0 and 1 :\n"
                   "Fast forward ? (Yes - 1) (No - 0)\n");
            scanf("%128s",userEntry);
        }
        fastForward = userEntry[0] - '0';
    }

    //While the game is still up
    while(1)
    {
        clearBoardHints(&board);
        //if the current player can't play and the other neither, the winner is the one with the most pawns
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
        printf("Where will you place your pawn %s player ?\n", board->m_playingTeam == BLACK ?  "black"  :  "white" );

        // If the player is the AI, activate it only if there is more than 1 possibility
        if (gameChoice == AIVSAI || board->m_playingTeam == AITeam)
        {
            //if there is only 1 possibility, search on the position on the board and place the pawn
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
                //if there is more than 1 possibiliy, create an AI who creates itself recursively by Alpha-Beta Prunning the best possibility
                //A copy of the board is needed so the AI doesn't modify the whole game
                int minMax = 0;
                position[0] = position[1] = 0;
                if (!fastForward)   PAUSE_ONESEC;
                struct Board_s* copy = copyBoard(board);
                struct AI_s* AI = NULL;
                if (board->m_playingTeam == BLACK)
                     AI = createAI(copy, difficultyAI1, &minMax, board->m_playingTeam, MINUS_INFINITY, INFINITY,
                                           board->m_playingTeam);
                else
                     AI = createAI(copy, difficultyAI2, &minMax, board->m_playingTeam, MINUS_INFINITY, INFINITY,
                                               board->m_playingTeam);

                // Once the AI calculated the best possibility, get the position and destroy it
                position[0] = AI->m_position[0];
                position[1] = AI->m_position[1];
                destroyAI(&AI);
            }
        }
        else
        {
            //Ask for the position and send try to place it
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
    return 0;
}