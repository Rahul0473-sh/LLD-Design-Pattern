#ifndef GAME_HPP
#define GAME_HPP
#include "board.hpp"

class Game{
    private:
    Board board;
    Color currentTurn;
    bool gameOver;

    public:
    Game();
    void start();
    void switchTurn();
    bool makeMove(Position from , Position to);
    Color getCurrentTurn() const;
    bool isGameOver() const;
    void displayBoard() const;
};

#endif