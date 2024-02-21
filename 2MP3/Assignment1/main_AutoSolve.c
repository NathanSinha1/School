#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 10
#define BOMBS 15

void Autosolve(char board[SIZE][SIZE], char playerBoard[SIZE+1][SIZE+1], int x, int y) {
    if (playerBoard[y+1][x+1] != '-') { // checks if cells has already been cleared
        return;
    }
    playerBoard[y+1][x+1] = board[y][x]; // changes cell to dev board
    if (board[y][x] == '0') { 
        Autosolve(board, playerBoard, x, y - 1); // Clears up
        Autosolve(board, playerBoard, x + 1, y); // Clears right
        Autosolve(board, playerBoard, x, y + 1); // Clears down
        Autosolve(board, playerBoard, x - 1, y); // Clears left
        Autosolve(board, playerBoard, x - 1, y - 1); // Clears up left
        Autosolve(board, playerBoard, x + 1, y - 1); // Clears up right
        Autosolve(board, playerBoard, x - 1, y + 1); // Clears down left
        Autosolve(board, playerBoard, x + 1, y + 1); // Clears down right
    }
}

int countAdjacentBombs(char board[SIZE][SIZE]){
    char surroundingBombs = '0'; 
    for (int i = 0; i < SIZE; i++) { // cycles through every cell
        for (int j = 0; j < SIZE; j++) {
            if (board[j - 1][i] == 'X' && j-1 >= 0) { // checks surounding spots for bombs
                surroundingBombs++;
            }
            if (board[j - 1][i + 1] == 'X' && i+1 < SIZE && j-1 >= 0) {
                surroundingBombs++;
            }
            if (board[j][i + 1] == 'X' && i+1 < SIZE) {
                surroundingBombs++;
            }
            if (board[j + 1][i + 1] == 'X' && i+1 < SIZE && j+1 < SIZE) {
                surroundingBombs++;
            }
            if (board[j + 1][i] == 'X' && j+1 < SIZE) {
                surroundingBombs++;
            }
            if (board[j + 1][i - 1] == 'X' && i-1 >= 0 && j+1 < SIZE) {
                surroundingBombs++;
            }
            if (board[j][i - 1] == 'X' && i-1 >= 0) {
                surroundingBombs++;
            }
            if (board[j - 1][i - 1] == 'X' && i-1 >= 0 && j-1 >= 0) {
                surroundingBombs++;
            }
            if (board[j][i] != 'X'){ // Change player board to say how many bombs in adjacent cells
                board[j][i] = surroundingBombs;
            }
            surroundingBombs = '0'; // reset count for for new cell
        }
    }
}

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

int main() {
    char board[SIZE][SIZE], playerBoard[SIZE+1][SIZE+1];
    char columns = '0', rows = '0';
    int gameOver = 0, gettingInput = 1;
    int x, y;
    int remainingCells = SIZE * SIZE - BOMBS, tempRemainingCells;
        
    for (int i=0; i < SIZE; i++){ // Initialize both boards
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

    placeBombs(board); // Initalize bomb locations

    countAdjacentBombs(board); // Initalize dev board

    printf("Welcome to minesweeper!\nEnter coordinates (x y) to reveal a cell.\n"); // Game start
    for (int i = 0; i < SIZE+1; i++) {
        for (int j = 0; j < SIZE+1; j++) {
            printf(" %c ", playerBoard[i][j]);
        }
        printf("\n");
    }

    while (remainingCells > BOMBS && !gameOver) { // Repeat untill win or lose

        printf("Enter X and Y cordinate: ");
        while (gettingInput == 1) {
            if (scanf("%d %d", &x, &y) == 2){ // If input is int, can proceed
                if (x < 0 || x >= SIZE || y < 0 || y >= SIZE){ // If input is in range, need new input
                    printf("Input is out of board's range. Please enter a new X and Y cordinate: ");
                }
                else if (playerBoard[y+1][x+1] != '-'){ // If input has already been revealed, need new input
                    printf("You have already revealed this cell. Please enter a new X and Y cordinate: ");
                }
                else { // Input is valid, can proceed
                    gettingInput = 0; 
                }
            }
            else {  // Input is not int, need new input (else loop written by chatGPT) 
                printf("Input must be integers. Please enter a new X and Y cordinate: ");
                int c;
                while ((c = getchar()) != '\n' && c != EOF); 
            }
        }
      
        if (board[y][x] == 'X'){ // Hitting a bomb, end game
            gameOver = 1;
        }

        else { // Hitting no bomb, reveal tile, auto solve if needed
            Autosolve(board, playerBoard, x, y); // Call autosolve (will reveal tile even if not 0)
            printf("Updated Board: \n"); // Prints new updated board
            for (int i = 0; i < SIZE + 1; i++) {
                for (int j = 0; j < SIZE + 1; j++) {
                    printf(" %c ", playerBoard[i][j]);
                }
                printf("\n");
            }
            tempRemainingCells = 0; // Counts remaining cell that need to be revealed to win
            for (int i = 0; i < SIZE; i++) {
                for (int j = 0; j < SIZE; j++) {
                    if (playerBoard[i+1][j+1] == '-'){
                        tempRemainingCells++;
                    }
                }
            }
            remainingCells = tempRemainingCells;
        }
        gettingInput = 1;
    }

    if (gameOver == 1) { // Lose
        printf("Game Over! You hit a bomb.\n");
    }
    else { // Win
        printf("You win! Thanks for playing.\n");
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
