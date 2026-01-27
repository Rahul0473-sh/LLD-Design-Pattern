#include <bits/stdc++.h>
using namespace std;

class Vehicle{
    public:
    virtual  int vehicleCapacity()=0;
    virtual  int efficency()=0;
};


class Car: public Vehicle{
    public: 
    int vehicleCapacity() override{
        return 10;
    }
    int efficency() override{
        return 12;
    }
};
class Null: public Vehicle{
    public: 
    int vehicleCapacity() override{
        return 0;
    }
    int efficency() override{
        return 0;
    }
};
class VehilceFactory{
    public:
    Vehicle* getVechicle(string type){
        if(type=="CAR") return new Car();
        return new Null();
    }
};
void PrintVehicle(Vehicle * v){
    // if(v!=null )

    cout<<v->vehicleCapacity()<<" "<< v->efficency()<<" "<<endl;
}
int main(){
    VehilceFactory veh;
    Vehicle* vehicle=veh.getVechicle("BIKE");
    PrintVehicle(vehicle);
}