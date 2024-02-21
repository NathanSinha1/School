#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 10
#define BOMBS 15

void placeBombs(char board[SIZE][SIZE]) {
    int bombsX[BOMBS], bombsY[BOMBS];
    int uniqueBombs = 0;
    srand(time(NULL));
    
    while(uniqueBombs < BOMBS) {
        bombsX[uniqueBombs] = rand() % SIZE; // location bomb x axis
        bombsY[uniqueBombs] = rand() % SIZE; // location bomb y axis
        
        if (board[bombsX[uniqueBombs]][bombsY[uniqueBombs]] == 'X') { // checking for dups
        }
        
        else { // placing bomb
            board[bombsX[uniqueBombs]][bombsY[uniqueBombs]] = 'X';
            uniqueBombs++;
        }
    }
}

int countAdjacentBombs(char board[SIZE][SIZE], char playerBoard[SIZE+1][SIZE+1], int x, int y) {
    char surroundingBombs = '0';
    if (board[y - 1][x] == 'X' && y-1 >= 0) { // Checking if X in adjacent cells & checking if tiles in range
        surroundingBombs++;
    }
    if (board[y - 1][x + 1] == 'X' && x+1 < SIZE && y-1 >= 0) {
        surroundingBombs++;
    }
    if (board[y][x + 1] == 'X' && x+1 < SIZE) {
        surroundingBombs++;
    }
    if (board[y + 1][x + 1] == 'X' && x+1 < SIZE && y+1 < SIZE) {
        surroundingBombs++;
    }
    if (board[y + 1][x] == 'X' && y+1 < SIZE) {
        surroundingBombs++;
    }
    if (board[y + 1][x - 1] == 'X' && x-1 >= 0 && y+1 < SIZE) {
        surroundingBombs++;
    }
    if (board[y][x - 1] == 'X' && x-1 >= 0) {
        surroundingBombs++;
    }
    if (board[y - 1][x - 1] == 'X' && x-1 >= 0 && y-1 >= 0) {
        surroundingBombs++;
    }
    playerBoard[y + 1][x + 1] = surroundingBombs; // Change player board to say how many bombs in adjacent cells
    surroundingBombs = '0';
    printf("Updated Board: \n"); // Update player board
    for (int i = 0; i < SIZE + 1; i++) {
        for (int j = 0; j < SIZE + 1; j++) {
            printf(" %c ", playerBoard[i][j]);
        }
        printf("\n");
    }
}

int main() {
    char board[SIZE][SIZE];
    char playerBoard[SIZE+1][SIZE+1];
    char columns = '0', rows = '0';
    int gameOver = 0, gettingInput = 1;
    int x, y;
    int remainingCells = SIZE * SIZE - BOMBS;
    
    for (int i=0; i < SIZE; i++){ // Initializing both boards
        for (int j=0; j < SIZE; j++) {
            board[i][j] = '-';
            playerBoard[i+1][j+1] = '-';
        }
    } 

    playerBoard[0][0] = ' '; // Making cordinates for playerBoard
    for (int i=0; i<SIZE; i++) {
        playerBoard[i+1][0] = columns++;
        playerBoard[0][i+1] = rows++;
    }

    placeBombs(board);

    printf("Welcome to minesweeper!\nEnter coordinates (x y) to reveal a cell.\n"); // Game start
    for (int i = 0; i < SIZE+1; i++) {
        for (int j = 0; j < SIZE+1; j++) {
            printf(" %c ", playerBoard[i][j]);
        }
        printf("\n");
    }

    
    // Repeat untill win or lose
    while (remainingCells > 0 && !gameOver) {
        printf("Enter X and Y cordinate: ");
        while (gettingInput == 1) { 
            if (scanf("%d %d", &x, &y) == 2){  // if input is int
                if (x < 0 || x >= SIZE || y < 0 || y >= SIZE){ // if input is in range
                    printf("Input is out of board's range. Please enter a new X and Y cordinate: ");
                }
                else if (playerBoard[y+1][x+1] != '-'){ // if input has already been revealed
                    printf("You have already revealed this cell. Please enter a new X and Y cordinate: ");
                }
                else { // if input is valid
                    gettingInput = 0; 
                }
            }
            else { // Input is not int, need new input (else loop written by chatGPT)
                printf("Input must be integers. Please enter a new X and Y cordinate: ");
                int c;
                while ((c = getchar()) != '\n' && c != EOF);
            }
        }
      
        if (board[y][x] == 'X'){ // Hitting a bomb
            gameOver = 1;
        }

        if (board[y][x] == '-') { // Hitting no bomb
            countAdjacentBombs(board, playerBoard, x, y);  
            remainingCells--; 
        }
        gettingInput = 1;
    }

    if (gameOver == 1) { // Loss
        printf("Game Over! You hit a bomb.\n");
    }
    else { // Win
        printf("You win!");
    }

    printf("Here's where all the bombs were!\n"); // reveals bombs
    for (int i=0; i < SIZE+1; i++){ // prints and updates player board w/ bombs
        for (int j=0; j < SIZE+1; j++) {
            if (board[i][j] == 'X' && i < SIZE && j < SIZE) { // change cells to X on player board
                playerBoard[i+1][j+1] = 'X';
            }
            printf(" %c ", playerBoard[i][j]);
        }
        printf("\n");
    } 
    
    return 0;
}