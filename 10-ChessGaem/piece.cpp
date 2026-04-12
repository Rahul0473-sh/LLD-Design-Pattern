#include "piece.hpp";
#include <bits/stdc++.h>
using namespace std;

Piece::Piece(PieceType type, Color color, Position position) : type(type),
                                                               color(color), position(position), captured(false) {}

PieceType Piece::getType() const { return type; }
bool Piece:: isCaptured() const { return captured; }
Position Piece:: getPosition() const { return position; }
void Piece:: setPosition(Position newPosition){
    position = newPosition;
}
void Piece:: setCaptured(bool status){
    captured = status;
}
string Piece:: getSymbol(){
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

