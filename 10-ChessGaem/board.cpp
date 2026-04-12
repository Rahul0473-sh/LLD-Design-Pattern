#include "board.hpp"

#include "pieces/bishop.hpp"
#include "pieces/king.hpp"
#include "pieces/knight.hpp"
#include "pieces/pawn.hpp"
#include "pieces/queen.hpp"
#include "pieces/rook.hpp"

using namespace std;

Board::Board() {
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            squared[i][j] = nullptr;
        }
    }
}

Board::~Board() {
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            delete squared[i][j];
            squared[i][j] = nullptr;
        }
    }
    for (Piece* p : capturedPieces) {
        delete p;
    }
    capturedPieces.clear();
}

void Board::placePiece(Piece* piece) {
    if (!piece) return;
    Position pos = piece->getPosition();
    squared[pos.getX()][pos.getY()] = piece;
}

void Board::setUpInitialPosition() {
    for (int y = 0; y < 8; ++y) {
        placePiece(new Pawn(Color::WHITE, Position(1, y)));
        placePiece(new Pawn(Color::BLACK, Position(6, y)));
    }

    placePiece(new Rook(Color::WHITE, Position(0, 0)));
    placePiece(new Knight(Color::WHITE, Position(0, 1)));
    placePiece(new Bishop(Color::WHITE, Position(0, 2)));
    placePiece(new Queen(Color::WHITE, Position(0, 3)));
    placePiece(new King(Color::WHITE, Position(0, 4)));
    placePiece(new Bishop(Color::WHITE, Position(0, 5)));
    placePiece(new Knight(Color::WHITE, Position(0, 6)));
    placePiece(new Rook(Color::WHITE, Position(0, 7)));

    placePiece(new Rook(Color::BLACK, Position(7, 0)));
    placePiece(new Knight(Color::BLACK, Position(7, 1)));
    placePiece(new Bishop(Color::BLACK, Position(7, 2)));
    placePiece(new Queen(Color::BLACK, Position(7, 3)));
    placePiece(new King(Color::BLACK, Position(7, 4)));
    placePiece(new Bishop(Color::BLACK, Position(7, 5)));
    placePiece(new Knight(Color::BLACK, Position(7, 6)));
    placePiece(new Rook(Color::BLACK, Position(7, 7)));
}

void Board::initialize() {
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            delete squared[i][j];
            squared[i][j] = nullptr;
        }
    }
    for (Piece* p : capturedPieces) {
        delete p;
    }
    capturedPieces.clear();

    setUpInitialPosition();
}

Piece* Board::getPiece(Position position) const {
    if (!position.isValid()) return nullptr;
    return squared[position.getX()][position.getY()];
}

bool Board::movePiece(Position from, Position to) {
    if (!from.isValid() || !to.isValid()) return false;
    if (from.getX() == to.getX() && from.getY() == to.getY()) return false;

    Piece* piece = squared[from.getX()][from.getY()];
    if (!piece) return false;

    if (!piece->isValidMove(to, squared)) return false;

    Piece* target = squared[to.getX()][to.getY()];
    if (target) {
        if (target->getColor() == piece->getColor()) return false;
        target->setCaptured(true);
        capturedPieces.push_back(target);
    }

    squared[from.getX()][from.getY()] = nullptr;
    squared[to.getX()][to.getY()] = piece;
    piece->setPosition(to);
    return true;
}

void Board::displayBoard() const {
    for (int x = 7; x >= 0; --x) {
        cout << x << " ";
        for (int y = 0; y < 8; ++y) {
            Piece* p = squared[x][y];
            cout << (p ? p->getSymbol() : string(".")) << " ";
        }
        cout << "\n";
    }
    cout << "  ";
    for (int y = 0; y < 8; ++y) {
        cout << y << " ";
    }
    cout << "\n";
}
