#include <bits/stdc++.h>
using namespace std;

class Player{
    string id;
    int currentPostion;

    public:
    Player(string id, int currentPosition){
        this->id=id;
        this->currentPostion=currentPosition;
    }
   string  getPlayerId(){return id;};
   int getCurrentPostion(){return currentPostion;}


};
class Jump{
    public:
    int start, end;
    Jump(){
        this->start=0;
        this->end=0;
    }
    Jump(int start,int end){
        this->start=start;
        this->end=end;
    }
    int getJumpStart(){return start;}
    int getJumpEnd(){ return end;}
};

class Cell{
    public:
    Jump jump;
};
class Dice{
    int diceCount;
    int min=1;
    int max=6;

    public:
    Dice(){
        this->diceCount=1;
    }
    Dice(int diceCount){
        this->diceCount=diceCount;
    }
    int rollDice(){
        int totalsum=0;
        int diceUsed=0;

        while(diceUsed<diceCount){
            srand(time(nullptr));
            totalsum+=rand()%6+1;
            diceUsed++;
        }
        return totalsum;
    }
};

class Board{
    vector<vector<Cell>>cells;
    public:
    Board(){
        intializeCells(10);
        addSnakeAndLadders(5,5);
    }
    Board(int boardSize, int numberOfLadders, int numberOfSnakes){
        intializeCells(boardSize);
        addSnakeAndLadders(numberOfLadders,numberOfSnakes);
        
    }
    void intializeCells(int n){
        cells.resize(n,vector<Cell>(n));

        for(int i=0;i<n;i++){
            for(int j=0;j<n;j++){
                cells[i][j]=Cell();
            }
        }
    }
    void addSnakeAndLadders(int snakes, int ladders){
        int n=cells.size();
        int maxcell=n*n-1;
        while(snakes>0){
            srand(time(nullptr));
            int snakeHead=rand()%maxcell+1;
            int snakeTail=rand()%maxcell+1;
            
            if(snakeTail>=snakeHead) continue;
            Jump  snakeObj=Jump();
            snakeObj.start=snakeHead;
            snakeObj.end=snakeTail;

            Cell cell=getCell(snakeHead);
            cell.jump=snakeObj;

            snakes--;
        }

        while(ladders>0){
            srand(time(nullptr));
            int ladderStart=rand()%maxcell+1;
            int ladderEnd=rand()%maxcell+1;
            
            if(ladderEnd<=ladderStart) continue;
            Jump  ladderObj=Jump();
            ladderObj.start=ladderStart;
            ladderObj.end=ladderEnd;

            Cell cell=getCell(ladderStart);
            cell.jump=ladderObj;

            ladders--;
        }
    }
    Cell getCell(int playerPosition){
        int boardrow=playerPosition/cells.size();
        int boardcol=playerPosition%cells.size();   

        return cells[boardrow][boardcol];
    }
};
class Game{
    Board board;
    list<Player>players;
    Dice dice;
    public:
    Game(){
        intializeGame();
    }
    void intializeGame(){
       board = Board(10, 5, 4);
       dice=Dice(1);
       addPlayers();

    }
    void addPlayers(){
        players.push_back(Player('id-1',0));
        players.push_back(Player('id-1',0));
    }
};

int main(){
    return 0;
}