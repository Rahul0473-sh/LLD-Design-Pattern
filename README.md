# Chess (LLD demo)

Small C++ chess sketch: console UI, polymorphic pieces, board and game loop.

## Build and run

From this directory:

```bash
g++ chessDemo.cpp game.cpp board.cpp position.cpp pieces/pawn.cpp pieces/rook.cpp pieces/knight.cpp pieces/bishop.cpp pieces/queen.cpp pieces/king.cpp -o chessDemo
./chessDemo
```

On Windows PowerShell:

```powershell
g++ chessDemo.cpp game.cpp board.cpp position.cpp pieces/pawn.cpp pieces/rook.cpp pieces/knight.cpp pieces/bishop.cpp pieces/queen.cpp pieces/king.cpp -o chessDemo
.\chessDemo.exe
```

Enter moves as four integers: `FromX FromY ToX ToY` (coordinates match the printed board). Enter `-1 -1 -1 -1` to quit.

## UML — class diagram

```mermaid
classDiagram
    direction TB

    class Game {
        -Board board
        -Color currentTurn
        -bool gameOver
        +Game()
        +start()
        +makeMove(Position, Position) bool
        +switchTurn()
        +getCurrentTurn() Color
        +isGameOver() bool
        +displayBoard()
    }

    class Board {
        -Piece* squared[8][8]
        -vector~Piece*~ capturedPieces
        +Board()
        +~Board()
        +initialize()
        +movePiece(Position, Position) bool
        +getPiece(Position) Piece*
        +displayBoard()
        -placePiece(Piece*)
        -setUpInitialPosition()
    }

    class Position {
        -int x
        -int y
        +Position(int x, int y)
        +getX() int
        +getY() int
        +isValid() bool
        +operator==(Position) bool
    }

    class Piece {
        <<abstract>>
        #PieceType type
        #Color color
        #Position position
        #bool captured
        +Piece(PieceType, Color, Position)
        +~Piece()*
        +getType() PieceType
        +getColor() Color
        +getPosition() Position
        +setPosition(Position)
        +isCaptured() bool
        +setCaptured(bool)
        +getSymbol() string
        +isValidMove(Position, Piece*[][]) bool*
    }

    class Pawn
    class Rook
    class Knight
    class Bishop
    class Queen
    class King

    Game *-- Board : owns
    Board o-- Piece : squares~8x8~
    Board o-- Piece : captured
    Piece --> Position
    Piece <|-- Pawn
    Piece <|-- Rook
    Piece <|-- Knight
    Piece <|-- Bishop
    Piece <|-- Queen
    Piece <|-- King
```

**Enums** (in `piece.hpp` with `Piece`): `PieceType`, `Color`.
