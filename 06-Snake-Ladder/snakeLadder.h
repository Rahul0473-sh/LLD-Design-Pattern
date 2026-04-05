#include <bits/stdc++.h>
#include <thread>
#include <chrono>
using namespace std;

class Board{
    private:
    unordered_map<int,int>snakes;
    unordered_map<int,int>ladders;
    int size;

    public:
    Board(int size){
        this->size=size;
    }
    int getsize() {
        return this->size;
    }
    void addSnake(int start,int end){
        snakes[start]=end;
    }
    void addLadder(int start, int end){
        ladders[start]=end;
    }
    int getNextPosition(int currPosition){
        auto snakeit=snakes.find(currPosition);
        if(snakeit!=snakes.end()){
            cout<<"You Hit the snake, you need to go down"<<endl;
            return snakeit->second;
        }
        auto ladderit=ladders.find(currPosition);
        if(ladderit!=ladders.end()){
            cout<<"You Hit the Ladder, you need to ladder Up"<<endl;
            return ladderit->second;
        }
        return currPosition;
    }
    void DisplayInfo(){
        cout<<"\nBoard Infromation"<<endl;
        cout<<"\n size "<<size<<" squares"<<endl;

        cout << "\nSnakes:" << endl;
        for (const auto &snake : snakes)
        {
            cout << "From " << snake.first << " to " << snake.second << endl;
        }

        cout << "\nLadders:" << endl;
        for (const auto &ladder : ladders)
        {
            cout << "From " << ladder.first << " to " << ladder.second << endl;
        }
    }

};
class Dice{
    int sides;
    public:
    Dice(int sides=6) {
        this->sides=sides;
    }
    int roll(){
        return rand()%sides+1;
    }
};
class Player{
    string name;
    int position;
    bool winner=false;

    public:
    Player(string name) : name(name), position(0), winner(false) {}
    
    string getName(){
        return this->name;
    }
    int getPosition(){
        return position;
    }
    bool isWinner(){
        return winner;
    }
    void setPosition(int position){
        this->position=position;
    }
    void setWinner(bool status){
        winner=status;
    }
    void displayInfo() {
        cout << "Player: " << name << endl;
        cout << "Position: " << position << endl;
        cout << "Status: " << (winner ? "Winner!" : "Playing") << endl;
    }
};
class Game{
    private:
    Board board;
    vector<Player*>players;
    Dice dice;
    int currentPlayerIdx;
    bool gameOver;

    public:
    Game(int boardsize=100): board(boardsize),currentPlayerIdx(0),gameOver(false){};

    void addPlayer(const string& name){
        players.push_back(new Player(name));
    }
    void setupBoard(){
    board.addSnake(99, 10);
    board.addSnake(95, 75);
    board.addSnake(92, 88);
    board.addSnake(89, 68);
    board.addSnake(74, 53);
    board.addSnake(62, 19);
    board.addSnake(46, 25);
    board.addSnake(49, 11);
    
    // Add ladders
    board.addLadder(2, 38);
    board.addLadder(7, 14);
    board.addLadder(8, 31);
    board.addLadder(15, 26);
    board.addLadder(21, 42);
    board.addLadder(28, 84);
    board.addLadder(36, 44);
    board.addLadder(51, 67);
    board.addLadder(71, 91);
    board.addLadder(78, 98);

    board.DisplayInfo();
    }
    bool makeMove(){
        if(gameOver || players.empty()) return false;
        Player * currentPlayer=getCurrentPlayer();

        cout << "\n" << currentPlayer->getName() << "'s turn" << endl;
        int roll=dice.roll();
        cout<<" Rolled "<<roll<< endl;
        movePlayer(currentPlayer, roll);

        if (checkWin(currentPlayer))
        {
            currentPlayer->setWinner(true);
            gameOver = true;
            cout << "\nCongratulations! " << currentPlayer->getName() << " wins!" << endl;
            return false;
        }
        currentPlayerIdx = (currentPlayerIdx + 1) % players.size();
        return true;
    }
    void play(){
        if(players.empty()){
            cout<<"\n No Players are Playing"<<endl;
            return;
        }

        cout<<"\nStarting the Game"<<endl;
        while(makeMove()){
            displayStatus();
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
    void movePlayer(Player * player, int steps ){
        int newPostion=player->getPosition()+steps;
        if(newPostion>board.getsize()){
            cout<<"Cannot Move , need exact number to win\n";
            return;
        }
        newPostion=board.getNextPosition(newPostion);
        player->setPosition(newPostion);

        cout << player->getName() << " moved to position " << newPostion << endl;
    }
    
    void displayStatus(){
        cout<<"Current Game Status"<<endl;
        for(const auto& player:players){
            cout << player->getName() << " at position " << player->getPosition() << endl;
        }
    }
    bool checkWin(Player* player){
        return player->getPosition()==board.getsize();
    }
    Player* getCurrentPlayer(){
        return players[currentPlayerIdx];
    }
};


// class
int main(){
     srand(time(0));

    Game game;
    game.addPlayer("Rahul");
    game.addPlayer("Ritik");
    game.setupBoard();
    game.play();
}