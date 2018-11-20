#ifndef __GAME_2048_HPP__
#define __GAME_2048_HPP__

#include <cstring>
#include <cstdint>
#include <ctime>
#include <cstdlib>

#define DEFAULT 0
#define BITSIZE 8

typedef uint8_t board_data;
const board_data SIZE = 4U;

class  Game2048 {
    public:
        Game2048();
        board_data (&getBoard())[SIZE][SIZE];
        void addRandom();
        bool moveUp();
        bool moveLeft();
        bool moveRight();
        bool moveDown();
        bool isEnd();
    private:
        board_data board[SIZE][SIZE];
        uint16_t cacheRandomBoard[SIZE * SIZE];
        int lenRandom;
        int score;
    private:
        void initBoard();
        bool slideArray(board_data (&)[SIZE]);
        board_data findTarget(uint8_t (&)[SIZE], board_data, board_data);
        void rotateBoard(board_data (&)[SIZE][SIZE]);
        board_data countEmpty(board_data (&)[SIZE][SIZE]);
        bool findPairDown(board_data (&)[SIZE][SIZE]);
};
#endif