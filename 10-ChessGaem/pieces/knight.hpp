#ifndef KNIGHT_HPP
#define KNIGHT_HPP
#include "10-ChessGaem/piece.hpp"


class Knight: public Piece{
    public:
    Knight(Color color,Position position);
    bool isValidMove(Position newPostion, Piece * board[8][8]) const override;
};

#endif