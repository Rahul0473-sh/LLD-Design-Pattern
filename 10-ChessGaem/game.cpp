#include <game.hpp>
#include<bits/stdc++.h>
using namespace std;

Game:: Game(): currentTurn(Color::WHITE), gameOver(false)  {
    board.initialize();
}
void Game:: start(){
    while(!gameOver){
        displayBoard();

        cout<<((currentTurn==Color::WHITE)?"WHITE":"BLACK")<<"'s turn"<<endl;
        int fromX,fromY, toX, toY;
        cout<<"Enter Move (FromX FromY ToX ToY)"<<endl;
        cin>>fromX>>fromY>>toX>>toY;

        Position from(fromX, fromY);
        Position to(toX,toY);

        if(makeMove(from,to)){
            switchTurn();
        }
        else{
            cout<<"Invalid Move Try again"<<endl;
        }
    }
};

bool Game::makeMove(Position from, Position to){
    Piece * piece=board.getPiece(from);
    if(!piece || piece->getColor()!=currentTurn) return false;
    
    return board.movePiece(from,to);
}
void Game:: switchTurn(){
    currentTurn=(currentTurn==Color::BLACK? Color::WHITE: Color::BLACK);
}
Color Game:: getCurrentTurn() const{
    return currentTurn;
}
bool Game:: isGameOver()const {
    return gameOver;
}
void Game:: displayBoard() const{
    board.displayBoard();
}