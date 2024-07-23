#include <stdio.h>
#include <windows.h>
#include <time.h>
#include <stdlib.h>
#include <conio.h>

#define HEAD_MARK 7     /* use number 7 to mark head of snake */
#define APPLE_MARK 8    /* use number 8 to mark apple */
#define EMPTY 0         /* use number 0 for empty */
#define BOUND 9         /* use number 9 for bound */
#define MAX_SCORE 62
#define BOARD_SIZE 10

enum Direction {
    DIR_NO = 0,
    DIR_UP,
    DIR_RIGHT,
    DIR_DOWN,
    DIR_LEFT
};

void printGame();
void resetConsole();
void resetCursorPosition();
void updateTailPosition();
void updateHeadPosition();
void createBoundary();
void createSnake();
void createApple();
void setCursorPositionToEnd();
int isSnakeCollision();
void getSnakeDirection();

int moving = FALSE;
int checkBreak = 0;
int score = 0;
int board[BOARD_SIZE][BOARD_SIZE]; // board
int headCol = 4, headRow = 4; // head
int tailRow = 3, tailCol = 4; // tail
int apple = 0;
int headDir = DIR_NO, tailDir = DIR_DOWN; // set direction

int main() {
    createBoundary();
    createSnake();
    do {
        getSnakeDirection();

        if (moving == TRUE) {
            updateHeadPosition();
            updateTailPosition();
            checkBreak = isSnakeCollision();
        }

        createApple();

        resetConsole();

        printGame();

        Sleep(500);
    } while (score < MAX_SCORE && checkBreak == FALSE);

    setCursorPositionToEnd();
    printf("score: %d\n", score);
    Sleep(1000);
    return 0;
}

void createBoundary() {
    for (int i = 0; i < BOARD_SIZE; i++) {
        board[i][0] = BOUND;
        board[0][i] = BOUND;
        board[i][BOARD_SIZE - 1] = BOUND;
        board[BOARD_SIZE - 1][i] = BOUND;
    }
}

void createSnake() {
    board[headRow][headCol] = HEAD_MARK; // set head
    board[tailRow][tailCol] = DIR_DOWN;  // set tail
}

void createApple() {
    if (apple == 0) {
        int x = 0, y = 0;
        srand(time(0));
        while (board[x][y] != EMPTY) {
            x = rand() % (BOARD_SIZE - 2) + 1;
            y = rand() % (BOARD_SIZE - 2) + 1;
        }
        board[x][y] = APPLE_MARK;
        apple++;
    }
}

void printGame() {
    if (checkBreak == FALSE) {
        for (int i = 0; i < BOARD_SIZE; i++) {
            for (int j = 0; j < BOARD_SIZE; j++) {
                switch (board[i][j]) {
                    case DIR_DOWN:
                    case DIR_RIGHT:
                    case DIR_UP:
                    case DIR_LEFT:
                        printf("0");
                        break;
                    case HEAD_MARK:
                        printf("*");
                        break;
                    case APPLE_MARK:
                        printf("$");
                        break;
                    case EMPTY:
                        printf(" ");
                        break;
                    case BOUND:
                        printf("@");
                        break;
                }
            }
            printf("\n");
        }
    }
}

void updateHeadPosition() {
    board[headRow][headCol] = headDir;
    switch (headDir) {
        case DIR_UP:
            headRow--;
            break;
        case DIR_RIGHT:
            headCol++;
            break;
        case DIR_DOWN:
            headRow++;
            break;
        case DIR_LEFT:
            headCol--;
            break;
    }

    if (board[headRow][headCol] == APPLE_MARK) {
        apple--;
        score++;
    } else if (board[headRow][headCol] != EMPTY) {
        checkBreak = TRUE;
    }

    board[headRow][headCol] = HEAD_MARK;
}

void updateTailPosition() {
    if (apple == 1) {
        board[tailRow][tailCol] = EMPTY;
        switch (tailDir) {
            case DIR_UP:
                tailRow--;
                break;
            case DIR_RIGHT:
                tailCol++;
                break;
            case DIR_DOWN:
                tailRow++;
                break;
            case DIR_LEFT:
                tailCol--;
                break;
        }
        tailDir = board[tailRow][tailCol];
    }
}

void resetCursorPosition() {
    COORD coord = {0, 0};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void setCursorPositionToEnd() {
    COORD coord = {0, BOARD_SIZE};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void resetConsole() {
    for (int i = 0; i < 5; i++) {
        printf("\n\n\n\n");
    }
    resetCursorPosition();
}

void getSnakeDirection() {
    if (_kbhit()) {
        int key = _getch();
        if (key == 0 || key == 224) {
            key = _getch(); // Get the extended key code
            switch (key) {
                case 72:
                    if (headDir != DIR_DOWN) {
                        headDir = DIR_UP;
                        moving = TRUE;
                    }
                    break;
                case 80:
                    if (headDir != DIR_UP) {
                        headDir = DIR_DOWN;
                        moving = TRUE;
                    }
                    break;
                case 75:
                    if (headDir != DIR_RIGHT) {
                        headDir = DIR_LEFT;
                        moving = TRUE;
                    }
                    break;
                case 77:
                    if (headDir != DIR_LEFT) {
                        headDir = DIR_RIGHT;
                        moving = TRUE;
                    }
                    break;
                default:
                    break;
            }
        }
    }
}

int isSnakeCollision() {
    return checkBreak;
}
