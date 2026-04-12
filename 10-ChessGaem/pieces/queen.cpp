#include "queen.hpp"
#include <bits/stdc++.h>
using namespace std;


Queen :: Queen(Color color, Position position): Piece(PieceType::QUEEN,color,position){};
bool Queen:: isValidMove(Position newPostion, Piece* board[8][8])const {
    if(!newPostion.isValid()) return false;
    Position currPos=getPosition();

    int dx=newPostion.getX()-currPos.getX();
    int dy=newPostion.getY()-currPos.getY();

    if((abs(dx)==abs(dy)) || dx==0 || dy==0){
        int xStep=dx/abs(dx);
        int yStep=dy/abs(dy);

        int x=currPos.getX()+xStep;
        int y=currPos.getY()+yStep;

        while   (x!=newPostion.getX() || y!=newPostion.getY()){
            if(board[x][y]!=nullptr) return false;

            x+=xStep;
            y+=yStep;
        }
        Piece *targetPiece=board[newPostion.getX()][newPostion.getY()];
        return !targetPiece || targetPiece->getColor()!=getColor();
    }
    return false;
}