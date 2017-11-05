//
// Created by START on 2017/11/2.
//

#ifndef INC_4F00ASSIGN2_GAMEBOARD_H
#define INC_4F00ASSIGN2_GAMEBOARD_H


#include <vector>

class GameBoard {
private:
    int width, height;
    std::vector<int> log;
    int totalStep;
    char *boardArr;    // empty:'-'    human:'X'    ai:'O'
    int nextAvailable(int x);    //return the next available place
    char *getPointer(int x, int y);    //return the pointer of the location
    bool checkVerticalAligned(int x, int y, int numToken, bool isHuman);    //check Vertical Aligned
    bool checkVerticalAligned(int pos, int numToken, bool isHuman);

    bool checkHorizontalAligned(int x, int y, int numToken, bool isHuman);    //check Vertical Aligned
    bool checkHorizontalAligned(int pos, int numToken, bool isHuman);

    bool checkLeftDiagonalAligned(int x, int y, int numToken, bool isHuman);    //check Vertical Aligned
    bool checkLeftDiagonalAligned(int pos, int numToken, bool isHuman);

    bool checkRightDiagonalAligned(int x, int y, int numToken, bool isHuman);    //check Vertical Aligned
    bool checkRightDiagonalAligned(int pos, int numToken, bool isHuman);

    bool isRowOneFull();

    void addLog(int pos);

    std::ostream& printBoard(std::ostream& out);

public:

    GameBoard();    //constructor
    GameBoard(int w, int h);    //constructor
    ~GameBoard();    //destructor
    bool isFull(int x);    //check if y column is full
    void dropToken(bool isHuman, int x);    //drop token at x column
    char get(int x, int y);    //get char at (x,y)
    char get(int pos);

    bool isHumanWin();    //check if Human wins
    bool isAIWin();    //check if AI wins
    bool hasWinner();    //check if this board has a winner
    int getScore(bool isHuman);    //get the evaluation score for the board
    int getWidth();
    int getHeight();
    int getLastPos();

    friend std::ostream& operator<<(std::ostream &out, GameBoard &gameBoard);

};


#endif //INC_4F00ASSIGN2_GAMEBOARD_H
