#ifndef SKIP_MAIN_H
#define SKIP_MAIN_H

#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <ctime>
#include <algorithm>

#define WIN 500000
#define LOSE -500000
#define MAX 2000000
#define MIN -2000000
#define TIMES_UP 12345
#define MAX_DEPTH 99

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

#define KING_EVAL 10
#define BISHOP_EVAL 3
#define HORSE_EVAL 3
#define PAWN_EVAL 1

static const int COMPUTER = 2;
static const int PLAYER = 1;
static const int MAX_MOVES = 56;

static const bool UNDO = true;
static const bool DONT_UNDO = false;

static  int REASON_NO_MOVES_LEFT = 0;
static const int REASON_NO_KINGS_LEFT = 1;

static const int BOARD_ROWS = 6;
static const int BOARD_COLS = 8;

char b[BOARD_ROWS][BOARD_COLS];

static const char KING_C = 'K';
static const char HORSE_C = 'H';
static const char BISHOP_C = 'B';
static const char PAWN_C = 'P';

static const char KING_P = 'k';
static const char HORSE_P = 'h';
static const char BISHOP_P = 'b';
static const char PAWN_P = 'p';

static const char* KING_UNI_C = "\u265A";
static const char* HORSE_UNI_C = "\u265E";
static const char* BISHOP_UNI_C = "\u265D";
static const char* PAWN_UNI_C = "\u265F";

static const char* KING_UNI_P = "\u2654";
static const char* HORSE_UNI_P = "\u2658";
static const char* BISHOP_UNI_P = "\u2657";
static const char* PAWN_UNI_P = "\u2659";
static const char* DOT = "\u00B7";

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
    char piece_captured;
    bool should_change;
    // identity[0] = original identity
    // identity[1] = new identity
    char identity[2];
};

void setup();

void play();

void playerTurn();

void computerTurn();

void checkGameOver();

void gameOver(int player, int reason);

void printMoves(Move **moves, int left);

bool isValidMove(Move **moves, int move_count, Move *player_move);

void allocateMoves(Move *moves[50]);

void deallocateMoves(Move *moves[50]);

void convertInput(Move *player_move, const char *input);

void printComputerMove(const Move &move);

float timeDiff(clock_t begin_time);

bool sortCapture(Move *m1, Move *m2);

// Mini max

Move miniMax();

int min(int depth, int max_depth, int parents_best_score);

int max(int depth, int max_depth, int parents_best_score);

int evaluateMin(int depth);

int evaluateMax(int depth);

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

int generatePawnMoves(Move **moves, int &index, bool player, int row, int col);

int generateKingMoves(Move **moves, int &index, bool player, int row, int col);

int generateHorseMoves(Move **moves, int &index, bool player, int row, int col);

int generateBishopMoves(Move **moves, int &index, bool player, int row, int col);

#endif //SKIP_MAIN_H
