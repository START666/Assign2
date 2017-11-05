#include <iostream>
#include <thread>
#include <mutex>
#include "GameBoard.h"

using namespace std;
GameBoard gameBoard;
int humanMove;
mutex humanMovingLock;
int aiMove;
mutex aiMovingLock;
bool hasWinner;
unsigned int *aiDecision;
mutex aiDecisionLock;

void aiCalc() {    //calculate the next step and store the decision into an array
    while (!hasWinner) {
        aiDecisionLock.lock();
        for (int i = 0; i < gameBoard.getWidth(); i++) {
            aiDecision[i] = (unsigned int) (rand() % 8);
        }

        aiDecisionLock.unlock();
    }
}

void aiDisplay() {    //get final decision array from aiCalc and send command to gameMaster
//PvE
    while (!hasWinner) {
        aiMovingLock.lock();
        aiDecisionLock.lock();
        if(hasWinner) break;

        int prevHumanMove = gameBoard.getLastPos();

        aiMove = aiDecision[prevHumanMove];

        aiMovingLock.unlock();    //GM thread will get the lock
        std::this_thread::sleep_for(
                std::chrono::milliseconds(200));    //to make sure GM thread get the lock and display the current move
        aiDecisionLock.unlock();    //aiCalc thread will get the lock
    }

//PvP
//    while (!hasWinner) {
//        aiMovingLock.lock();
//
//        int input;
//        do {
//            cout << "Please choose a location to put token: " << endl;
//            cin >> input;
//
//            while (cin.fail()) {    //integer check
//                cerr << "Please input an integer!" << endl;
//                cin.clear();
//                cin.ignore(256, '\n');
//                cin >> input;
//            }
//
//            if (input < 0 || input > 7 )
//                cerr << "Please input 0 or 7." << endl;
//
//        } while (input < 0 || input > 7 );
//
//        aiMove = input;
//        aiMovingLock.unlock();
//        std::this_thread::sleep_for(std::chrono::milliseconds(20));    //to make sure GM thread get the lock and display the current move
//    }
}

void human() {    //get user input and send command to gameMaster
    while (!hasWinner) {
        humanMovingLock.lock();
        if(hasWinner) break;

        int input;
        in:
        do {
            cout << "Please choose a location to put token: " << endl;
            cin >> input;

            while (cin.fail()) {    //integer check
                cerr << "Please input an integer!" << endl;
                cin.clear();
                cin.ignore(256, '\n');
                cin >> input;
            }

            if (input < 0 || input > 7)
                cerr << "Please input 0 or 7." << endl;

        } while (input < 0 || input > 7);

        humanMove = input;
        if (gameBoard.isFull(humanMove)) {
            cout << "The column is full!" << endl;
            goto in;
        }
        humanMovingLock.unlock();
        std::this_thread::sleep_for(
                std::chrono::milliseconds(20));    //to make sure GM thread get the lock and display the current move
    }
}

void gameMaster(bool isHumanFirst) {    //receive command from players and display the game board
    bool prevHuman = !isHumanFirst;
    bool isHumanWin;
    humanMovingLock.lock();
    aiMovingLock.lock();

    cout << (isHumanFirst ? "Human moves First" : "AI moves First") << endl;

    hasWinner = false;
    while (!gameBoard.hasWinner()) {
        humanMove = -1;
        aiMove = -1;
        if (!prevHuman) {
            cout << gameBoard << endl;
            cout << "X Move: ";
            humanMovingLock.unlock();    //human thread will get the lock
            std::this_thread::sleep_for(std::chrono::milliseconds(20));    //to make sure human thread get the lock
            humanMovingLock.lock();
        } else {
//PvP
//            cout << gameBoard << endl;
//            cout << "O Move: ";
            aiMovingLock.unlock();    //aiDisplay thread will get the lock
            std::this_thread::sleep_for(std::chrono::milliseconds(20));    //to make sure ai thread get the lock
            aiMovingLock.lock();
        }
        gameBoard.dropToken(!prevHuman, !prevHuman ? humanMove : aiMove);
        if (gameBoard.hasWinner()) break;
        prevHuman = !prevHuman;
    }
    hasWinner = true;
    isHumanWin = !prevHuman;

    cout << gameBoard;
    cout << (isHumanWin ? "Human Wins" : "AI Wins") << endl;
    humanMovingLock.unlock();
    aiMovingLock.unlock();
}

bool menu() {    //display the dialogue, let user to choose AI first or human first
    int input;
    do {
        cout << "Please choose who moves first: " << endl;
        cout << "    0.Human    1.AI" << endl;
        cin >> input;

        while (cin.fail()) {    //integer check
            cerr << "Please input an integer!" << endl;
            cin.clear();
            cin.ignore(256, '\n');
            cin >> input;
        }

        if (input != 0 && input != 1)
            cerr << "Please input 0 or 1." << endl;

    } while (input != 0 && input != 1);
    return (input == 0);

}

int main() {

    bool isHumanFirst = menu();
    aiDecision = (unsigned int *) (malloc(sizeof(unsigned int) * 8));

    thread gameMasterThread(gameMaster, isHumanFirst);
    //std::this_thread::sleep_for(std::chrono::milliseconds(20));
    thread aiCalcThread(aiCalc);
    thread aiDisplayThread(aiDisplay);
    thread humanThread(human);

    aiCalcThread.join();
//    cout << "aiCalcThread joined" << endl;
    aiDisplayThread.join();
//    cout << "aiDisplayThread joined" << endl;
    humanThread.join();
//    cout << "humanThread joined" << endl;
    gameMasterThread.join();
//    cout << "gameMasterThread joined" << endl;

//    cout << "Main Thread end." << endl;
    return 0;
}