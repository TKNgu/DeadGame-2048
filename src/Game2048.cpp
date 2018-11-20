#include "Game2048.hpp"

Game2048::Game2048() : score(0) {
    memset((void*)this->board, DEFAULT, sizeof(board_data) * SIZE * SIZE);
    srand(time(NULL));
    addRandom();
    addRandom();
}

board_data (&Game2048::getBoard())[SIZE][SIZE] {
    return this->board;
}

void Game2048::addRandom() {
    this->lenRandom = 0;
    for(board_data indexX = 0; indexX < SIZE; indexX++) {
        for(board_data indexY = 0; indexY < SIZE; indexY++) {
            if(!this->board[indexX][indexY]) {
                this->cacheRandomBoard[this->lenRandom++] =
                    indexX << BITSIZE | indexY;
            }
        }
    }
    int index = rand() % this->lenRandom;
    this->board[this->cacheRandomBoard[index] >> BITSIZE]
    [cacheRandomBoard[index] & 0xff] = (rand()%10)/9+1;
}

board_data Game2048::findTarget(board_data (&array)[SIZE],board_data x,board_data stop) {
    uint8_t t;
    // if the position is already on the first, don't evaluate
    if (x==0) {
        return x;
    }
    for(t=x-1; t>=0; t--) {
        if (array[t]!=0) {
            if (array[t]!=array[x]) {
                // merge is not possible, take next position
                return t+1;
            }
            return t;
        } else {
            // we should not slide further, return this one
            if (t==stop) {
                return t;
            }
        }
    }
    // we did not find a
    return x;
}

bool Game2048::slideArray(board_data (&array)[SIZE]) {
    bool success = false;
    uint8_t x,t,stop=0;

    for (x=0; x<SIZE; x++) {
        if (array[x]!=0) {
            t = findTarget(array,x,stop);
            // if target is not original position, then move or merge
            if (t!=x) {
                // if target is zero, this is a move
                if (array[t]==0) {
                    array[t]=array[x];
                } else if (array[t]==array[x]) {
                    // merge (increase power of two)
                    array[t]++;
                    // increase score
                    this->score+=(uint32_t)1<<array[t];
                    // set stop to avoid double merge
                    stop = t+1;
                }
                array[x]=0;
                success = true;
            }
        }
    }
    return success;
}

bool Game2048::moveUp() {
    bool success = false;
    for(int indexX = 0; indexX < SIZE; indexX++) {
        success |= Game2048::slideArray(this->board[indexX]);
    }
    return success;
}

bool Game2048::moveLeft() {
    bool success;
    rotateBoard(this->board);
    success = moveUp();
    rotateBoard(this->board);
    rotateBoard(this->board);
    rotateBoard(this->board);
    return success;
}

void Game2048::rotateBoard(uint8_t (&board)[SIZE][SIZE]) {
    uint8_t i,j,n=SIZE;
    uint8_t tmp;
    for (i=0; i<n/2; i++) {
        for (j=i; j<n-i-1; j++) {
            tmp = board[i][j];
            board[i][j] = board[j][n-i-1];
            board[j][n-i-1] = board[n-i-1][n-j-1];
            board[n-i-1][n-j-1] = board[n-j-1][i];
            board[n-j-1][i] = tmp;
        }
    }
}

bool Game2048::moveRight() {
    bool success;
    rotateBoard(board);
    rotateBoard(board);
    rotateBoard(board);
    success = moveUp();
    rotateBoard(board);
    return success;
}

bool Game2048::moveDown() {
    bool success;
    rotateBoard(board);
    rotateBoard(board);
    success = moveUp();
    rotateBoard(board);
    rotateBoard(board);
    return success;
}

bool Game2048::isEnd() {
    bool ended = true;
    if (countEmpty(board)>0) return false;
    if (findPairDown(board)) return false;
    rotateBoard(board);
    if (findPairDown(board)) ended = false;
    rotateBoard(board);
    rotateBoard(board);
    rotateBoard(board);
    return ended;
}

board_data Game2048::countEmpty(uint8_t (&board)[SIZE][SIZE]) {
    uint8_t x,y;
    uint8_t count=0;
    for (x=0; x<SIZE; x++) {
        for (y=0; y<SIZE; y++) {
            if (board[x][y]==0) {
                count++;
            }
        }
    }
    return count;
}

bool Game2048::findPairDown(uint8_t (&board)[SIZE][SIZE]) {
    bool success = false;
    uint8_t x,y;
    for (x=0; x<SIZE; x++) {
        for (y=0; y<SIZE-1; y++) {
            if (board[x][y]==board[x][y+1]) return true;
        }
    }
    return success;
}