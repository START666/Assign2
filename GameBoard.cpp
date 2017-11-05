//
// Created by START on 2017/11/2.
//

//#include <cstdlib>
#include <iostream>
#include "GameBoard.h"

/*************** private ****************/

int GameBoard::nextAvailable(int x) {    //(x,y) = x + y * width

    int depth = 0;
    while (boardArr[x + depth * width] != '-') {
        depth++;
    }
    return depth;
}

char *GameBoard::getPointer(int x, int y) {
    return &boardArr[x + y * width];
}

bool GameBoard::checkVerticalAligned(int x, int y, int numToken, bool isHuman) {
    char token = isHuman ? 'X' : 'O';
    int count = 0;

    if (get(x, y) == token) {    //if the current is not 'X' or 'O', then return false, used to speed up calculation
        for (int yCoord = y - numToken; yCoord < y + numToken; yCoord++) {
            if (yCoord < 0 || yCoord >= width) continue;
            if (get(x, yCoord) == token) {
                count++;
            } else {
                count = 0;
            }
            if (count == numToken) return true;
        }
    }
    return false;
}

bool GameBoard::checkVerticalAligned(int pos, int numToken, bool isHuman) {
    int x, y;
    y = pos / width;
    x = pos - width * y;
    return checkVerticalAligned(x, y, numToken, isHuman);
}

bool GameBoard::checkHorizontalAligned(int x, int y, int numToken, bool isHuman) {
    char token = isHuman ? 'X' : 'O';
    int count = 0;
    if (get(x, y) == token) {
        for (int xCoord = x - numToken; xCoord < x + numToken; xCoord++) {
            if (xCoord < 0 || xCoord >= height) continue;
            if (get(xCoord, y) == token) {
                count++;
            } else {
                count = 0;
            }
            if (count == numToken) return true;
        }
    }
    return false;
}

bool GameBoard::checkHorizontalAligned(int pos, int numToken, bool isHuman) {
    int x, y;
    y = pos / width;
    x = pos - width * y;
    return checkHorizontalAligned(x, y, numToken, isHuman);
}

bool GameBoard::checkLeftDiagonalAligned(int x, int y, int numToken, bool isHuman) {
    char token = isHuman ? 'X' : 'O';
    int count = 0;

    if (get(x, y) == token) {
        //(x-i,y+i) && (x-i,y-i)
        for (int i = -numToken + 1; i < numToken; i++) {
            int xCoord = x - i;
            int yCoord = y + i;
            if (xCoord < 0 || xCoord >= height || yCoord < 0 || yCoord >= width) continue;
            if (get(xCoord, yCoord) == token)
                count++;
            else
                count = 0;
            if (count == numToken) return true;
        }
    }
    return false;
}

bool GameBoard::checkLeftDiagonalAligned(int pos, int numToken, bool isHuman) {
    int x, y;
    y = pos / width;
    x = pos - width * y;
    return checkLeftDiagonalAligned(x, y, numToken, isHuman);
}

bool GameBoard::checkRightDiagonalAligned(int x, int y, int numToken, bool isHuman) {
    char token = isHuman ? 'X' : 'O';
    int count = 0;

    if (get(x, y) == token) {
        //(x-i,y-i) && (x+i,y+i)
        for (int i = -numToken + 1; i < numToken; i++) {
            int xCoord = x + i;
            int yCoord = y + i;
            if (xCoord < 0 || xCoord >= height || yCoord < 0 || yCoord >= width) continue;
            if (get(xCoord, yCoord) == token)
                count++;
            else
                count = 0;
            if (count == numToken) return true;
        }

    }
    return false;
}

bool GameBoard::checkRightDiagonalAligned(int pos, int numToken, bool isHuman) {
    int x, y;
    y = pos / width;
    x = pos - width * y;
    return checkRightDiagonalAligned(x, y, numToken, isHuman);
}

bool GameBoard::isRowOneFull() {
    for (int i = 0; i < width; i++)
        if (get(i, 0) == '-') return false;
    return true;
}

void GameBoard::addLog(int pos) {
    log.push_back(pos);
    totalStep++;
}

std::ostream &GameBoard::printBoard(std::ostream &out) {
    int xCoord, yCoord;
    for (yCoord = height - 1; yCoord >= -1; yCoord--) {
        for (xCoord = 0; xCoord < width; xCoord++) {

            if (yCoord == -1) out << xCoord;
            else out << get(xCoord, yCoord);

            if (xCoord != width - 1) out << ' ';
        }
        out << std::endl;
    }

    return out;
}

/*************** public *****************/

GameBoard::GameBoard() {
    this->width = 8;
    this->height = 8;
    this->totalStep = 0;
    boardArr = (char *) malloc(sizeof(char) * width * height);
    for (int i = 0; i < width * height; i++) {
        boardArr[i] = '-';    //init empty board
    }

}

GameBoard::GameBoard(int w, int h) {
    this->width = w;
    this->height = h;
    this->totalStep = 0;
    boardArr = (char *) malloc(sizeof(char) * width * height);
    for (int i = 0; i < width * height; i++) {
        boardArr[i] = '-';    //init empty board
    }
}

GameBoard::~GameBoard() {
    free(boardArr);
}

bool GameBoard::isFull(int x) {
    if (x >= 0 && x <= width - 1) {
        return (get(x, height - 1) != '-');
    } else {
        std::cerr << "GameBoard.isFull(int x): @param x="<< x <<" should be from 0 to 7" << std::endl;
        return NULL;
    }
}

void GameBoard::dropToken(bool isHuman, int x) {
    if (!isFull(x)) {
        int y = nextAvailable(x);
        char *p = getPointer(x, y);
        *p = isHuman ? 'X' : 'O';
        std::cout << "Token dropped at " << x << std::endl;
        addLog(x);
        if (isRowOneFull() && !hasWinner()) {
            std::cout << "Row 1 is full" << std::endl;
            for (int yCoord = 1; yCoord < height; yCoord++)
                for (int xCoord = 0; xCoord < width; xCoord++) {
                    char *up = getPointer(xCoord, yCoord);
                    char *down = getPointer(xCoord, yCoord - 1);
                    *down = *up;
                }
            for (int xCoord = 0; xCoord < width; xCoord++) {
                char *pointer = getPointer(xCoord, height);
                *pointer = '-';
            }
        }

    } else {
        std::cerr << "GameBoard.dropToken(bool isHuman, int y): the y column is full" << std::endl;
    }
}

char GameBoard::get(int x, int y) {
    if (x >= 0 && x < width && y >= 0 && y < height)    //input check
        return (*getPointer(x, y));
    else
        return ' ';
}

char GameBoard::get(int pos) {
    if (pos >= 0 && pos < width * height) {
        return boardArr[pos];
    } else
        return ' ';
}

bool GameBoard::isHumanWin() {    //human is 'X'
    for (int i = 0; i < width * height; i++) {
        if (get(i) != 'X') continue;
        if (checkHorizontalAligned(i, 4, true)) return true;
        if (checkVerticalAligned(i, 4, true)) return true;
        if (checkLeftDiagonalAligned(i, 4, true)) return true;
        if (checkRightDiagonalAligned(i, 4, true)) return true;
    }
    return false;
}

bool GameBoard::isAIWin() {    //human is 'O'
    for (int i = 0; i < width * height; i++) {
        if (get(i) != 'O') continue;
        if (checkHorizontalAligned(i, 4, false)) return true;
        if (checkVerticalAligned(i, 4, false)) return true;
        if (checkLeftDiagonalAligned(i, 4, false)) return true;
        if (checkRightDiagonalAligned(i, 4, false)) return true;
    }
    return false;
}

bool GameBoard::hasWinner() {
    return (isHumanWin() || isAIWin());
}

int GameBoard::getScore(bool isHuman) {
    int score = 0;
    char self = isHuman ? 'X' : 'O';

    for (int i = 0; i < width * height; i++) {
        if (get(i) == '-') continue;
        if (get(i) == self) {    //increment
            if (checkHorizontalAligned(i, 2, isHuman)) score++;

            if (checkVerticalAligned(i, 2, isHuman)) score++;

            if (checkLeftDiagonalAligned(i, 2, isHuman)) score++;

            if (checkRightDiagonalAligned(i, 2, isHuman)) score++;

            if (checkHorizontalAligned(i, 3, isHuman)) score += 2;

            if (checkVerticalAligned(i, 3, isHuman)) score += 2;

            if (checkLeftDiagonalAligned(i, 3, isHuman)) score += 2;

            if (checkRightDiagonalAligned(i, 3, isHuman)) score += 2;

            if (isHuman ? isHumanWin() : isAIWin()) score += 10000;    //if wins, add a significantly large value

        } else {    //decrement
            if (checkHorizontalAligned(i, 2, !isHuman)) score--;

            if (checkVerticalAligned(i, 2, !isHuman)) score--;

            if (checkLeftDiagonalAligned(i, 2, !isHuman)) score--;

            if (checkRightDiagonalAligned(i, 2, !isHuman)) score--;

            if (checkHorizontalAligned(i, 3, !isHuman)) score -= 2;

            if (checkVerticalAligned(i, 3, !isHuman)) score -= 2;

            if (checkLeftDiagonalAligned(i, 3, !isHuman)) score -= 2;

            if (checkRightDiagonalAligned(i, 3, !isHuman)) score -= 2;

            if (!isHuman ? isHumanWin() : isAIWin()) score -= 10000;    //if wins, subtract a significantly large value
        }
    }

    return score;

}

int GameBoard::getWidth() {
    return width;
}

int GameBoard::getHeight() {
    return height;
}

int GameBoard::getLastPos() {
    if (totalStep != 0) return log.back();
    else return -1;
}

std::ostream &operator<<(std::ostream &out, GameBoard &gameBoard) {
    return gameBoard.printBoard(out);
}