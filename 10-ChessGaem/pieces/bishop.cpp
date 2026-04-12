#include "bishop.hpp"
#include <bits/stdc++.h>
using namespace std;

Bishop::Bishop(Color color, Position position) : Piece(PieceType::BISHOP, color, position) {}

bool Bishop::isValidMove(Position newPosition, Piece* board[8][8]) const {
    if (!newPosition.isValid()) return false;
    Position currPos = getPosition();

    int dx = newPosition.getX() - currPos.getX();
    int dy = newPosition.getY() - currPos.getY();

    if (dx == 0 || dy == 0) return false;
    if (abs(dx) != abs(dy)) return false;

    int xStep = (dx > 0) ? 1 : -1;
    int yStep = (dy > 0) ? 1 : -1;

    int x = currPos.getX() + xStep;
    int y = currPos.getY() + yStep;

    while (x != newPosition.getX() || y != newPosition.getY()) {
        if (board[x][y] != nullptr) return false;
        x += xStep;
        y += yStep;
    }
    Piece* targetPiece = board[newPosition.getX()][newPosition.getY()];
    return !targetPiece || targetPiece->getColor() != getColor();
}
