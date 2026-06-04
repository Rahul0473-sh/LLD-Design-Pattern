#include <bits/stdc++.h>
using namespace std;

class Handler{
    public:
    Handler * nxt=nullptr;
    virtual void setNexthandler(Handler* nxt)=0;
    virtual void CallNext()=0;
    virtual void  HandlerRequest()=0;

    virtual ~Handler(){};
};
class Interview: public Handler{
    public:
    void setNexthandler(Handler * nxt) override{
        this->nxt=nxt;
    }
    void CallNext() override{
        if (nxt)
            nxt->HandlerRequest();
        else
            cout << "\nProcess completed successfully!\n";
    }
    void HandlerRequest() override{
        int value=(rand()%100)+1;
        if(value>50){
            cout<<"Candidate Passed Interview Phase"<<endl;
            this->CallNext();
        }
        else{
            cout<<value;
            cout<<"Interview failed"<<endl;
            return ;
        }
    }
};
class BarRaise: public Handler{
    public:
    void setNexthandler(Handler * nxt) override{
        this->nxt=nxt;
    }
    void CallNext() override{
       if (nxt)
            nxt->HandlerRequest();
        else
            cout << "\nProcess completed successfully!\n";
    }
    void HandlerRequest() override{
        int value=(rand()%100)+1;
        if(value>50){
            cout<<"Candidate Passed BarRaise Phase"<<endl;
            this->CallNext();
        }
        else{
            cout<<"Bar Raise failed"<<endl;
            return ;
        }
    }
};
class HR: public Handler{
    public:
    void setNexthandler(Handler * nxt) override{
        this->nxt=nxt;
    }
    void CallNext() override{
       if (nxt)
            nxt->HandlerRequest();
        else
            cout << "\nProcess completed successfully!\n";
    }
    void HandlerRequest() override{
        int value=(rand()%100)+1;
        if(value>50){
            cout<<"Candidate Passed HR Phase"<<endl;
            this->CallNext();
        }
    }
};
int main(){
    
    srand(time(nullptr));
    Handler* inter= new Interview();
    Handler * bar =new BarRaise();
    Handler * hr=new HR();
    inter->setNexthandler(bar);
    bar->setNexthandler(hr);

    inter->HandlerRequest();


    delete inter;
    delete bar;
    delete hr;
    
}