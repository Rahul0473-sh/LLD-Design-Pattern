#include <bits/stdc++.h>
using namespace std;
enum Loglevel{
    info=1,
    error=2,
    debug=3
};

class Logger{
    public:
    Loglevel level;
    Logger * nextLogger;

    Logger(Loglevel level){
        this->level=level;
        nextLogger=nullptr;
    }

    void setNext(Logger * next){
        this->nextLogger=next;
    }
    void log(Loglevel messageLevel, string  message){
        if(messageLevel==level){
            write(message);
        }
        if(nextLogger!=nullptr){
            nextLogger->log(messageLevel,message);
        }
    }
    virtual void write(string message)=0;

};

class Infologger : public Logger{
    public:
    Infologger():Logger(info){}

    void write(string message) override{
        cout<<"[INFO] "<< message<<endl;
    }
};
class Errorlogger : public Logger{
    public:
    Errorlogger():Logger(error){}

    void write(string message) override{
        cout<<"[ERROR] "<< message<<endl;
    }
};

class Debuglogger : public Logger{
    public:
    Debuglogger():Logger(debug){}

    void write(string message) override{
        cout<<"[debug] "<< message<<endl;
    }
};

int main(){
    Logger * infoLogger= new Infologger();
    Logger * debuglogger= new Debuglogger();
    Logger * errorlogger=new Errorlogger();

    infoLogger->setNext(debuglogger);
    debuglogger->setNext(errorlogger);

    infoLogger->log(info,"APPLICATON STARTED");
    cout<<"---------------------------------------"<< endl;
    infoLogger->log(error, "databaseconnnection failed");

    infoLogger->log(debug, "varaibel x=42");



}