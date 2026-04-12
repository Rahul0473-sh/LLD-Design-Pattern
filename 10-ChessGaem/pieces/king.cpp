#include "king.hpp";
#include "bits/stdc++.h"
using namespace std;

King:: King(Color color, Position position) :Piece(PieceType:: KING, color, position){};
bool King:: isValidMove(Position newPos,Piece* board[8][8]) const{
    if(!newPos.isValid()) return false;
    
    Position currPos=getPosition();
    int dx=(newPos.getX()-currPos.getX());
    int dy=(newPos.getY()-currPos.getY());

    if(abs(dx)<=1 && abs(dx)<=1){
        int xStep=dx/abs(dx);
        int yStep=dy/abs(dy);

        int x=currPos.getX()+xStep;
        int y=currPos.getY()+yStep;

        while(x!=newPos.getX() && y!=newPos.getY()){
            if(board[x][y]!=nullptr) return false;

            x+=xStep;
            y+=yStep;
        }
        Piece *targetPiece=board[newPos.getX()][newPos.getY()];
        return !targetPiece || targetPiece->getColor()!=getColor();

    }
    return false;

}