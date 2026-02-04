#include <bits/stdc++.h>
using namespace std;

enum class SeatCategory{
    Luxury,
    Premium,
    General
};
enum class SeatStatus{
    AVAILABLE,LOCKED,BOOKED
};
class Movie{
    public:
    string name;
    Movie(string name){
        this->name=name;
    }
    string getMovieName(){
        return name;
    }
};
class MovieController{
    unordered_map<string,vector<Movie>>cityMovies;
    vector<Movie>Movies;
};
class Seat{
    public:
    int id;
    SeatCategory category;

    public:
    Seat(int id, SeatCategory SeatCat){
        this->id=id;
        this->category=SeatCat;
    }
};
class Show{
    Movie * movie;
    string showDate;
    string startTime;

    unordered_map<int,SeatStatus>SeatStatusMap;
    unordered_map<int,int>seatLocks;
    public:
    Movie * getMovie(){ return movie;}
    string getLocalDate(){ return showDate; }
    string getStartTime(){return startTime;}


};
class Screen{
    int screenId;
    unordered_map<string,vector<Seat>>showsByDate;
    vector<Seat>seats;
    public:

    Screen(int screenId,vector<Seat>seats){
        this->seats=seats;
        this->screenId=screenId;
    }
    vector<Seat>getSeats(){
        return seats;
    }
};

int main(){
    return 0;
}