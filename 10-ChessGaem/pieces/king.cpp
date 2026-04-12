#include "king.hpp"
#include "bits/stdc++.h"
using namespace std;

King::King(Color color, Position position) : Piece(PieceType::KING, color, position) {}

bool King::isValidMove(Position newPos, Piece* board[8][8]) const {
    if (!newPos.isValid()) return false;

    Position currPos = getPosition();
    int dx = abs(newPos.getX() - currPos.getX());
    int dy = abs(newPos.getY() - currPos.getY());
    if (dx > 1 || dy > 1 || (dx == 0 && dy == 0)) return false;

    Piece* targetPiece = board[newPos.getX()][newPos.getY()];
    return !targetPiece || targetPiece->getColor() != getColor();
}
