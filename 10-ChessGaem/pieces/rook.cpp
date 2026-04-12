#include "rook.hpp"
#include "bits/stdc++.h"
using namespace std;

Rook:: Rook(Color color, Position position) :Piece(PieceType:: ROOK, color, position){};
bool Rook:: isValidMove(Position newPos,Piece* board[8][8]) const{
    if(!newPos.isValid()) return false;
    
    Position currPos=getPosition();
    int dx=(newPos.getX()-currPos.getX());
    int dy=(newPos.getY()-currPos.getY());

    // it horizonatly and vertically
    if(dx==0 || dy==0){
        int xStep=dx==0?0:dx/abs(dx);
        int yStep=dy==0?0:dy/abs(dy);

        int x=currPos.getX()+xStep;
        int y=currPos.getY()+yStep;

        while(x!=newPos.getX() || y!=newPos.getY()){
            if(board[x][y]!=nullptr) return false;
 
            x+=xStep;
            y+=yStep;
        }
        Piece *targetPiece=board[newPos.getX()][newPos.getY()];
        return !targetPiece || targetPiece->getColor()!=getColor();

    }
    return false;

}