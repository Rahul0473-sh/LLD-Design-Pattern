#ifndef PIECE_HPP
#define PIECE_HPP

#include <bits/stdc++.h>
#include "position.hpp"
using namespace std;

enum class PieceType{
    KING, QUEEN,BISHOP,KNIGHT,ROOK,PAWN
};
enum class Color{
    WHITE,BLACK
};
class Piece{
    PieceType  type;
    Color color;
    Position position;
    bool captured;

    public:
    Piece(PieceType type, Color color,Position position):type(type),
    color(color),position(position),captured(false){}
    
    // virtual ~Piece=default;

    PieceType getType() const{return type;}
    Color getColor()const{return color;}
    bool isCaptured() const {return captured;}
    Position getPosition() const {return position;}
    void setPosition(Position newPosition){
        position=newPosition;
    }
    void setCaptured(bool status){
        captured=status;
    }
    virtual bool isValidMove(Position newPosition,Piece* board[8][8]) const=0;

    string getSymbol(){
        char symbol;
        switch (type)
        {
            case PieceType:: KING: symbol='K';break;
            case PieceType:: QUEEN: symbol='Q';break;
            case PieceType:: BISHOP: symbol='B';break;
            case PieceType:: ROOK: symbol='R';break;
            case PieceType:: KNIGHT: symbol='N';break;
            case PieceType:: PAWN: symbol='P';break;
            
            default:symbol='?';
        }
        return string(1,color==Color:: WHITE?symbol:tolower(symbol));
    }
};


#endif