#include <bits/stdc++.h>
using namespace std;

class Coffee{ // this is jus an interface
    public:
    virtual string getDescription()=0;
    virtual int getCost()=0;

    virtual ~Coffee(){}
};

class Capacuino :public Coffee{
    public:
    string getDescription(){
        return "Cappucino Coffee ";
    }
    int getCost(){
        return 100;
    }
};
class Latte :public Coffee{
    public:
    string getDescription(){
        return "Cappucino Coffee ";
    }
    int getCost(){
        return 120;
    }
};

// ----------------->Addons<----------------------------
class Addons :  public Coffee{
    public:
    Coffee * coff;
    Addons(Coffee * ptr){
        coff=ptr;
    }
};
class ExtraCreme :public Addons{
    public:
    ExtraCreme(Coffee * ptr): Addons(ptr){}

    string  getDescription() override{
        return coff->getDescription() +" with ExtraCreme";
    }
    int  getCost() override{
        return coff->getCost()+ 50;
    }

};
class ExtraMilk :public Addons{
    public:
    ExtraMilk(Coffee * ptr): Addons(ptr){}

    string getDescription() override{
        return coff->getDescription() +"With ExtraMilk ";
    }
    int  getCost() override{
        return coff->getCost()+ 30;
    }

};

int main(){
    Coffee * coffee1=new ExtraMilk (new ExtraCreme (new Capacuino()));
    cout<< coffee1->getDescription()<<" with cost of "<< coffee1->getCost(); 


    return 0;
}