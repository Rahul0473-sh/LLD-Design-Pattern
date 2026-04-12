#ifndef KING_HPP
#define KING_CPP

#include "10-ChessGaem/piece.hpp"
class King: public Piece{
    public:
    King(Color color ,Position postion);
    bool isValidMove(Position newPostion,Piece* board[8][8]) const;
};
#endif