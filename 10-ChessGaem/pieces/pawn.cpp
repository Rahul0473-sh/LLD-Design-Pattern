#include "pawn.hpp"
#include <bits/stdc++.h>
using namespace std;

Pawn :: Pawn(Color color, Position position): Piece(PieceType::PAWN,color, position){};
bool Pawn:: isValidMove(Position newPositon, Piece* board[8][8])const{

    Position currPos=getPosition();
    int direction=(getColor()==Color::WHITE)?1:-1;
    int dx=newPositon.getX()-currPos.getX();
    int dy=newPositon.getY()-currPos.getY();

    // Normal Move Forward;
    if(dy==0 && dx==direction){
        return board[newPositon.getX()][newPositon.getY()]==nullptr;
    }
    // two steps in inital phase
    if(dy==0 && dx==2*direction && 
      ( (getColor()==Color:: WHITE&& currPos.getX()==1)||
        (getColor()==Color:: BLACK&& currPos.getX()==6)
      )
    ){
        return board[currPos.getX()+direction][currPos.getY()]==nullptr &&
        board[newPositon.getX()][newPositon.getY()]==nullptr;
    }
    // if diagonaly moves;

    if(abs(dy)==1 && dx==direction){

        Piece* targetPiece=board[newPositon.getX()][newPositon.getY()];
        if(!targetPiece || targetPiece->getColor()!=getColor()) return true;
    }
    return false;
}