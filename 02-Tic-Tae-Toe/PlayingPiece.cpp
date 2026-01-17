#include <bits/stdc++.h>
using namespace std;

enum class PieceType{
    X,
    O
};
class PlayingPiece{
    public:
    PieceType piece;

    PlayingPiece (PieceType piece){
        this->piece=piece;
    }
    virtual char getChar()=0;
};
class PlayingPieceX :public PlayingPiece{
    public:
    PlayingPieceX() : PlayingPiece(PieceType::X) {}

    char getChar() override{
        return 'X';
    }
};
class PlayingPieceO :public PlayingPiece{
    public:
    PlayingPieceO() : PlayingPiece(PieceType::O){}

    char getChar() override{
        return 'O';
    }
};


class Board{
    public:
    int n;
    vector<vector<PlayingPiece *>>grid;

    // bool addPiece(int r,int c,PlayingPiece * ch);

    Board(int n){
        this->n=n;
        grid.resize(n,vector<PlayingPiece*>(n,nullptr));
    }
    vector<pair<int,int>> getFreeCells(){
        vector<pair<int,int>>nums;
        for(int i=0;i<n;i++){
            for(int j=0;j<n;j++){
                if(grid[i][j]==nullptr){
                    nums.push_back({i,j});
                }
            }
        }
        return nums;
    }
    bool addPiece(int r, int c, PlayingPiece * piece){
        if(r<0 || r>=n || c<0 || c>=n || grid[r][c]!=nullptr) return false;
        grid[r][c]=piece;
        return true;

    }

    bool CheckWinner(int r,int c,PlayingPiece *piece){
        // down
        char ch=piece->getChar();
        bool col=true;
        for(int i=0;i<n;i++){
            if(grid[i][c]==NULL || grid[i][c]->getChar()!=ch) {
                col=false;
                break;
            }
        }
        if(col) return true;
        bool row=true;
        
        // row
        for(int j=0;j<n;j++){
            if(grid[r][j]==NULL || grid[r][j]->getChar() !=ch){
                row=false;
                break;
            }
        }
        if(row) return true;
        // digonal
        
        if(r==c){
            bool diagonal=true;
            for(int i=0;i<n;i++){
                if(grid[i][i]==NULL || grid[i][i]->getChar()!=ch){
                    diagonal=false;
                    break;
                }
            }
            if(diagonal) return true;
        }
        // Antidiagonal

        if(r+c==n-1){
            bool antidiagonal=true;
            for(int i=0;i<n;i++){
                if(grid[i][n-i-1]==NULL || grid[i][n-i-1]->getChar()!=ch){
                    antidiagonal=false;
                    break;
                }
            }
            if(antidiagonal) return true;
        }
        return false;
    }
    void PrintBoard(){
        cout<<"\n";
        for(int i=0;i<n;i++){
            for(int j=0;j<n;j++){
                if(grid[i][j]==NULL){
                    cout<<".";
                }else{
                    cout<<grid[i][j]->getChar();
                }
                if(j!=n-1) cout<<" | ";
            }
            cout<<endl;
        }
        cout<<endl;
    }
};
class Player{
    public:
    string name;
    PlayingPiece * character;

    Player(string name, PlayingPiece * ptr){
        this->name=name;
        character=ptr;
    }
};
class Tic_TAC_TOE{

    list<Player *>players;
    Board  *board;

    public:
    Tic_TAC_TOE(Player * ptr1, Player * ptr2,int n): board (new Board(n)){
        players.push_back(ptr1);
        players.push_back(ptr2);
    };
    void StartGame(){
        int totalmoves=board->n* board->n;
        int moves=0;
        while (moves<totalmoves)
        {
           Player * curr=players.front();
           players.pop_front();
           board->PrintBoard();

           cout<<curr->name<<"( "<<curr->character->getChar()<<" ) enter a row and coloumn";

           int r,c;
           cin>>r>>c;
           if(!board->addPiece(r,c,curr->character)){
            cout<<"Enter a valid row and column here"<<endl;
            players.push_front(curr);
            continue;
           }
           moves++;

           if(board->CheckWinner(r,c,curr->character)){
            board->PrintBoard();
             cout<<"Winner "<< curr->name<<" of this game"<<endl;
             return ;
           }
           players.push_back(curr);
        }
        board->PrintBoard();
        cout<<"DRAW game !"<<endl;

    }
};

int main(){
     PlayingPieceX px;
    PlayingPieceO po;

    Player p1("Rahul", &px);
    Player p2("Pranav", &po);

    Tic_TAC_TOE game(&p1, &p2, 3);
    game.StartGame();

}
