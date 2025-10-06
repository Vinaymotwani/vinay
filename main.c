#include <stdio.h>
#include <limits.h>
#include <stdbool.h> // For using bool, true, and false

#define PLAYER 'X'
#define AI 'O'

char board[3][3];

// Function to initialize the board
void initializeBoard() {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            board[i][j] = ' ';
        }
    }
}

// Function to print the board
void printBoard() {
    printf("\n");
    printf(" %c | %c | %c \n", board[0][0], board[0][1], board[0][2]);
    printf("---|---|---\n");
    printf(" %c | %c | %c \n", board[1][0], board[1][1], board[1][2]);
    printf("---|---|---\n");
    printf(" %c | %c | %c \n", board[2][0], board[2][1], board[2][2]);
    printf("\n");
}

// Check if any moves are left on the board
bool isMovesLeft() {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == ' ')
                return true;
        }
    }
    return false;
}

// Evaluate board state
int evaluate() {
    // Check rows
    for (int row = 0; row < 3; row++) {
        if (board[row][0] == board[row][1] && 
            board[row][1] == board[row][2]) {
            if (board[row][0] == AI)
                return +10;
            else if (board[row][0] == PLAYER)
                return -10;
        }
    }
    // Check columns
    for (int col = 0; col < 3; col++) {
        if (board[0][col] == board[1][col] && 
            board[1][col] == board[2][col]) {
            if (board[0][col] == AI)
                return +10;
            else if (board[0][col] == PLAYER)
                return -10;
        }
    }
    // Check diagonals
    if (board[0][0] == board[1][1] && 
        board[1][1] == board[2][2]) {
        if (board[0][0] == AI)
            return +10;
        else if (board[0][0] == PLAYER)
            return -10;
    }
    if (board[0][2] == board[1][1] && 
        board[1][1] == board[2][0]) {
        if (board[0][2] == AI)
            return +10;
        else if (board[0][2] == PLAYER)
            return -10;
    }
    return 0; // No winner
}

// Minimax function
int minimax(int depth, bool isMax) {
    int score = evaluate();

    if (score == 10) return score - depth;  // Prefer faster wins
    if (score == -10) return score + depth; // Prefer slower losses
    if (!isMovesLeft()) return 0;

    if (isMax) {
        int best = INT_MIN;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i][j] == ' ') {
                    board[i][j] = AI;
                    best = (minimax(depth + 1, false) > best) ? minimax(depth + 1, false) : best;
                    board[i][j] = ' ';
                }
            }
        }
        return best;
    } else {
        int best = INT_MAX;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i][j] == ' ') {
                    board[i][j] = PLAYER;
                    best = (minimax(depth + 1, true) < best) ? minimax(depth + 1, true) : best;
                    board[i][j] = ' ';
                }
            }
        }
        return best;
    }
}

// Find the best move for AI
struct Move {
    int row, col;
};

struct Move findBestMove() {
    int bestVal = INT_MIN;
    struct Move bestMove;
    bestMove.row = -1;
    bestMove.col = -1;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == ' ') {
                board[i][j] = AI;
                int moveVal = minimax(0, false);
                board[i][j] = ' ';

                if (moveVal > bestVal) {
                    bestMove.row = i;
                    bestMove.col = j;
                    bestVal = moveVal;
                }
            }
        }
    }
    return bestMove;
}

// Main function
int main() {
    initializeBoard();
    int x, y;
    printf("Tic-Tac-Toe: You are X, AI is O\n");

    while (true) {
        printBoard();
        printf("Enter your move (row and column: 0 1 2): ");
        scanf("%d %d", &x, &y);

        if (board[x][y] != ' ') {
            printf("Invalid move, try again!\n");
            continue;
        }
        board[x][y] = PLAYER;

        if (evaluate() == -10) {
            printBoard();
            printf("You win!\n");
            break;
        }
        if (!isMovesLeft()) {
            printBoard();
            printf("It's a draw!\n");
            break;
        }

        struct Move bestMove = findBestMove();
        board[bestMove.row][bestMove.col] = AI;

        if (evaluate() == 10) {
            printBoard();
            printf("AI wins!\n");
            break;
        }
        if (!isMovesLeft()) {
            printBoard();
            printf("It's a draw!\n");
            break;
        }
    }
    return 0;
}
