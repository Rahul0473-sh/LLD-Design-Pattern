#include <bits/stdc++.h>
using namespace std;

enum VehicleType {TWO_WHEELER=1, FOUR_WHEELER=2};
class Vehicle{
    public:
    string vehicleId;
    Vehicle(string id, VehicleType t){ vehicleId=id; vehType=t; }
    VehicleType vehType;

    string getVehicleId(){
        return vehicleId;
    }
    VehicleType getVehicleType(){
        return vehType;
    }
};
class Ticket{
    public:
    string ticketId;
    string buildingId;
    string levelId;
    VehicleType vechicleType;
    string spotId;
    string vehicleId;
    bool active;

    time_t startMinute;
    int endMinute;

    Ticket(string ticketId,string buildingId, string levelId, VehicleType vechicleType, string spotId, string vehicleId,bool active) {
        this->ticketId = ticketId;
    this->buildingId = buildingId;
    this->levelId = levelId;
    this->vechicleType = vechicleType;
    this->spotId = spotId;
    this->vehicleId = vehicleId;
    this->active = active;
    this->startMinute = time(nullptr);
    };
};

class ParkingSpot{
    string spotId;
    VehicleType allowedtype;
    bool occupied;
    string vehilcePlate;

    public:
    ParkingSpot (string spotId, VehicleType t){
        this->spotId=spotId;
        allowedtype=t;
        occupied=false;
        vehilcePlate="";
    }

    bool isFree(){
        return occupied==false;
    }
    string getId(){
        return this->spotId;
    }
    VehicleType getAllowedType(){
        return allowedtype;
    }
    bool canfit(Vehicle *v){
        if(!occupied && v->getVehicleType()==allowedtype) return true;
        return false;
    }
    void park(Vehicle* vehicle){
        occupied=true;
        vehilcePlate=vehicle->vehicleId;
    }
    void unPark(){
        occupied=false;
        vehilcePlate="";
    }
};
// ---------------->Parkign Strategy<-------------------
class ParkingStrategy{
    public:
    virtual ParkingSpot * findSpot(vector<ParkingSpot*>)=0;
    virtual ~ParkingStrategy(){};
};
// let's say just one parking strategy for example
class SimpleStrategy: public ParkingStrategy{
    public:
    ParkingSpot * findSpot(vector<ParkingSpot*>list){
        for(auto it:list){
            if(it->isFree()) {
                return it;
            }

        }
        return nullptr;
    }
};
// -------------->ParkingSpot Manager<--------------------
class ParkingSpotManger{
    public:
    ParkingStrategy * strategy;
    vector<ParkingSpot* >parkingspots;

    virtual VehicleType managestype()=0;

    ParkingSpotManger(vector<ParkingSpot*>spotsptrs,ParkingStrategy *strat){
        strategy=strat;
        parkingspots=spotsptrs;
    }
    ParkingSpot * park(Vehicle *vechicle){
        if(vechicle->getVehicleType()!=managestype()) return nullptr;
        if(strategy==nullptr) return nullptr;

        ParkingSpot * spot=strategy->findSpot(parkingspots);
        if(spot==nullptr) return nullptr;
        spot->park(vechicle);

        return spot;
    }
    bool unPark(string spotId){
        for(auto it:parkingspots){
            if(it->getId()==spotId){
                it->unPark();
                return true;
            }
        }
        return false;
    }

};

class TwoWheelerParkingSpotManager:public ParkingSpotManger{
    public:
    TwoWheelerParkingSpotManager(vector<ParkingSpot*>spotsptrs,ParkingStrategy *strat):ParkingSpotManger(spotsptrs,strat){}

    VehicleType managestype() override{
        return {TWO_WHEELER};
    }
};
class FourWheelerParkingSpotManager: public ParkingSpotManger{
    public:
    FourWheelerParkingSpotManager (vector<ParkingSpot*>spotsptrs,ParkingStrategy *strat):ParkingSpotManger(spotsptrs,strat){}
    VehicleType managestype() override{
        return {FOUR_WHEELER};
    }
};

class PriceStrategy{
    public:
    virtual int calculatePrice(Ticket *t)=0;
};

class  FlatStrategy :public PriceStrategy{
    public:
    int basePay=0;
    int calculatePrice(Ticket *t){
        if(t->vechicleType==TWO_WHEELER) basePay=20;
        else if(t->vechicleType==FOUR_WHEELER) basePay=40;
        else{
            basePay=50;
        }
        int endTime=time(nullptr);
        double totalTime=difftime(endTime,t->startMinute)/3600;
        if(totalTime<=0) totalTime=1;
        cout<<"TotalTime"<<totalTime<<endl;

        return totalTime* basePay;
    }
};


class ParkingLevel{
    public:
    string level;
    vector<ParkingSpotManger* >managers;

    ParkingLevel(string id,vector<ParkingSpotManger* > ptrs){
        managers=ptrs;
        level=id;
    }
    string getLevel(){
        return level;
    }

    ParkingSpot * tryPark(Vehicle* vehicle){
        for(auto managers:managers){
            if(managers->managestype()==vehicle->vehType){
                ParkingSpot* sp= managers->park(vehicle);
                if(sp) return sp;
            }
        }
        return nullptr;
    }
    bool unpark(VehicleType v, string spotId){
        for(auto manager: managers){
            if(manager->managestype()==v)  return manager->unPark(spotId);
        }
        return false;
    }
};
class ParkingBuilding{

    public:
    string builingNumber;
    vector<ParkingLevel *>parkingLevels;
    unordered_map<string,Ticket *>activeTickets;
    PriceStrategy * pricingStrategy;

    long long ticektSeq;

    ParkingBuilding(string builingNumber, vector<ParkingLevel*>ptrs,PriceStrategy * strat) {
        this->builingNumber=builingNumber;
        parkingLevels=ptrs;
        ticektSeq=0;
        pricingStrategy=strat;
    }

    string nextTicketId(){
        ticektSeq++;
        return builingNumber+"-T"+ to_string(ticektSeq);
    }
    Ticket * Park(Vehicle *v){
        for(auto it:parkingLevels){
            ParkingSpot * spot=it->tryPark(v);
            if(spot){
                Ticket *t =new Ticket(nextTicketId(),this->builingNumber, it->getLevel(),v->getVehicleType(), spot->getId(),v->getVehicleId(),true);
                activeTickets[t->ticketId]=t;
                return t;
            }
        }
        return nullptr;
    }
    double releaseTicket(string ticketId){
        if(activeTickets.find(ticketId)==activeTickets.end()) throw "Invalid Ticket";

        Ticket * t=activeTickets[ticketId];
        if(t->active==false) throw "Ticket already closed";

        bool unParked=false;
        for(auto it: parkingLevels){
            if(it->getLevel()==t->levelId){
                unParked=it->unpark(t->vechicleType, t->spotId);
                break;
            }
        }
        if(!unParked) throw "Unable to unpark";
        if(pricingStrategy==NULL) throw "can park without strategy";

        int amount =pricingStrategy->calculatePrice(t);

        activeTickets.erase(t->ticketId);

        return amount;
    }


};
class EntranceGate{
    ParkingBuilding * building;

    public:
    EntranceGate(ParkingBuilding * ptr){
        building=ptr;
    }
    Ticket * enter(Vehicle *v){
        return building->Park(v);
    }
};

class ParkingLot{
    ParkingBuilding * building;
    EntranceGate * gate;

    public:

    ParkingLot(ParkingBuilding * ptr1,EntranceGate * ptr2 ){
        building=ptr1;
        gate=ptr2;
    }

    Ticket * EnterVehicle(Vehicle *v){
        return gate->enter(v);
    }
    double exitVehicle(string TicketId){
        return building->releaseTicket(TicketId);
    }


};

int main(){
     vector<ParkingSpot*> twoWSpots;
    twoWSpots.push_back(new ParkingSpot("L1-2W-01", TWO_WHEELER));
    twoWSpots.push_back(new ParkingSpot("L1-2W-02", TWO_WHEELER));

    vector<ParkingSpot*> fourWSpots;
    fourWSpots.push_back(new ParkingSpot("L1-4W-01", FOUR_WHEELER));
    fourWSpots.push_back(new ParkingSpot("L1-4W-02", FOUR_WHEELER));

    // --------- Parking Strategy ----------
    ParkingStrategy* simpleStrat2W = new SimpleStrategy();
    ParkingStrategy* simpleStrat4W = new SimpleStrategy();

    // --------- Managers ----------
    ParkingSpotManger* twoMgr = new TwoWheelerParkingSpotManager(twoWSpots, simpleStrat2W);
    ParkingSpotManger* fourMgr = new FourWheelerParkingSpotManager(fourWSpots, simpleStrat4W);

    vector<ParkingSpotManger*> managers;
    managers.push_back(twoMgr);
    managers.push_back(fourMgr);

    // --------- Level ----------
    ParkingLevel* level1 = new ParkingLevel("L1", managers);
    vector<ParkingLevel*> levels;
    levels.push_back(level1);

    // --------- Pricing Strategy ----------
    PriceStrategy* pricing = new FlatStrategy();

    // --------- Building ----------
    ParkingBuilding* building = new ParkingBuilding("B1", levels, pricing);

    // --------- Gate ----------
    EntranceGate* gate = new EntranceGate(building);

    // --------- ParkingLot ----------
    ParkingLot* lot = new ParkingLot(building, gate);

    // --------- Vehicles ----------
    Vehicle* bike = new Vehicle("BIKE-101", TWO_WHEELER);
    Vehicle* car  = new Vehicle("CAR-999", FOUR_WHEELER);

    // --------- Entry ----------
    Ticket* t1 = lot->EnterVehicle(bike);
    if (t1) {
        cout << "Bike Ticket: " << t1->ticketId
             << " Spot: " << t1->spotId
             << " Level: " << t1->levelId << "\n";
    } else {
        cout << "No spot for Bike\n";
    }

    Ticket* t2 = lot->EnterVehicle(car);
    if (t2) {
        cout << "Car Ticket: " << t2->ticketId
             << " Spot: " << t2->spotId
             << " Level: " << t2->levelId << "\n";
    } else {
        cout << "No spot for Car\n";
    }

    int  t2amount=lot->exitVehicle(t2->ticketId);
    int t1amount =lot->exitVehicle(t1->ticketId);

    cout<<"car AMount"<<t2amount<<endl;
    cout<<"Bike AMount"<<t1amount<<endl;

}