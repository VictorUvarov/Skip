#include "main.h"

int main() {
    setup();
    displayBoard();
    play();
    return EXIT_SUCCESS;
}

void setup() {
    PLAYER_KINGS = 2;
    COMPUTER_KINGS = 2;
    int i, j;
    for (i = 0; i < BOARD_ROWS; i++) {
        for (j = 0; j < BOARD_COLS; j++)
            b[i][j] = '-';
    }

    b[ROW_ONE][A] = HORSE_P;
    b[ROW_ONE][B] = HORSE_P;
    b[ROW_ONE][D] = KING_P;
    b[ROW_ONE][E] = KING_P;
    b[ROW_ONE][G] = BISHOP_P;
    b[ROW_ONE][H] = BISHOP_P;

    b[ROW_TWO][B] = PAWN_P;
    b[ROW_TWO][C] = PAWN_P;
    b[ROW_TWO][D] = PAWN_P;
    b[ROW_TWO][E] = PAWN_P;
    b[ROW_TWO][F] = PAWN_P;
    b[ROW_TWO][G] = PAWN_P;

    b[ROW_SIX][A] = HORSE_C;
    b[ROW_SIX][B] = HORSE_C;
    b[ROW_SIX][D] = KING_C;
    b[ROW_SIX][E] = KING_C;
    b[ROW_SIX][G] = BISHOP_C;
    b[ROW_SIX][H] = BISHOP_C;

    b[ROW_FIVE][B] = PAWN_C;
    b[ROW_FIVE][C] = PAWN_C;
    b[ROW_FIVE][D] = PAWN_C;
    b[ROW_FIVE][E] = PAWN_C;
    b[ROW_FIVE][F] = PAWN_C;
    b[ROW_FIVE][G] = PAWN_C;
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
    if (PLAYER_MOVES_LEFT == 0)
        gameOver(PLAYER, REASON_NO_MOVES_LEFT);
    printMoves(moves, PLAYER_MOVES_LEFT);

    do {
        std::cout << "Enter move: ";
        if (std::cin >> input) {
            convertInput(playerMove, input);
            legalMove = isValidMove(moves, playerMove);

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
    std::cout << "...\n";
    Move move = miniMax();

    printComputerMove(move);
    movePiece(&move, DONT_UNDO);
    displayBoard();
}

void checkGameOver() {
    updateNumberOfKings();
    if (PLAYER_KINGS == 0)
        gameOver(PLAYER, REASON_NO_KINGS_LEFT);
    if (COMPUTER_KINGS == 0)
        gameOver(COMPUTER, REASON_NO_KINGS_LEFT);
}

void updateNumberOfKings() {
    int k_c = 0, k_p = 0;
    for (int i = 0; i < BOARD_ROWS; i++) {
        for (int j = 0; j < BOARD_COLS; j++) {
            if (b[i][j] == KING_P)
                k_p++;
            if (b[i][j] == KING_C)
                k_c++;
        }
    }
    PLAYER_KINGS = k_p;
    COMPUTER_KINGS = k_c;
}

void convertInput(Move *playerMove, const char *input) {
    playerMove->move[0] = '6' - input[1];
    playerMove->move[1] = toupper(input[0]) - 'A';
    playerMove->move[2] = '6' - input[3];
    playerMove->move[3] = toupper(input[2]) - 'A';
}

void printComputerMove(const Move &move) {
    std::cout << "My move is "
              << (char) (move.move[1] + 'A')
              << 6 - move.move[0]
              << (char) (move.move[3] + 'A')
              << 6 - move.move[2]
              << "("
              << (char) (move.move[1] + 'A')
              << 9 - (8 - move.move[0])
              << (char) (move.move[3] + 'A')
              << 9 - (8 - move.move[2])
              << ")" << std::endl;
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

float timeDiff(const clock_t begin_time) { return float(clock() - begin_time) / CLOCKS_PER_SEC; }

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
    Move *prevBestMove = new Move();
    Move bestMove = {};
    int score = 0, i = 0, d = 0, depth = 0, maxDepth = 1;
    int bestScore = MIN;
    START_TIME = clock();
    allocateMoves(computer_moves);

    COMPUTER_MOVES_LEFT = generateComputerMoves(computer_moves);
    if (COMPUTER_MOVES_LEFT == 0)
        gameOver(COMPUTER, REASON_NO_MOVES_LEFT);

    while ((timeDiff(START_TIME)) < ALLOWED_TIME) {
        for (i = 0; i < COMPUTER_MOVES_LEFT; ++i) {
            movePiece(computer_moves[i], DONT_UNDO);
            score = min(depth + 1, maxDepth + d, MIN);
            movePiece(computer_moves[i], UNDO);
            if (score > bestScore) {
                bestScore = score;
                bestMove = *computer_moves[i];
            }
        }
        if (score == TIMES_UP) *prevBestMove = bestMove; // if early store last best
        if (score == MAX) break; // stop searching if win
        d++;
    }
    std::cout << "dove down to depth " << maxDepth + i << " that turn\n";
    if (score == TIMES_UP) bestMove = *prevBestMove; // if we break out early we want to return to last depths best
    deallocateMoves(computer_moves);
    return bestMove;
}

int min(const int depth, const int maxDepth, const int parent_best_score) {
    if (checkForWinner() != 0)
        return checkForWinner();
    if (depth == maxDepth)
        return evaluate();

    int bestScore = MAX, score;
    Move *moves[MAX_MOVES];
    allocateMoves(moves);
    int size = generatePlayerMoves(moves);

    for (int i = 0; i < size; ++i) {
        if (timeDiff(START_TIME) >= ALLOWED_TIME) {
            deallocateMoves(moves);
            return TIMES_UP;
        }
        movePiece(moves[i], DONT_UNDO);
        score = max(depth + 1, maxDepth, parent_best_score);
        movePiece(moves[i], UNDO);
        if (score == TIMES_UP) {
            deallocateMoves(moves);
            return TIMES_UP;
        }
        if (score < parent_best_score) {
            deallocateMoves(moves);
            return score;
        }
        if (score < bestScore)
            bestScore = score;
    }
    deallocateMoves(moves);
    return bestScore;
}

int max(const int depth, const int maxDepth, const int parent_best_score) {
    if (checkForWinner() != 0)
        return checkForWinner();
    if (depth == maxDepth)
        return evaluate();

    int bestScore = MIN, score;
    Move *moves[MAX_MOVES];
    allocateMoves(moves);
    int size = generateComputerMoves(moves);

    for (int i = 0; i < size; ++i) {
        if (timeDiff(START_TIME) >= ALLOWED_TIME) {
            deallocateMoves(moves);
            return TIMES_UP;
        }
        movePiece(moves[i], DONT_UNDO);
        score = min(depth + 1, maxDepth, parent_best_score);
        movePiece(moves[i], UNDO);
        if (score == TIMES_UP) {
            deallocateMoves(moves);
            return TIMES_UP;
        }
        if (score > parent_best_score) {
            deallocateMoves(moves);
            return score;
        }
        if (score > bestScore) {
            bestScore = score;
        }
    }
    deallocateMoves(moves);
    return bestScore;
}

int evaluate() {
    int pieces = 0;
    int piecesWeight = 0;
    for (int i = 0; i < BOARD_ROWS; i++) {
        for (int j = 0; j < BOARD_COLS; j++) {
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

int checkForWinner() {
    updateNumberOfKings();
    if (PLAYER_MOVES_LEFT == 0)
        return WIN;
    if (PLAYER_KINGS == 0)
        return WIN;
    if (COMPUTER_MOVES_LEFT == 0)
        return LOSE;
    if (COMPUTER_KINGS == 0)
        return LOSE;
    return 0;
}

void movePiece(Move *move, const bool undo) {
    char temp;
    if (undo) {
        b[move->move[0]][move->move[1]] = b[move->move[2]][move->move[3]];
        b[move->move[2]][move->move[3]] = move->pieceCaptured;
        if (move->shouldChange) {
            temp = move->newIdentity;
            b[move->move[0]][move->move[1]] = move->newIdentity;
            move->newIdentity = temp;
        }
    } else {
        temp = b[move->move[0]][move->move[1]];
        b[move->move[0]][move->move[1]] = b[move->move[2]][move->move[3]];
        b[move->move[2]][move->move[3]] = temp;
        if (move->capture)
            b[move->move[0]][move->move[1]] = '-';
        if (move->shouldChange) {
            b[move->move[2]][move->move[3]] = move->newIdentity;
            move->newIdentity = temp;
        }
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

bool moveInBounds(const int row, const int col) {
    return row >= 0 && row < BOARD_ROWS && col >= 0 && col < BOARD_COLS;
}

bool isSenior(const bool isPlayer, const int row, const int col) {
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

    if (from_quad == 1 && (to_quad == 2 || to_quad == 3))
        return true;
    if (from_quad == 2 && (to_quad == 1 || to_quad == 4))
        return true;
    if (from_quad == 3 && (to_quad == 1 || to_quad == 4))
        return true;

    return from_quad == 4 && (to_quad == 2 || to_quad == 3);
}

int whatQuadrant(const int row, const int col) {
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

int generatePawnMoves(Move **moves, int index, const bool player, const int row, const int col) {
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

int generateKingMoves(Move **moves, int index, const bool player, const int row, const int col) {
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

int generateHorseMoves(Move **moves, int index, const bool player, const int row, const int col) {
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

int generateBishopMoves(Move **moves, int index, const bool player, const int row, const int col) {
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
        // forward left diagonal
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

        // backwards right diagonal SENIOR ONLY
        i = 1;
        while (moveInBounds(row + i, col + i)) {
            if (isSenior(true, row, col) && isupper(b[row + i][col + i])) {
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

        // backwards left diagonal SENIOR ONLY
        i = 1;
        while (moveInBounds(row + i, col - i)) {
            if (isSenior(true, row, col) && isupper(b[row + i][col - i])) {
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
        // forward left diagonal
        int i = 1;
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

        // forward right diagonal
        i = 1;
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


        // backwards right diagonal SENIOR ONLY
        i = 1;
        while (moveInBounds(row - i, col + i)) {
            if (isSenior(false, row, col) && islower(b[row - i][col + i])) {
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

        // backwards left diagonal SENIOR ONLY
        i = 1;
        while (moveInBounds(row - i, col - i)) {
            if (isSenior(false, row, col) && islower(b[row - i][col - i])) {
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
