#include <bits/stdc++.h>
#include <thread>
#include <chrono>
#include <mutex>
#include <condition_variable>

using namespace std;

enum class ElevatorDirection{UP,DOWN,IDLE};
enum class DoorState{OPEN, CLOSED};
class InternalButton{
    ElevatorController * controller;

    public:
    InternalButton(ElevatorController * controller) :controller(controller){};

    void PressButton(int destinationFloor){
        controller->submitRequest(destinationFloor); 
    }
};

class Door{
    DoorState state;
    public:
    Door() {
        state =DoorState :: CLOSED;
    }
    void openDoor(int id){
        state=DoorState:: OPEN;
        cout<<"Opening the door "<<id<<endl;
    }
    void closeDoor(int id){
        state=DoorState:: CLOSED;
        cout<<"Closing the door "<<id<<endl;
    }
};

// --------------->Elevator Car<--------------------------
class ElevatorCar{
    public:
    int id;
    int currentFloor;
    int nextFloorStopage;
    ElevatorDirection moviingDirection;
    Door door;

    ElevatorCar(int id){
        this->id=id;
        currentFloor=0; // currentFloor(0);
        nextFloorStopage=0;
        moviingDirection= ElevatorDirection :: IDLE;
    }
    void showDisplay(){
        cout<<"Elevator "<< id 
        << " |Floor"<< currentFloor<<
        " | Direction"<< (moviingDirection== ElevatorDirection :: UP ?"UP":
        moviingDirection==ElevatorDirection :: DOWN ? "DOWN": "IDLE")<<endl;

    }

    void moveElevator(int destinationFloor){
        nextFloorStopage=destinationFloor;
        if(currentFloor==nextFloorStopage){
            door.openDoor(id);
            return ;
        }
        door.closeDoor(id);

        if(destinationFloor>currentFloor){
            moviingDirection =ElevatorDirection :: UP;
            for(int i=currentFloor;i<=destinationFloor;i++){
                 std::this_thread::sleep_for(std::chrono::milliseconds(5));
                 currentFloor=i;
                 showDisplay();
            }
        }
        else if(destinationFloor<currentFloor){
            moviingDirection = ElevatorDirection :: DOWN;
            for(int i=currentFloor;i>=destinationFloor;i--){
                std::this_thread::sleep_for(std::chrono::milliseconds(5));
                 currentFloor=i;
                 showDisplay();
            }
        }
        door.closeDoor(id);
    }

};


// --------------------------->Elevator Controller<----------------

class ElevatorController{
    public:
    ElevatorCar * elevator;

    priority_queue<int,vector<int>,greater<int>>upPQ;
    priority_queue<int>downPQ;

    std::mutex mx;
    std::condition_variable cv;
    ElevatorController(ElevatorCar * ptr) {
       this->elevator=ptr;
    }
    void submitRequest(int floor){
        unique_lock<mutex> lock(mx);
        if(floor>=elevator->currentFloor){
            upPQ.push(floor);
        }
        else{
            downPQ.push(floor);
        }
        cv.notify_one(); //
    }
    
    void run(){
        while(true){
            unique_lock<mutex> lock(mx);
            
            cv.wait(lock,[&](){
                return !upPQ.empty() || !downPQ.empty();
            });

            while(!upPQ.empty()){
                int floor=upPQ.top();
                upPQ.pop();
                lock.unlock();
                elevator->moveElevator(floor);
                lock.lock();
            }
            while(!downPQ.empty()){
                int floor=downPQ.top();
                downPQ.pop();
                lock.unlock();
                elevator->moveElevator(floor);
                lock.lock();
            }
        }
    }
};

// ----------------------->Strategy<-------------------------------
class ElevationStrategy{
    public:
    virtual ElevatorController * selectElevator(
        vector<ElevatorController *>& controllers,int floor
    )=0;
};
class NearestStrategy :public ElevationStrategy{
    public:
    ElevatorController * selectElevator(vector<ElevatorController*>& controllers,int floor) override{
        ElevatorController * best=nullptr;
        int mindist=INT_MAX;

        for(auto c: controllers){
            int dist=abs(c->elevator->currentFloor-floor);
            if(mindist>dist){
                mindist=dist;
                best=c;
            }
        }
        return best;
    }
};

// --------------------->Scheduler<--------------------------
class ElevatorScheduler{
    vector<ElevatorController*>controllers;
    ElevationStrategy * strategy;

    public:
    ElevatorScheduler(vector<ElevatorController*> ptr1, ElevationStrategy* ptr2):
    controllers(ptr1), strategy(ptr2){};

    ElevatorController * assingElevator(int floor){
        return strategy->selectElevator(controllers,floor);
    }
};
//---------------------->Dispatchers<----------------------------
class ExternalDispatcher{
    ElevatorScheduler * scheduler;
    public:
    ExternalDispatcher(ElevatorScheduler * scheduler) :scheduler(scheduler){};

    void submitRequest(int floor){
        // Elev
    }
};
int main(){
    return 0;
}