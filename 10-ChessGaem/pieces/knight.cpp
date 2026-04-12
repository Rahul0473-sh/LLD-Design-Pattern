#include "knight.hpp"
#include "bits/stdc++.h"
using namespace std;


Knight:: Knight(Color color,Position position):Piece(PieceType::KNIGHT, color,position ){};
bool Knight:: isValidMove(Position newPos,Piece* board[8][8]) const{
    if(!newPos.isValid()) return false;
    
    Position currPos=getPosition();
    int dx=abs(newPos.getX()-currPos.getX());
    int dy=abs(newPos.getY()-currPos.getY());

    if((dx==1 && dx==2) || (dx==2 && dx==1)){
        Piece * targetPiece=board[newPos.getX()][newPos.getY()];
        return !targetPiece || targetPiece->getColor()!=getColor();
    }
    return false;
}

