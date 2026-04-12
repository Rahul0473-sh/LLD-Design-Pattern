#ifndef KNIGHT_HPP
#define KNIGHT_HPP
#include "../piece.hpp"


class Knight: public Piece{
    public:
    Knight(Color color,Position position);
    bool isValidMove(Position newPostion, Piece * board[8][8]) const override;
};

#endif