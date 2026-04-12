#ifndef KING_HPP
#define KING_HPP

#include "../piece.hpp"
class King: public Piece{
    public:
    King(Color color ,Position postion);
    bool isValidMove(Position newPostion, Piece* board[8][8]) const override;
};
#endif