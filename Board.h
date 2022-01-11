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
#else
#define CLEAR_CONSOLE system("clear")
#endif

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

void printBoard(struct Board_s* p_board);

void placePiece(struct Board_s** p_board, char p_position[2]);

bool isPositionCorrect(struct Board_s* p_board, int positionX, int positionY, bool replace);

void returnPieces(struct Board_s** p_board, int positionX, int positionY, enum Direction_e p_direction, int number);

void destroyBoard(struct Board_s* p_board);

bool stillPlayable(struct Board_s* p_board);

int whiteCount(struct Board_s* p_board);

int blackCount(struct Board_s* p_board);

void clearBoardHints(struct Board_s** p_board);
#endif //OTHELLO_BOARD_H
