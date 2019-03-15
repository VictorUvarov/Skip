#include "main.h"

int main() {
    setup();
    displayBoard();
    play();
    return EXIT_SUCCESS;
}

void setup() {
    int i, j;
    for (i = 0; i < BOARD_ROWS; i++) {
        for (j = 0; j < BOARD_COLS; j++)
            b[i][j] = '-';
    }

    b[5][0] = HORSE_P;
    b[5][1] = HORSE_P;
    b[5][3] = KING_P;
    b[5][4] = KING_P;
    b[5][6] = BISHOP_P;
    b[5][7] = BISHOP_P;

    b[4][1] = PAWN_P;
    b[4][2] = PAWN_P;
    b[4][3] = PAWN_P;
    b[4][4] = PAWN_P;
    b[4][5] = PAWN_P;
    b[4][6] = PAWN_P;

    b[0][0] = HORSE_C;
    b[0][1] = HORSE_C;
    b[0][3] = KING_C;
    b[0][4] = KING_C;
    b[0][6] = BISHOP_C;
    b[0][7] = BISHOP_C;

    b[1][1] = PAWN_C;
    b[1][2] = PAWN_C;
    b[1][3] = PAWN_C;
    b[1][4] = PAWN_C;
    b[1][5] = PAWN_C;
    b[1][6] = PAWN_C;

}

void play() {
    int turn;
    std::cout << "Who goes first? (1)Player (2)Computer\n";
    std::cout << "Choice: ";
    if (std::cin >> turn) {
        if (turn == COMPUTER) {
            computerTurn();
            checkGameOver();
        }
        while (true) {
            playerTurn();
            checkGameOver();
            computerTurn();
            checkGameOver();
        }
    } else
        printf("ERROR: Invalid input. Choices were (1)Player (2)Computer\n");
}

void playerTurn() {
    IS_PLAYER_TURN = true;
    IS_COMPUTER_TURN = false;
    Move *moves[MAX_MOVES];
    Move *playerMove = new Move;
    bool legalMove = false;
    char input[5];

    allocateMoves(moves);

    PLAYER_MOVES_LEFT = generatePlayerMoves(moves);
    printMoves(moves, PLAYER_MOVES_LEFT);

    do {
        std::cout << "Enter move: ";
        if (std::cin >> input) {
            // convert input to correct move format
            playerMove->move[0] = '6' - input[1];
            playerMove->move[1] = toupper(input[0]) - 'A';
            playerMove->move[2] = '6' - input[3];
            playerMove->move[3] = toupper(input[2]) - 'A';

            // validate move
            legalMove = isValidMove(moves, playerMove);
            // move piece
            if (legalMove)
                movePiece(playerMove, DONT_UNDO);
            else {
                displayBoard();
                std::cout << "Illegal move, ";
                std::cout << "please review list of legal moves and try again.\n";
            }

        }
    } while (!legalMove);

    // Free up unused memory
    deallocateMoves(moves);
    delete playerMove;
    displayBoard();
}

void computerTurn() {
    IS_PLAYER_TURN = false;
    IS_COMPUTER_TURN = true;
    printf("Just thinking..\n");
    Move move = miniMax();
    // print computer move
    std::cout << "My move is "
              << (char) (move.move[1] + 'A')
              << move.move[0] + BOARD_ROWS
              << (char) (move.move[3] + 'A')
              << BOARD_ROWS - move.move[2]
              << "("
              << (char) (move.move[1] + 'A')
              << 7 - (BOARD_ROWS + move.move[0])
              << (char) (move.move[3] + 'A')
              << 7 - (BOARD_ROWS - move.move[2])
              << ")" << std::endl;


    // computer move
    movePiece(&move, DONT_UNDO);
    // display board
    displayBoard();
}

void checkGameOver() {
    updateNumberOfKings();
    if (COMPUTER_KINGS == 0) {
        gameOver(COMPUTER, REASON_NO_KINGS_LEFT);
    }
    if (PLAYER_KINGS == 0) {
        gameOver(PLAYER, REASON_NO_KINGS_LEFT);
    }
    if (COMPUTER_MOVES_LEFT == 0) {
        gameOver(COMPUTER, REASON_NO_MOVES_LEFT);
    }
    if (PLAYER_MOVES_LEFT == 0) {
        gameOver(PLAYER, REASON_NO_MOVES_LEFT);
    }
}

void updateNumberOfKings() {
    for (int i = 0; i < BOARD_ROWS; i++) {
        for (int j = 0; j < BOARD_COLS; j++) {
            if (b[i][j] == KING_P)
                PLAYER_KINGS++;
            if (b[i][j] == KING_C)
                COMPUTER_KINGS++;
        }
    }
}

bool isValidMove(Move **moves, Move *playerMove) {
    bool legalMove = false;
    for (int i = 0; i < PLAYER_MOVES_LEFT; i++) {
        if (moves[i]->move[0] == playerMove->move[0] &&
            moves[i]->move[1] == playerMove->move[1] &&
            moves[i]->move[2] == playerMove->move[2] &&
            moves[i]->move[3] == playerMove->move[3]) {
            playerMove->capture = moves[i]->capture;
            playerMove->pieceCaptured = moves[i]->pieceCaptured;
            playerMove->shouldChange = moves[i]->shouldChange;
            playerMove->newIdentity = moves[i]->newIdentity;
            legalMove = true;
            break;
        }
    }
    return legalMove;
}

void printMoves(Move **moves, const int size) {
    std::cout << "Available Moves: ";
    for (int i = 0; i < size; i++) {
        std::cout << (char) (moves[i]->move[1] + 'A') // from col
                  << BOARD_ROWS - moves[i]->move[0]  // from row
                  << (char) (moves[i]->move[3] + 'A') // to col
                  << BOARD_ROWS - moves[i]->move[2] // to row
                  << " ";
    }
    std::cout << std::endl;
}

void gameOver(const int player, const int reason) {
    if (player == PLAYER) {
        if (reason == REASON_NO_KINGS_LEFT) {
            std::cout << "Player has no kings. COMPUTER WINS.\n";
            displayBoard();
            exit(EXIT_SUCCESS);
        } else {
            std::cout << "Player ran out of moves. COMPUTER WINS.\n";
            displayBoard();
            exit(EXIT_SUCCESS);
        }
    } else {
        if (reason == REASON_NO_KINGS_LEFT) {
            std::cout << "Computer has no kings. YOU WIN!\n";
            displayBoard();
            exit(EXIT_SUCCESS);
        } else {
            std::cout << "Computer ran out of moves. YOU WIN!\n";
            displayBoard();
            exit(EXIT_SUCCESS);
        }
    }
}

void deallocateMoves(Move *moves[50]) {
    for (int i = 0; i < MAX_MOVES; i++)
        delete (moves[i]);
}

void allocateMoves(Move *moves[50]) {
    for (int i = 0; i < MAX_MOVES; i++) {
        moves[i] = new Move();
    }
}

Move miniMax() {
    Move *computer_moves[MAX_MOVES];
    Move bestMove;
    allocateMoves(computer_moves);

    COMPUTER_MOVES_LEFT = generateComputerMoves(computer_moves);
    bestMove = *computer_moves[0];

    deallocateMoves(computer_moves);
    return bestMove;
}

int max(int depth, int parentBest, int maxDepth, time_t start) {

}

int min(int depth, int parentBest, int maxDepth, time_t start) {

}

int evaluate() {
    int pieces = 0;
    int piecesWeight = 0;
    for (int i = 0; i < BOARD_ROWS; i++)
    {
        for (int j = 0; j < BOARD_COLS; j++)
        {
            if (islower(b[i][j]))
                pieces--;
            else if (isupper(b[i][j]))
                pieces++;
            else if (b[i][j] == KING_C)
                piecesWeight += 4;
            else if (b[i][j] == HORSE_C)
                piecesWeight += 2;
            else if (b[i][j] == BISHOP_C)
                piecesWeight += 1;
            else if (b[i][j] == KING_P)
                piecesWeight -= 4;
            else if (b[i][j] == HORSE_P)
                piecesWeight -= 2;
            else if (b[i][j] == BISHOP_P)
                piecesWeight -= 1;
        }
    }
    return pieces + piecesWeight;
}

int checkForWinner() {}

void movePiece(Move *move, bool undo) {
    char temp;
    if (undo) {
        b[move->move[0]][move->move[1]] = b[move->move[2]][move->move[3]];
        b[move->move[2]][move->move[3]] = move->pieceCaptured;
    } else {
        temp = b[move->move[0]][move->move[1]];
        b[move->move[0]][move->move[1]] = b[move->move[2]][move->move[3]];
        b[move->move[2]][move->move[3]] = temp;
        if (move->capture)
            b[move->move[0]][move->move[1]] = '-';
        if(move->shouldChange)
            b[move->move[2]][move->move[3]] = move->newIdentity;
    }
}

void displayBoard() {
    if (IS_COMPUTER_TURN)
        std::cout << "\n  \033[31;1;1m--------------------------------  Computer\033[0m\n";
    else
        std::cout << "\n  --------------------------------  Computer\n";
    for (int i = 0; i < BOARD_ROWS; i++) {
        std::cout << BOARD_ROWS - i << " ";
        for (int j = 0; j < BOARD_COLS; j++) {
            if ((i + j) % 2 == 0)
                std::cout << "\033[48;5;57m";

            switch (b[i][j]) {
                case KING_P:
                    std::cout << " " << KING_UNI_P << "  ";
                    break;
                case HORSE_P:
                    std::cout << " " << HORSE_UNI_P << "  ";
                    break;
                case BISHOP_P:
                    std::cout << " " << BISHOP_UNI_P << "  ";
                    break;
                case PAWN_P:
                    std::cout << " " << PAWN_UNI_P << "  ";
                    break;
                case KING_C:
                    std::cout << " " << KING_UNI_C << "  ";
                    break;
                case BISHOP_C:
                    std::cout << " " << BISHOP_UNI_C << "  ";
                    break;
                case HORSE_C:
                    std::cout << " " << HORSE_UNI_C << "  ";
                    break;
                case PAWN_C:
                    std::cout << " " << PAWN_UNI_C << "  ";
                    break;
                default:
                    std::cout << " " << DOT << "  ";
            }
            std::cout << "\033[0m";

        }
        std::cout << std::endl;

    }
    std::cout << "   A   B   C   D   E   F   G   H\n";
    if (IS_PLAYER_TURN)
        std::cout << "  \033[34;1;1m--------------------------------  Human\033[0m\n";
    else
        std::cout << "  --------------------------------  Human\n";
}

bool moveInBounds(int row, int col) {
    return row >= 0 && row < BOARD_ROWS && col >= 0 && col < BOARD_COLS;
}

bool isSenior(bool isPlayer, int row, int col) {
    int quad = whatQuadrant(row, col);
    if (isPlayer) {
        if (quad == 1 || quad == 2)
            return true;
    } else if (quad == 3 || quad == 4)
        return true;

    return false;
}

bool shouldChangeIdentity(Move *move) {
    int from_quad = whatQuadrant(move->move[0], move->move[1]);
    int to_quad = whatQuadrant(move->move[2], move->move[3]);

    if(from_quad == 1 && (to_quad == 2 || to_quad == 3))
        return true;
    if(from_quad == 2 && (to_quad == 1 || to_quad == 4))
        return true;
    if(from_quad == 3 && (to_quad == 1 || to_quad == 4))
        return true;

    return from_quad == 4 && (to_quad == 2 || to_quad == 3);
}

int whatQuadrant(int row, int col) {
    //Q1: ROW 0-2 | COL 4-7
    if (row >= 0 && row <= 2 && col >= 4 && col <= 7)
        return 1;
    //Q2: ROW 0-2 | COL 0-3
    if (row >= 0 && row <= 2 && col >= 0 && col <= 3)
        return 2;
    //Q3: ROW 3-5 | COL 0-3
    if (row >= 3 && row <= 5 && col >= 0 && col <= 3)
        return 3;
    //Q4: ROW 3-5 | COL 4-7
    //if(row >= 3 && row <= 5 && col >= 4 && col <= 7)
    return 4;
}

int generatePlayerMoves(Move **moves) {
    // keeps track of number of moves
    // and the current index for moves
    int numberOfMoves = 0;
    for (int i = 0; i < BOARD_ROWS; i++) {
        for (int j = 0; j < BOARD_COLS; j++) {
            switch (b[i][j]) {
                case PAWN_P:
                    numberOfMoves = generatePawnMoves(moves, numberOfMoves, true, i, j);
                    break;
                case KING_P:
                    numberOfMoves = generateKingMoves(moves, numberOfMoves, true, i, j);
                    break;
                case HORSE_P:
                    numberOfMoves = generateHorseMoves(moves, numberOfMoves, true, i, j);
                    break;
                case BISHOP_P:
                    numberOfMoves = generateBishopMoves(moves, numberOfMoves, true, i, j);
                    break;
                default:
                    break;
            }
        }
    }
    return numberOfMoves;
}

int generateComputerMoves(Move **moves) {
    // keeps track of number of moves
    // and the current index for moves
    int numberOfMoves = 0;
    for (int i = 0; i < BOARD_ROWS; i++) {
        for (int j = 0; j < BOARD_COLS; j++) {
            switch (b[i][j]) {
                case PAWN_C:
                    numberOfMoves = generatePawnMoves(moves, numberOfMoves, false, i, j);
                    break;
                case KING_C:
                    numberOfMoves = generateKingMoves(moves, numberOfMoves, false, i, j);
                    break;
                case HORSE_C:
                    numberOfMoves = generateHorseMoves(moves, numberOfMoves, false, i, j);
                    break;
                case BISHOP_C:
                    numberOfMoves = generateBishopMoves(moves, numberOfMoves, false, i, j);
                    break;
                default:
                    break;
            }
        }
    }
    return numberOfMoves;
}

int generatePawnMoves(Move **moves, int index, bool player, int row, int col) {
    if (player) {
        //forward
        if (moveInBounds(row - 1, col) && b[row - 1][col] == '-') {
            moves[index]->move[0] = row;
            moves[index]->move[1] = col;
            moves[index]->move[2] = row - 1;
            moves[index]->move[3] = col;
            moves[index]->capture = false;
            moves[index]->pieceCaptured = b[row - 1][col];
            index++;
        }
        //left capture
        if (moveInBounds(row - 1, col - 1) &&
            isupper(b[row - 1][col - 1])) {
            moves[index]->move[0] = row;
            moves[index]->move[1] = col;
            moves[index]->move[2] = row - 1;
            moves[index]->move[3] = col - 1;
            moves[index]->capture = true;
            moves[index]->pieceCaptured = b[row - 1][col - 1];
            index++;
        }
        //right capture
        if (moveInBounds(row - 1, col + 1) &&
            isupper(b[row - 1][col + 1])) {
            moves[index]->move[0] = row;
            moves[index]->move[1] = col;
            moves[index]->move[2] = row - 1;
            moves[index]->move[3] = col + 1;
            moves[index]->capture = true;
            moves[index]->pieceCaptured = b[row - 1][col + 1];
            index++;
        }
    } else // if(computer)
    {
        //forward
        if (moveInBounds(row + 1, col) && b[row + 1][col] == '-') {
            moves[index]->move[0] = row;
            moves[index]->move[1] = col;
            moves[index]->move[2] = row + 1;
            moves[index]->move[3] = col;
            moves[index]->capture = false;
            moves[index]->pieceCaptured = b[row + 1][col];
            index++;
        }
        //left capture
        if (moveInBounds(row + 1, col - 1) &&
            islower(b[row + 1][col - 1])) {
            moves[index]->move[0] = row;
            moves[index]->move[1] = col;
            moves[index]->move[2] = row + 1;
            moves[index]->move[3] = col - 1;
            moves[index]->capture = true;
            moves[index]->pieceCaptured = b[row + 1][col - 1];
            index++;
        }
        //right capture
        if (moveInBounds(row + 1, col + 1) &&
            islower(b[row + 1][col + 1])) {
            moves[index]->move[0] = row;
            moves[index]->move[1] = col;
            moves[index]->move[2] = row + 1;
            moves[index]->move[3] = col + 1;
            moves[index]->capture = true;
            moves[index]->pieceCaptured = b[row + 1][col + 1];
            index++;
        }
    }
    return index;
}

int generateKingMoves(Move **moves, int index, bool player, int row, int col) {
    if (player) {
        // right king
        if (moveInBounds(row, col + 1) && b[row][col + 1] == '-' && whatQuadrant(row, col) == 4) {
            moves[index]->move[0] = row;
            moves[index]->move[1] = col;
            moves[index]->move[2] = row;
            moves[index]->move[3] = col + 1;
            moves[index]->capture = false;
            moves[index]->pieceCaptured = b[row][col + 1];
            index++;
        }
        // right king right capture
        if (moveInBounds(row, col + 1) && isupper(b[row][col + 1]) && whatQuadrant(row, col) == 4) {
            moves[index]->move[0] = row;
            moves[index]->move[1] = col;
            moves[index]->move[2] = row;
            moves[index]->move[3] = col + 1;
            moves[index]->capture = true;
            moves[index]->pieceCaptured = b[row][col + 1];
            index++;
        }
        // left king
        if (moveInBounds(row, col - 1) && b[row][col - 1] == '-' && whatQuadrant(row, col) == 3) {
            moves[index]->move[0] = row;
            moves[index]->move[1] = col;
            moves[index]->move[2] = row;
            moves[index]->move[3] = col - 1;
            moves[index]->capture = false;
            moves[index]->pieceCaptured = b[row][col - 1];
            index++;
        }
        // left king left capture
        if (moveInBounds(row, col - 1) && isupper(b[row][col - 1]) && whatQuadrant(row, col) == 3) {
            moves[index]->move[0] = row;
            moves[index]->move[1] = col;
            moves[index]->move[2] = row;
            moves[index]->move[3] = col - 1;
            moves[index]->capture = true;
            moves[index]->pieceCaptured = b[row][col - 1];
            index++;
        }
    } else // if(computer)
    {
        // right king
        if (moveInBounds(row, col - 1) && b[row][col - 1] == '-' && whatQuadrant(row, col) == 2) {
            moves[index]->move[0] = row;
            moves[index]->move[1] = col;
            moves[index]->move[2] = row;
            moves[index]->move[3] = col - 1;
            moves[index]->capture = false;
            moves[index]->pieceCaptured = b[row][col - 1];
            index++;
        }
        // right king right capture
        if (moveInBounds(row, col - 1) && islower(b[row][col - 1]) && whatQuadrant(row, col) == 2) {
            moves[index]->move[0] = row;
            moves[index]->move[1] = col;
            moves[index]->move[2] = row;
            moves[index]->move[3] = col - 1;
            moves[index]->capture = true;
            moves[index]->pieceCaptured = b[row][col - 1];
            index++;
        }
        // left king
        if (moveInBounds(row, col + 1) && b[row][col + 1] == '-' && whatQuadrant(row, col) == 1) {
            moves[index]->move[0] = row;
            moves[index]->move[1] = col;
            moves[index]->move[2] = row;
            moves[index]->move[3] = col + 1;
            moves[index]->capture = false;
            moves[index]->pieceCaptured = b[row][col + 1];
            index++;
        }
        // left king left capture
        if (moveInBounds(row, col + 1) && islower(b[row][col + 1]) && whatQuadrant(row, col) == 1) {
            moves[index]->move[0] = row;
            moves[index]->move[1] = col;
            moves[index]->move[2] = row;
            moves[index]->move[3] = col + 1;
            moves[index]->capture = true;
            moves[index]->pieceCaptured = b[row][col + 1];
            index++;
        }
    }
    return index;
}

int generateHorseMoves(Move **moves, int index, bool player, int row, int col) {
    if (player) {
        // 2 left 1 up (col - 2, row - 1)
        if (moveInBounds(row - 1, col - 2) &&
            !islower(b[row - 1][col - 2])) {
            moves[index]->move[0] = row;
            moves[index]->move[1] = col;
            moves[index]->move[2] = row - 1;
            moves[index]->move[3] = col - 2;
            moves[index]->pieceCaptured = b[row - 1][col - 2];
            moves[index]->capture = isupper(b[row - 1][col - 2]) != 0;
            if (shouldChangeIdentity(moves[index])) {
                moves[index]->shouldChange = true;
                moves[index]->newIdentity = BISHOP_P;
            }
            index++;
        }
        // 2 right 1 up (col + 2, row - 1)
        if (moveInBounds(row - 1, col + 2) &&
            !islower(b[row - 1][col + 2])) {
            moves[index]->move[0] = row;
            moves[index]->move[1] = col;
            moves[index]->move[2] = row - 1;
            moves[index]->move[3] = col + 2;
            moves[index]->pieceCaptured = b[row - 1][col + 2];
            moves[index]->capture = isupper(b[row - 1][col + 2]) != 0;
            if (shouldChangeIdentity(moves[index])) {
                moves[index]->shouldChange = true;
                moves[index]->newIdentity = BISHOP_P;
            }
            index++;
        }
        // 1 left 2 up ( col - 1, row - 2)
        if (moveInBounds(row - 2, col - 1) &&
            !islower(b[row - 2][col - 1])) {
            moves[index]->move[0] = row;
            moves[index]->move[1] = col;
            moves[index]->move[2] = row - 2;
            moves[index]->move[3] = col - 1;
            moves[index]->pieceCaptured = b[row - 2][col - 1];
            moves[index]->capture = isupper(b[row - 2][col - 1]) != 0;
            if (shouldChangeIdentity(moves[index])) {
                moves[index]->shouldChange = true;
                moves[index]->newIdentity = BISHOP_P;
            }
            index++;
        }
        // 1 right 2 up ( col + 1, row - 2)
        if (moveInBounds(row - 2, col + 1) &&
            !islower(b[row - 2][col + 1])) {
            moves[index]->move[0] = row;
            moves[index]->move[1] = col;
            moves[index]->move[2] = row - 2;
            moves[index]->move[3] = col + 1;
            moves[index]->pieceCaptured = b[row - 2][col + 1];
            moves[index]->capture = isupper(b[row - 2][col + 1]) != 0;
            if (shouldChangeIdentity(moves[index])) {
                moves[index]->shouldChange = true;
                moves[index]->newIdentity = BISHOP_P;
            }
            index++;
        }
        // if senior backwards capture
        // 2 left 1 down (col - 2, row + 1)
        if (moveInBounds(row + 1, col - 2) &&
            isupper(b[row + 1][col - 2]) &&
            isSenior(true, row, col)) {
            moves[index]->move[0] = row;
            moves[index]->move[1] = col;
            moves[index]->move[2] = row + 1;
            moves[index]->move[3] = col - 2;
            moves[index]->capture = true;
            moves[index]->pieceCaptured = b[row + 1][col - 2];
            if (shouldChangeIdentity(moves[index])) {
                moves[index]->shouldChange = true;
                moves[index]->newIdentity = BISHOP_P;
            }
            index++;
        }
        // 2 right 1 down (col + 2, row + 1)
        if (moveInBounds(row + 1, col + 2) &&
            isupper(b[row + 1][col + 2]) &&
            isSenior(true, row, col)) {
            moves[index]->move[0] = row;
            moves[index]->move[1] = col;
            moves[index]->move[2] = row + 1;
            moves[index]->move[3] = col + 2;
            moves[index]->capture = true;
            moves[index]->pieceCaptured = b[row + 1][col + 2];
            if (shouldChangeIdentity(moves[index])) {
                moves[index]->shouldChange = true;
                moves[index]->newIdentity = BISHOP_P;
            }
            index++;
        }
        // 1 left 2 down (col - 1, row + 2)
        if (moveInBounds(row + 2, col - 1) &&
            isupper(b[row + 2][col - 1]) &&
            isSenior(true, row, col)) {
            moves[index]->move[0] = row;
            moves[index]->move[1] = col;
            moves[index]->move[2] = row + 2;
            moves[index]->move[3] = col - 1;
            moves[index]->capture = true;
            moves[index]->pieceCaptured = b[row + 2][col - 1];
            if (shouldChangeIdentity(moves[index])) {
                moves[index]->shouldChange = true;
                moves[index]->newIdentity = BISHOP_P;
            }
            index++;
        }
        // 1 right 2 down (col + 1, row + 2)
        if (moveInBounds(row + 2, col + 1) &&
            isupper(b[row + 2][col + 1]) &&
            isSenior(true, row, col)) {
            moves[index]->move[0] = row;
            moves[index]->move[1] = col;
            moves[index]->move[2] = row + 2;
            moves[index]->move[3] = col + 1;
            moves[index]->capture = true;
            moves[index]->pieceCaptured = b[row + 2][col + 1];
            if (shouldChangeIdentity(moves[index])) {
                moves[index]->shouldChange = true;
                moves[index]->newIdentity = BISHOP_P;
            }
            index++;
        }
    } else // if(computer)
    {
        // 2 left 1 up (col - 2, row + 1)
        if (moveInBounds(row + 1, col - 2) &&
            !isupper(b[row + 1][col - 2])) {
            moves[index]->move[0] = row;
            moves[index]->move[1] = col;
            moves[index]->move[2] = row + 1;
            moves[index]->move[3] = col - 2;
            moves[index]->pieceCaptured = b[row + 1][col - 2];
            moves[index]->capture = islower(b[row + 1][col - 2]) != 0;
            if (shouldChangeIdentity(moves[index])) {
                moves[index]->shouldChange = true;
                moves[index]->newIdentity = BISHOP_C;
            }
            index++;
        }
        // 2 right 1 up (col + 2, row + 1)
        if (moveInBounds(row + 1, col + 2) &&
            !isupper(b[row + 1][col + 2])) {
            moves[index]->move[0] = row;
            moves[index]->move[1] = col;
            moves[index]->move[2] = row + 1;
            moves[index]->move[3] = col + 2;
            moves[index]->pieceCaptured = b[row + 1][col + 2];
            moves[index]->capture = islower(b[row + 1][col + 2]) != 0;
            if (shouldChangeIdentity(moves[index])) {
                moves[index]->shouldChange = true;
                moves[index]->newIdentity = BISHOP_C;
            }
            index++;
        }
        // 1 left 2 up ( col - 1, row + 2)
        if (moveInBounds(row + 2, col - 1) &&
            !isupper(b[row + 2][col - 1])) {
            moves[index]->move[0] = row;
            moves[index]->move[1] = col;
            moves[index]->move[2] = row + 2;
            moves[index]->move[3] = col - 1;
            moves[index]->pieceCaptured = b[row + 2][col - 1];
            moves[index]->capture = islower(b[row + 2][col - 1]) != 0;
            if (shouldChangeIdentity(moves[index])) {
                moves[index]->shouldChange = true;
                moves[index]->newIdentity = BISHOP_C;
            }
            index++;
        }
        // 1 right 2 up ( col + 1, row + 2)
        if (moveInBounds(row + 2, col + 1) &&
            !isupper(b[row + 2][col + 1])) {
            moves[index]->move[0] = row;
            moves[index]->move[1] = col;
            moves[index]->move[2] = row + 2;
            moves[index]->move[3] = col + 1;
            moves[index]->pieceCaptured = b[row + 2][col + 1];
            moves[index]->capture = islower(b[row + 2][col + 1]) != 0;
            if (shouldChangeIdentity(moves[index])) {
                moves[index]->shouldChange = true;
                moves[index]->newIdentity = BISHOP_C;
            }
            index++;
        }
        // if senior backwards capture
        // 2 left 1 down (col - 2, row - 1)
        if (moveInBounds(row - 1, col - 2) &&
            islower(b[row - 1][col - 2]) &&
            isSenior(false, row, col)) {
            moves[index]->move[0] = row;
            moves[index]->move[1] = col;
            moves[index]->move[2] = row - 1;
            moves[index]->move[3] = col - 2;
            moves[index]->capture = true;
            moves[index]->pieceCaptured = b[row - 1][col - 2];
            if (shouldChangeIdentity(moves[index])) {
                moves[index]->shouldChange = true;
                moves[index]->newIdentity = BISHOP_C;
            }
            index++;
        }
        // 2 right 1 down (col + 2, row - 1)
        if (moveInBounds(row - 1, col + 2) &&
            islower(b[row - 1][col + 2]) &&
            isSenior(false, row, col)) {
            moves[index]->move[0] = row;
            moves[index]->move[1] = col;
            moves[index]->move[2] = row - 1;
            moves[index]->move[3] = col + 2;
            moves[index]->capture = true;
            moves[index]->pieceCaptured = b[row - 1][col + 2];
            if (shouldChangeIdentity(moves[index])) {
                moves[index]->shouldChange = true;
                moves[index]->newIdentity = BISHOP_C;
            }
            index++;
        }
        // 1 left 2 down (col - 1, row - 2)
        if (moveInBounds(row - 2, col - 1) &&
            islower(b[row - 2][col - 1]) &&
            isSenior(false, row, col)) {
            moves[index]->move[0] = row;
            moves[index]->move[1] = col;
            moves[index]->move[2] = row - 2;
            moves[index]->move[3] = col - 1;
            moves[index]->capture = true;
            moves[index]->pieceCaptured = b[row - 2][col - 1];
            if (shouldChangeIdentity(moves[index])) {
                moves[index]->shouldChange = true;
                moves[index]->newIdentity = BISHOP_C;
            }
            index++;
        }
        // 1 right 2 down (col + 1, row - 2)
        if (moveInBounds(row - 2, col + 1) &&
            islower(b[row - 2][col + 1]) &&
            isSenior(false, row, col)) {
            moves[index]->move[0] = row;
            moves[index]->move[1] = col;
            moves[index]->move[2] = row - 2;
            moves[index]->move[3] = col + 1;
            moves[index]->capture = true;
            moves[index]->pieceCaptured = b[row - 2][col + 1];
            if (shouldChangeIdentity(moves[index])) {
                moves[index]->shouldChange = true;
                moves[index]->newIdentity = BISHOP_C;
            }
            index++;
        }
    }
    return index;
}

int generateBishopMoves(Move **moves, int index, bool player, int row, int col) {
    if (player) {
        //forward right diagonal
        int i = 1;
        while (moveInBounds(row - i, col + i)) {
            if (!islower(b[row - i][col + i])) {
                moves[index]->move[0] = row;
                moves[index]->move[1] = col;
                moves[index]->move[2] = row - i;
                moves[index]->move[3] = col + i;
                moves[index]->pieceCaptured = b[row - i][col + i];
                if (shouldChangeIdentity(moves[index])) {
                    moves[index]->shouldChange = true;
                    moves[index]->newIdentity = HORSE_P;
                }
                if (isupper(b[row - i][col + i])) {
                    moves[index]->capture = true;
                    index++;
                    break;
                } else {
                    moves[index]->capture = false;
                    index++;
                }
            } else
                break;
            i++;
        }

        i = 1;
        while (moveInBounds(row - i, col - i)) {
            if (!islower(b[row - i][col - i])) {
                moves[index]->move[0] = row;
                moves[index]->move[1] = col;
                moves[index]->move[2] = row - i;
                moves[index]->move[3] = col - i;
                moves[index]->pieceCaptured = b[row - i][col - i];
                if (shouldChangeIdentity(moves[index])) {
                    moves[index]->shouldChange = true;
                    moves[index]->newIdentity = HORSE_P;
                }
                if (isupper(b[row - i][col - i])) {
                    moves[index]->capture = true;
                    index++;
                    break;
                } else {
                    moves[index]->capture = false;
                    index++;
                }
            } else
                break;
            i++;
        }

        i = 1;
        while (moveInBounds(row + i, col + i)) {
            if (isSenior(true, row + i, col + i) && isupper(b[row + i][col + i])) {
                moves[index]->move[0] = row;
                moves[index]->move[1] = col;
                moves[index]->move[2] = row + i;
                moves[index]->move[3] = col + i;
                moves[index]->capture = true;
                moves[index]->pieceCaptured = b[row + i][col + i];
                if (shouldChangeIdentity(moves[index])) {
                    moves[index]->shouldChange = true;
                    moves[index]->newIdentity = HORSE_P;
                }
                index++;
                break;
            } else if (islower(b[row + i][col + i]))
                break;
            i++;
        }

        i = 1;
        while (moveInBounds(row + i, col - i)) {
            if (isSenior(true, row + i, col - i) && isupper(b[row + i][col - i])) {
                moves[index]->move[0] = row;
                moves[index]->move[1] = col;
                moves[index]->move[2] = row + i;
                moves[index]->move[3] = col - i;
                moves[index]->capture = true;
                moves[index]->pieceCaptured = b[row + i][col - i];
                if (shouldChangeIdentity(moves[index])) {
                    moves[index]->shouldChange = true;
                    moves[index]->newIdentity = HORSE_P;
                }
                index++;
                break;
            } else if (islower(b[row + i][col - i]))
                break;
            i++;
        }
    } else // if(computer)
    {
        int i = 1;
        while (moveInBounds(row + i, col + i)) {
            if (!isupper(b[row + i][col + i])) {
                moves[index]->move[0] = row;
                moves[index]->move[1] = col;
                moves[index]->move[2] = row + i;
                moves[index]->move[3] = col + i;
                moves[index]->pieceCaptured = b[row + i][col + i];
                if (shouldChangeIdentity(moves[index])) {
                    moves[index]->shouldChange = true;
                    moves[index]->newIdentity = HORSE_C;
                }
                if (islower(b[row + i][col + i])) {
                    moves[index]->capture = true;
                    index++;
                    break;
                } else {
                    moves[index]->capture = false;
                    index++;
                }
            } else
                break;
            i++;
        }

        i = 1;
        while (moveInBounds(row + i, col - i)) {
            if (!isupper(b[row + i][col - i])) {
                moves[index]->move[0] = row;
                moves[index]->move[1] = col;
                moves[index]->move[2] = row + i;
                moves[index]->move[3] = col - i;
                moves[index]->pieceCaptured = b[row + i][col - i];
                if (shouldChangeIdentity(moves[index])) {
                    moves[index]->shouldChange = true;
                    moves[index]->newIdentity = HORSE_C;
                }
                if (islower(b[row + i][col - i])) {
                    moves[index]->capture = true;
                    index++;
                    break;
                } else {
                    moves[index]->capture = false;
                    index++;
                }
            } else
                break;
            i++;
        }

        i = 1;
        while (moveInBounds(row - i, col + i)) {
            if (islower(b[row - i][col + i])) {
                moves[index]->move[0] = row;
                moves[index]->move[1] = col;
                moves[index]->move[2] = row - i;
                moves[index]->move[3] = col + i;
                moves[index]->capture = true;
                moves[index]->pieceCaptured = b[row - i][col + i];
                if (shouldChangeIdentity(moves[index])) {
                    moves[index]->shouldChange = true;
                    moves[index]->newIdentity = HORSE_C;
                }
                index++;
                break;
            } else if (isupper(b[row - i][col + i]))
                break;
            i++;
        }

        i = 1;
        while (moveInBounds(row - i, col - i)) {
            if (islower(b[row - i][col - i])) {
                moves[index]->move[0] = row;
                moves[index]->move[1] = col;
                moves[index]->move[2] = row - i;
                moves[index]->move[3] = col - i;
                moves[index]->capture = true;
                moves[index]->pieceCaptured = b[row - i][col - i];
                if (shouldChangeIdentity(moves[index])) {
                    moves[index]->shouldChange = true;
                    moves[index]->newIdentity = HORSE_C;
                }
                index++;
                break;
            } else if (isupper(b[row - i][col - i]))
                break;
            i++;
        }
    }
    return index;
}
