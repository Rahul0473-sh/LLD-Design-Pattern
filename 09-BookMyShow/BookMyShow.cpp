#include <bits/stdc++.h>
#include <thread>
using namespace std;

 enum class SeatType{
    REGULAR, RECLINER
};
enum class BookingTypeStatus{
    PENDING,CONFIRMED,CANCELLED
};
class Seat{
    public:

    string id;
    double price;
    Seat(string id, double price): id(id),price(price){};
    virtual SeatType getType()=0;
};
class RegularSeat: public Seat{
    public:
    RegularSeat(string id, double price): Seat(id,price){};
    SeatType getType() override{
        return SeatType:: REGULAR;
    }
};
class ReclinarSeat: public Seat{
    public:
    ReclinarSeat(string id, double price): Seat(id,price){};
    SeatType getType() override{
        return SeatType:: RECLINER;
    }
};
class Screen{
    public:
    string id;
    unordered_map<string,Seat*>seats;

    Screen(string id): id(id){};
    void addSeat(Seat* seat) {
        seats[seat->id] = seat;
    }
};
class Theater{
    public:
    string id;
    string name;
    unordered_map<string,Screen*>screens;
    Theater(string id, string name): id(id),name(name){};
    
    void addScreen(Screen * screen){
        screens[screen->id]=screen;
    }
};
class Movie{
    public:
    string id;
    string title;
    int duration;
    Movie(string id, string title, int duration): id(id), title(title), duration(duration){};
};
class Show{
    public:
    string id;
    Movie * movie;

    time_t startTime;
    Theater * theater;
    Screen * screen;

    Show(string id,Movie * movie,time_t startTime,Theater * theater,Screen * screen):
    id(id), movie(movie),startTime(startTime), theater(theater),screen(screen){};
};
struct LockData{
    string userId;
    long long expiresAt;
};

class LockProvider{
    public:
   virtual bool trylock(string key, string userId, int ttl)=0;
   virtual void unlock(string key, string userId)=0;
   virtual bool isExpired(string key)=0;
   virtual bool isLockedBy(string key,string userId)=0;
};
class MemoryLockProvider: public LockProvider{
    long long now(){
        return chrono:: duration_cast<chrono:: milliseconds>(chrono:: system_clock:: now().time_since_epoch()).count();
    }
    public:
    unordered_map<string,LockData>store;
    bool trylock(string key, string userId, int ttl) override{
        long long t=now();
        if(store.count(key) && t<store[key].expiresAt) return false;

        store[key]={userId,t+ttl*1000};
        return true;
    }
    void unlock(string key,string userId) override{
        if(store.count(key) && store[key].userId==userId) store.erase(key);
    }
    bool isExpired(string key) override{
        
        if(!store.count(key)) return true;

        return now()>store[key].expiresAt;
    }
    bool isLockedBy(string key, string userId){
        if(!store.count(key)) return false;
        return store[key].userId==userId;
    }
};
class Booking;
class PaymentStrategy{
    public:
    virtual bool pay(Booking& booking)=0;
};
class UpiPaymentStartegy: public PaymentStrategy{
    public:
    virtual bool pay(Booking& booking){
        cout<<"Paid By UPI"<<endl;
        return true;
    }
};
class CardPaymentStartegy: public PaymentStrategy{
    public:
    virtual bool pay(Booking& booking){
        cout<<"Paid By Card"<<endl;
        return true;
    }
};
enum class PaymentType{
    UPI,CARD
};
class  PaymentStrategyFactory{
    public:
    static PaymentStrategy*  getStrategy(PaymentType TYPE){
        if(TYPE==PaymentType::UPI){
            return new UpiPaymentStartegy();
        }
        if(TYPE==PaymentType::CARD){
            return new CardPaymentStartegy();
        }

    }
};



class Booking{
    public:
    long long id;
    string userId;
    string showid;
    vector<string>seats;
    BookingTypeStatus status;
};
class BookingRepo{
    unordered_map<long long ,Booking>db; // it's a db rigth now
    public:
    void save(Booking& b){
        db[b.id]=b;
    };
    Booking getBooking(long long id){
        return db[id];
    };
};


// Service


class MovieService{
    public:
    unordered_map<string,Movie*>movies;
    void addMovie(Movie* movie){
        movies[movie->id]=movie;
    };
};
class ShowService{
    public:
    vector<Show*>shows;
    void addShow(Show* show){
        shows.push_back(show);
    }
    vector<Show*>searchByMovieName(string name){
        vector<Show*>res;
        for(auto s:shows){
            if(s->movie->title==name){
                res.push_back(s);
            }
        }
        return res;
    }

};
class TheaterService{
    public:
    unordered_map<string, Theater*>theaters;
    void addTheater(Theater * theater){
        theaters[theater->id]=theater;
    }
};
class ScreenService{
    public:
    unordered_map<string,Screen*>screens;
    void addScreens(Screen* screen){
        screens[screen->id]=screen;
    }
};

class BookingService{
    BookingRepo * repo;
    LockProvider* provider;
    int TTL=5;

    public:

    BookingService(LockProvider * provider, BookingRepo * repo): provider(provider), repo(repo){};
    Booking CreateBooking(string userId, Show* show, vector<string>seats){
        vector<string>keys;
        for(auto seat: seats){
            string key="lock"+ show->id+":"+ seat;
            keys.push_back(key);

            if(!provider->trylock(key,userId,TTL)){
                for(auto it:keys){
                    provider->unlock(it,userId);
                }
                throw runtime_error("Seat already locked");
            }
        }
        Booking b;
        b.id=rand();
        b.userId=userId;
        b.seats=seats;
        b.showid=show->id;
        b.status=BookingTypeStatus:: PENDING;

        return b;
    }
    Booking confirmBooking(Booking & b,PaymentType Type){
        for(auto& seat: b.seats){
           string key="lock"+ b.showid+":"+ seat;

           if(provider->isExpired(key)  || !provider->isLockedBy(key,b.userId)){
            __throw_runtime_error("Lock Expired, Payment Rejected");
           }
        } 
        PaymentStrategy * startegy=PaymentStrategyFactory::getStrategy(Type);
        startegy->pay(b);
        b.status= BookingTypeStatus:: CONFIRMED;

        repo->save(b);

        for(auto& seat: b.seats){
            provider->unlock("lock"+ b.showid+":"+ seat,b.userId);
        }
        return b;
        
    }
};



int main(){
    cout<<"\n ================SETUP=================";
    TheaterService theaterService;
    MovieService movieService;
    ShowService showService;

    MemoryLockProvider LockProvider;
    BookingRepo bookingrepo;
    BookingService bookingServive(&LockProvider,&bookingrepo);

    // add theater
    Theater * theater=new Theater("T1","PVR");
    theaterService.addTheater(theater);

    cout<<"Theater Added";
    
    //2 Add Scrren
    Screen* screen = new Screen("S1");
    theater->addScreen(screen);

     cout << "✅ Screen Added\n";


    screen->addSeat(new RegularSeat("A1", 100));
    screen->addSeat(new RegularSeat("A2", 100));
    screen->addSeat(new ReclinarSeat("A3", 300));
    cout << "✅ Seats Added\n";

    Movie* movie=new Movie("M1","Inception",120);
    movieService.addMovie(movie);
    cout << "✅ Movie Added\n";

    // 5 Create Show
    Show * show=new Show("SH1",movie, time(0), theater,screen);
    showService.addShow(show);
    cout << "✅ Show Created\n";

    auto Showdata=showService.searchByMovieName("Inception");
    cout << "🎬 Shows Found: " << Showdata.size() << "\n";

    cout << "\n===== TEST 1: SUCCESS =====\n";

    auto booking1=bookingServive.CreateBooking("user1",show,{"A1"});
    bookingServive.confirmBooking(booking1,PaymentType::CARD);

     // 8. Double Booking

     cout << "\n===== TEST 2: DOUBLE BOOKING =====\n";
     try{
        bookingServive.CreateBooking("user2",show,{"A1"});
     }catch(exception& e){
        cout<<e.what()<<endl;
     }

     cout << "\n===== TEST 3: LOCK EXPIRY =====\n";
     try
     {
        auto booking2=bookingServive.CreateBooking("user3", show, {"A3"});
        cout << "⏳ Waiting...\n";
        this_thread::sleep_for(chrono::seconds(6));

        bookingServive.confirmBooking(booking2,PaymentType::UPI);
     }
     catch(const std::exception& e)
     {
        cout << e.what() << '\n';
     }
     
    



}