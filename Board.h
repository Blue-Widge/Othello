//
// Created by bluew on 11/01/2022.
//
#ifndef OTHELLO_BOARD_H
#define OTHELLO_BOARD_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#ifdef WIN32
#define CLEAR_CONSOLE system("cls")
#define PAUSE_ONESEC system("timeout 1")
#include <windows.h>
#define FOREGROUND_WHITE (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY)
#define WHITE_PAWN_DARK SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),BACKGROUND_GREEN | FOREGROUND_WHITE)
#define WHITE_PAWN_LIGHT SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),BACKGROUND_GREEN | BACKGROUND_INTENSITY | FOREGROUND_WHITE)
#define BLACK_PAWN_DARK SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),BACKGROUND_GREEN)
#define BLACK_PAWN_LIGHT SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),BACKGROUND_GREEN | BACKGROUND_INTENSITY)
#define POSSIBLE_TILE_DARK SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),BACKGROUND_RED | FOREGROUND_RED)
#define EMPTY_TILE_DARK SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),BACKGROUND_GREEN | FOREGROUND_GREEN )
#define EMPTY_TILE_LIGHT SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),BACKGROUND_GREEN | BACKGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_INTENSITY)
#define RESET_TEXT SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7)
#else
#define CLEAR_CONSOLE system("clear")
#define PAUSE_ONESEC sleep(1)
#define WHITE_PAWN_DARK printf("\033[37m\033[0;47m")
#define WHITE_PAWN_LIGHT printf("\033[37m\033[1;47m")
#define BLACK_PAWN_DARK printf("\033[30m\033[1;40m")
#define BLACK_PAWN_LIGHT printf("\033[30m\033[1;40m")
#define POSSIBLE_TILE_DARK printf("\033[31m\033[1;41m")
#define POSSIBLE_TILE_LIGHT printf("\033[31m\033[1  ;41m")
#define EMPTY_TILE_DARK printf("\033[32m\033[1;42m")
#define EMPTY_TILE_LIGHT printf("\033[32m\033[1;42m")
#define RESET_TEXT printf("\033[0m");
#endif

#define NORMAL "\033[0;0m"
#define RED "\033[0;31m"
#define BLUE "\033[0;34m"
#define GREEN "\033[0;32m]"

enum Team_e
{
    BLANK = 0,
    BLACK = 1,
    WHITE = 2,
    PLAYABLE = 8
};

struct Board_s
{
    int m_width;
    int m_height;
    int** m_board;
    enum Team_e m_playingTeam;
    int m_remainingCases;
    enum Team_e m_winner;
    int m_nbPossibilites;
};

enum Direction_e
{
    RIGHT,
    UPRIGHT,
    UP,
    UPLEFT,
    LEFT,
    DOWNLEFT,
    DOWN,
    DOWNRIGHT
};

struct Board_s* createBoard(int p_width, int p_height);

struct Board_s* copyBoard(struct Board_s* p_board);

void printBoard(struct Board_s* p_board);

void placePiece(struct Board_s** p_board, char p_position[3]);

bool isPositionCorrect(struct Board_s* p_board, int positionX, int positionY, bool replace);

void returnPieces(struct Board_s** p_board, int positionX, int positionY, enum Direction_e p_direction, int number);

void destroyBoard(struct Board_s* p_board);

bool stillPlayable(struct Board_s** p_board);

int whiteCount(struct Board_s* p_board);

int blackCount(struct Board_s* p_board);

void clearBoardHints(struct Board_s** p_board);
#endif //OTHELLO_BOARD_H
