#include "bishop.hpp";
#include <bits/stdc++.h>
using namespace std;

Bishop :: Bishop(Color color,Position position): Piece(PieceType:: BISHOP,color, position){}

bool Bishop:: isValidMove(Position newPostion,Piece* board[8][8]) const{
    if(newPostion.isValid()) return false;
    Position currPos=getPosition();

    int dx=abs(currPos.getX()-newPostion.getX());
    int dy=abs(currPos.getY()-newPostion.getY());

    if(abs(dx)==abs(dy)){
        int xStep=dx/abs(dx);
        int yStep=dx/abs(dy);

        int x=currPos.getX()+xStep;
        int y=currPos.getY()+yStep;

        while(x!=newPostion.getX() && y!=newPostion.getY()){
            if(board[x][y]!=nullptr) return false;
            x+=xStep;
            y+=yStep;
        }
        Piece *targetPiece=board[newPostion.getX()][newPostion.getY()];
        return !targetPiece || targetPiece->getColor()!=getColor();
    }
    return false;
}