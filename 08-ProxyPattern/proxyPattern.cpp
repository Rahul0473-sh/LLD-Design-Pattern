#include <bits/stdc++.h>
using namespace std;

class EmployeTable{
    public:
    virtual void create(string role)=0;
    virtual void deletefn(string role)=0;
    virtual ~EmployeTable(){
        cout<<"BASE PARENT DESTRUCTOR CALLED"<<endl;
    } //Parent destructor virtual isliye hota hai taaki base pointer se delete karne par child ka destructor bhi call ho.
};
class ConcreteEmployeTable : public EmployeTable{
    public:
    void create(string role) override{
        cout<<"Table Created";
    }
    void deletefn(string role ) override{
        cout<<"Table deleted";
    }
    ~ConcreteEmployeTable(){
        cout<<"concrete called"<<endl;
    }
};
class ProxyConcreteEmployeTable: public EmployeTable{
    public:
    EmployeTable * table;
    ProxyConcreteEmployeTable(EmployeTable * ptr){
        this->table=ptr;
    }

    void create(string role){
        if(role=="admin"){
            table->create(role);
            cout<<"Success"<<endl;
        }else{
            cout<<"fail"<< endl;
        }
    }
    void deletefn(string role){
        if(role=="admin"){
            table->deletefn(role);
            cout<<"Success";
        }
        else{
            cout<<"fail"<< endl;
        }
    }
    ~ProxyConcreteEmployeTable() {
        cout<<"Proxy called"<<endl;
        delete table;
    }


};

int main(){
   EmployeTable * proxy= new ProxyConcreteEmployeTable(new ConcreteEmployeTable());
   proxy->create("user");

   delete proxy;
}