#ifndef BOARD_HPP
#define BOARD_HPP
#include <bits/stdc++.h>

#include "piece.hpp"

class Board{
    private:
    Piece* squared[8][8];
    vector<Piece*>capturedPieces;

    public:
    Board();
    ~Board();
    void initialize();
    bool movePiece(Position from,Position to);
    Piece* getPiece(Position position) const;
    void displayBoard() const;

    private:
    void placePiece(Piece* piece);
    void setUpInitialPosition();
    
};

#endif