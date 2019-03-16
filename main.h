#ifndef SKIP_MAIN_H
#define SKIP_MAIN_H

#include <cstdlib>
#include <cstdio>
#include <string>
#include <iostream>
#include <ctime>

#define WIN 5000
#define LOSE -5000
#define MAX 20000
#define MIN -20000
#define TIMES_UP 10000

#define A 0
#define B 1
#define C 2
#define D 3
#define E 4
#define F 5
#define G 6
#define H 7

#define ROW_SIX 0
#define ROW_FIVE 1
#define ROW_FOUR 2
#define ROW_THREE 3
#define ROW_TWO 4
#define ROW_ONE 5
#define ALLOWED_TIME 5

const int COMPUTER = 2;
const int PLAYER = 1;
const int MAX_MOVES = 50;

const bool UNDO = true;
const bool DONT_UNDO = false;

const int REASON_NO_MOVES_LEFT = 0;
const int REASON_NO_KINGS_LEFT = 1;

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

const std::string KING_UNI_C = "\u265A";
const std::string HORSE_UNI_C = "\u265E";
const std::string BISHOP_UNI_C = "\u265D";
const std::string PAWN_UNI_C = "\u265F";

const std::string KING_UNI_P = "\u2654";
const std::string HORSE_UNI_P = "\u2658";
const std::string BISHOP_UNI_P = "\u2657";
const std::string PAWN_UNI_P = "\u2659";
const std::string ROBOT = "\u1F91";
const std::string DOT = "\u00B7";

bool IS_PLAYER_TURN = false;
bool IS_COMPUTER_TURN = false;

clock_t START_TIME;

struct Move {
    // move[0] = from row
    // move[1] = from col
    // move[2] = to row
    // move[3] = to col
    int move[4];
    bool capture;
    char pieceCaptured;
    bool shouldChange;
    char newIdentity;
};

void setup();

void play();

void playerTurn();

void computerTurn();

void checkGameOver();

void updateNumberOfKings();

void gameOver(int player, int reason);

void printMoves(Move **moves, int left);

bool isValidMove(Move **moves, Move *move);

void allocateMoves(Move *moves[50]);

void deallocateMoves(Move *moves[50]);

void convertInput(Move *playerMove, const char *input);

void printComputerMove(const Move &move);

float timeDiff(clock_t begin_time);

// Minimax

Move miniMax();

int min(int depth, int maxDepth, int parent_best_score);

int max(int depth, int maxDepth, int parent_best_score);

int evaluate();

int checkForWinner();

// Board
void displayBoard();

void movePiece(Move *move, bool undo);

bool moveInBounds(int row, int col);

int whatQuadrant(int row, int col);

bool isSenior(bool isPlayer, int row, int col);

bool shouldChangeIdentity(Move *move);

// Move generator
int generatePlayerMoves(Move **moves);

int generateComputerMoves(Move **moves);

int generatePawnMoves(Move **moves, int index, bool player, int row, int col);

int generateKingMoves(Move **moves, int index, bool player, int row, int col);

int generateHorseMoves(Move **moves, int index, bool player, int row, int col);

int generateBishopMoves(Move **moves, int index, bool player, int row, int col);

#endif //SKIP_MAIN_H
