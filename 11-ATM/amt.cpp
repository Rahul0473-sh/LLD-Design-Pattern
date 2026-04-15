#include <bits/stdc++.h>
using namespace std;

enum class operationTypes{
    WITHDRAW_CASH,DEPOSIT_CASH,CASH_BALANCE,CHECK_BALANCE
};
class Card{
    string cardNumber;
    string pin;
    public:

    Card(string cardNumber,string pin): cardNumber(cardNumber),pin(pin){};
    string getCardNumber() const {return cardNumber;}
    string getPin()const {return pin;}

};
// Account
class Account{
    string accountNumber;
    double balance;

    unordered_map<string,shared_ptr<Card>>cards; // cardNumber->card;
    public:
    Account(string accountNumber, double initial) : accountNumber(accountNumber), balance(initial){};
    
    bool withDraw(double amount){
        if(amount<=0 || amount>balance) return false;
        balance-=amount;
        return true;
    }
    void deposit(double amount){
        if(amount>0) balance+=amount;
    }
    double getBalance()const {return balance;}
    string getAccountNumber() const {return accountNumber;}
    void addCard(shared_ptr<Card>card){
        cards[card->getCardNumber()]=card;
    } 


};

class BankService{
    unordered_map<string,shared_ptr<Card>>cards;
    unordered_map<string,shared_ptr<Account>>accounts;
    unordered_map<string,string>cardToAccount;

    public:
    shared_ptr<Account>createAccount(string accountNumber,double bal){
        auto account=make_shared<Account>(accountNumber,bal);
        accounts[accountNumber]=account;
        return account;
    }
    shared_ptr<Card>createCard(string cardNumber,string pin){
        auto card=make_shared<Card>(cardNumber,pin);
        cards[cardNumber]=card;
        return card;
    }
    void linkCard(shared_ptr<Card>card,shared_ptr<Account>acc){
        cardToAccount[card->getCardNumber()]=acc->getAccountNumber();
        acc->addCard(card);
    }
    
    shared_ptr<Card>getCard(string cardNo){
        if(cards.count(cardNo)) return cards[cardNo];
        return nullptr; 
    }
    bool Authenticate(shared_ptr<Card>card,string pin){
        return card && card->getPin()==pin;
    }
    shared_ptr<Account>getAccount(shared_ptr<Card> card){
        if(!card) return nullptr;
        string accNumber=cardToAccount[card->getCardNumber()];
        return accounts[accNumber];
    }
    double getBalance(shared_ptr<Card>card){
       auto account=getAccount(card);
      return  account?account->getBalance():-1;
    }
    bool withDraw(shared_ptr<Card>card, double amount){
        auto acc=getAccount(card);
        return acc and acc->withDraw(amount);
    }
    
    void deposit(shared_ptr<Card>card, double amount){
        auto acc=getAccount(card);
        if(acc) acc->deposit(amount);
    }
};

// Dispence chain
class DispenseChain{ // abstract class
    public:
    virtual void dispense(int amount)=0;
    virtual void setNext(shared_ptr<DispenseChain>next)=0;
};
class NoteDispenser: public DispenseChain{
    int noteValue;
    int numNotes;
    shared_ptr<DispenseChain> nxt;

    public:
    NoteDispenser(int value, int count): noteValue(value),numNotes(count){};
    void setNext(shared_ptr<DispenseChain>next){
        nxt=next;
    }
    void dispense(int amount) override{
        if(amount>=noteValue ){
            int note=min(amount/noteValue,numNotes);
            numNotes-=note;
            cout<<"Dispensing "<<note<<"x"<<noteValue<<endl;
            
            int remainder=amount-(note* noteValue);
            if(remainder>0 && nxt){
                nxt->dispense(remainder);
            }

        }
        else {
            nxt->dispense(amount);
        }
    }
};

class Note100: public NoteDispenser{
    public:
    Note100(int n): NoteDispenser(100,n){};
};
class Note50: public NoteDispenser{
    public:
    Note50(int n): NoteDispenser(50,n){};
};
class Note10: public NoteDispenser{
    public:
    Note10(int n): NoteDispenser(10,n){};
};

class CashDispenser{
    shared_ptr<DispenseChain>chain;

    public:
    CashDispenser(){
        auto d100=make_shared<Note100>(100);
        auto d50=make_shared<Note50>(20);
        auto d10=make_shared<Note10>(500);

        d100->setNext(d50);
        d50->setNext(d10);
        chain=d100;

    }
    void dispense(int amount){
        cout<<"[ATM] Dispnesing"<<amount<<endl;
        chain->dispense(amount);
    }
};


//ATM STATEPATTERN
class ATMSystem;
class ATMState{
    public:
    virtual void insertCard(ATMSystem*, string)=0;
    virtual void enterPin(ATMSystem*,string)=0;
    virtual void operation(ATMSystem*,operationTypes, int)=0;
    virtual void ejectCard(ATMSystem*)=0;
};
class IdleState;
class HasCardState;
class AuthState;

// ATMSYSTEM SingleTon pattern;

class ATMSystem{ 
    static ATMSystem* instance;
    ATMState* state;

    shared_ptr<Card>currentCard;

    BankService* bank;
    CashDispenser* dispenser;

    ATMSystem(BankService * b, CashDispenser* d):
    bank(b),dispenser(d){};

    public:
    static ATMSystem* getInstance(BankService*b, CashDispenser* dispenser){
        if(instance==nullptr){
            instance=new ATMSystem(b,dispenser);
        }
        return instance;
    }

    void setState(ATMState*s){state=s;}
    ATMState* getState(){return state;}
    
    void setCard(shared_ptr<Card>c){currentCard=c;}
    shared_ptr<Card>getCard(){return currentCard;}

    CashDispenser* getDisp(){return dispenser;}
    bool authenticate(string pin){
        return bank->Authenticate(currentCard,pin);
    }

    void withDraw(int amt){
        if(bank->withDraw(currentCard,amt)){
            dispenser->dispense(amt);
            cout<<"WithDraw Successfull"<<endl;
        }
    }
    void Deposit(int amt){
        bank->deposit(currentCard,amt);
        cout<<"Deposit Successfull";
    }
    void balance(){
       cout<<"Current Balance"<< bank->getBalance(currentCard)<<endl;

    }

    BankService* getBank(){return bank;}

};
ATMSystem* ATMSystem::instance=nullptr;

class IdleState: public ATMState{
    public:
    void insertCard(ATMSystem* atm,string cardNo )override;
    void enterPin(ATMSystem*, string) override {
        cout << "Insert card first\n";
    }

    void operation(ATMSystem*, operationTypes, int) override {
        cout << "Insert card first\n";
    }

    void ejectCard(ATMSystem*) override {
        cout << "No card\n";
    }
};
class HasCardState:public ATMState{
    void insertCard(ATMSystem* amt, string CardNo)override{
        cout<<"card already Inserted"<<endl;
        return;
    }
    void enterPin(ATMSystem *atm,string pin) override;
      void operation(ATMSystem*, operationTypes op, int) override {
        cout << "PIN not verified. Enter the correct PIN before performing operations.\n";
    }

    void ejectCard(ATMSystem* atm) override {
        atm->setCard(nullptr);
        atm->setState(new IdleState());
        cout << "Card ejected\n";
    }

};

class AuthState : public ATMState {
public:
    void insertCard(ATMSystem*, string) override {
        cout << "Already authenticated\n";
    }

    void enterPin(ATMSystem*, string) override {
        cout << "Already authenticated\n";
    }

    void operation(ATMSystem* atm, operationTypes op, int amt) override {
        if (op == operationTypes::CHECK_BALANCE)
            atm->balance();
        else if (op == operationTypes::WITHDRAW_CASH)
            atm->withDraw(amt);
        else if (op == operationTypes::DEPOSIT_CASH)
            atm->Deposit(amt);
    }

    void ejectCard(ATMSystem* atm) override {
        atm->setCard(nullptr);
        atm->setState(new IdleState());
        cout << "Session ended\n";
    }
};

void IdleState::insertCard(ATMSystem* atm, string cardNo) {
    auto card = atm->getBank()->getCard(cardNo);
    if (!card) {
        cout << "Invalid card.\n";
        return;
    }
    atm->setCard(card);
    atm->setState(new HasCardState());
    cout << "CardInserted" << endl;
}

void HasCardState::enterPin(ATMSystem* atm, string pin) {
    if (atm->authenticate(pin)) {
        atm->setState(new AuthState());
        cout << "Authenticated" << endl;
    } else {
        cout << "Invalid PIN. Try again.\n";
    }
}

int main() {
    BankService bank;
    CashDispenser dispenser;

    auto atm = ATMSystem::getInstance(&bank, &dispenser);
    atm->setState(new IdleState());

    auto acc = bank.createAccount("A1", 5000);
    auto card = bank.createCard("1111", "1234");
    bank.linkCard(card, acc);

    atm->getState()->insertCard(atm, "1111");
    atm->getState()->enterPin(atm, "1234");
    atm->getState()->operation(atm, operationTypes::CHECK_BALANCE, 0);
    atm->getState()->operation(atm, operationTypes::WITHDRAW_CASH, 500);
    atm->getState()->ejectCard(atm);

    return 0;
}

