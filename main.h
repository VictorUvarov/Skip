#ifndef SKIP_MAIN_H
#define SKIP_MAIN_H

#include <cstdlib>
#include <cstdio>
#include <string>
#include <iostream>

const int COMPUTER = 2;
const int PLAYER = 1;
const int MAX_MOVES = 50;

const int UNDO = 1;
const int DONT_UNDO = 0;

const int REASON_NO_MOVES_LEFT = 0;
const int REASON_NO_KINGS_LEFT = 1;

const int KING_MAX = 4;
const int QUEEN_MAX = 3;
const int KNIGHT_MAX = 2;
const int BISHOP_MAX = 1;
const int KING_MIN = -4;
const int QUEEN_MIN = -3;
const int KNIGHT_MIN = -2;
const int BISHOP_MIN = -1;

int PLAYER_KINGS;
int COMPUTER_KINGS;
int PLAYER_MOVES_LEFT = 1;
int COMPUTER_MOVES_LEFT = 1;

const int BOARD_ROWS = 6;
const int BOARD_COLS = 8;

char b[BOARD_ROWS][BOARD_COLS];

const char KING_C = 'K';
const char HORSE_C = 'H';
const char BISHOP_C = 'B';
const char PAWN_C = 'P';

const char KING_P = 'k';
const char HORSE_P = 'h';
const char BISHOP_P = 'b';
const char PAWN_P = 'p';

const std::string KING_UNI_C = "♚";
const std::string HORSE_UNI_C = "♞";
const std::string BISHOP_UNI_C = "♝";
const std::string PAWN_UNI_C = "♟";

const std::string KING_UNI_P = "♔";
const std::string HORSE_UNI_P = "♘";
const std::string BISHOP_UNI_P = "♗";
const std::string PAWN_UNI_P = "♙";

const std::string DOT = "·";

bool IS_PLAYER_TURN = false;
bool IS_COMPUTER_TURN = false;

struct Move {
    // move[0] = from row
    // move[1] = from col
    // move[2] = to row
    // move[3] = to col
    int move[4];
    bool capture;
    char pieceCaptured;
};

void setup();

void play();

void playerTurn();

void computerTurn();

void checkGameOver();

void updateNumberOfKings();

void gameOver(int player, int p);

void printMoves(Move **moves, int left);

bool isValidMove(Move **moves, Move *move);

void allocateMoves(Move *moves[50]);

void deallocateMoves(Move *moves[50]);

// Minimax

Move miniMax();

int min(int depth,int parentBest,int maxDepth,time_t start);

int max(int depth,int parentBest,int maxDepth,time_t start);

int evaluate();

int checkForWinner();

// Board
void displayBoard();

void movePiece(Move *move, bool undo);

bool moveInBounds(int row, int col);

int whatQuadrant(int row, int col);

bool isSenior(bool isPlayer, int row, int col);

bool isLeftWing(bool isPlayer, int row, int col);

// Move generator
int generatePlayerMoves(Move **moves);

int generateComputerMoves(Move **moves);

int generatePawnMoves(Move **moves, int index, bool player, int row, int col);

int generateKingMoves(Move **moves, int index, bool player, int row, int col);

int generateHorseMoves(Move **moves, int index, bool player, int row, int col);

int generateBishopMoves(Move **moves, int index, bool player, int row, int col);

#endif //SKIP_MAIN_H
