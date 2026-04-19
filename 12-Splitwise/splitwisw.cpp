#include <bits/stdc++.h>
using namespace std;
class User{
    public:
    string id;
    string name;
    User(string id,string name):id(id),name(name){};
};
class Split{
    public:
    User* user;
    double amount;
    Split(User* user, double amount): user(user),amount(amount){};
};
enum class SplitType{
    EQUAL,PERCENTAGE
};

class Expense{
    public:
    string description;
    double amount;
    User* paidBy;
    vector<Split>splits;
    SplitType type;
    Expense(string description,double amount,User* paidBy, vector<Split>splits, SplitType type):
    description(description),amount(amount),paidBy(paidBy), splits(splits),type(type){};
};

// strategy:
class SplitStrategy{
    public:
    virtual vector<Split> split(double amount,vector<User*>participants,const vector<double>& metadata)=0;
    virtual ~SplitStrategy(){};
};
class EqualSplitStrategy: public SplitStrategy{
    vector<Split>split(double amount,vector<User*>participants,const vector<double>& metadata) override{
        double share=amount/participants.size();
        vector<Split>result;
        for(auto it:participants){
            // result.push_back(Split(it,amount));
            result.emplace_back(it,share);
        }
        return result;
    }
};
class PercentageSplitStrategy: public SplitStrategy{
    vector<Split>split(double amount,vector<User*>participants,const vector<double>& percentages) override{
        vector<Split>result;
        
        for(auto i=0;i<participants.size();i++){
            // result.push_back(Split(it,amount));
           double share=(percentages[i]/100.0)* amount;
           result.emplace_back(participants[i],share);
        }
        return result;
    }
};

// factory Method

class SplitStrategyFactory{
    public:
    static SplitStrategy* getStrategy(SplitType type){
        if(type==SplitType::EQUAL) return new EqualSplitStrategy();
        else if(type==SplitType:: PERCENTAGE) return new PercentageSplitStrategy();
        else return nullptr;
    }
};


// ---------------------->Group<----------------------------------

class Group{
    public:
    string id;
    string name;
    vector<User*>members;

    // balance
    // balances[A][B]-> A owes B
    unordered_map<string,unordered_map<string,double>>balances;
    Group(string id, string name): id(id), name(name){};
    void addMember(User* user){
        members.push_back(user);
    }

};

// Service
class BalanceSheetService{
    public:
    static void updateBalance(Group * group, Expense * expense){
        User* paidBy=expense->paidBy;
        
        for(auto split:expense->splits){
            if(split.user==paidBy) continue;
            auto u=split.user->id;
            auto v=paidBy->id;

            double amount=split.amount;
            if(group->balances[v][u]>0){
                double minVal=min(group->balances[v][u],amount);
                group->balances[v][u]-=minVal;
                amount-=minVal;
            }
            if(amount>0){
                group->balances[split.user->id][paidBy->id]+=split.amount;
            }

        }
    }
    static void printBalances(Group * group){
        cout<<"\n----------Balances-----------------\n";
        for(auto u1:group->balances){
            for(auto u2:u1.second){
                if(u2.second>0){
                    cout << u1.first << " owes " << u2.first << " : " << u2.second << endl;
                    
                }
            }
        }
    }

};

class ExpenseServcie{
    public:
    static Expense* addExpense(Group * group, string des, double amount, User* paidBy, 
    vector<User*>& participants, SplitType type,const vector<double>& metadata
    ){
        SplitStrategy * strat= SplitStrategyFactory:: getStrategy(type);
        vector<Split>splits=strat->split(amount,participants,metadata);

        // Expense(string description,double amount,User* paidBy, vector<Split>splits, SplitType type):
        Expense* expense=new Expense(des,amount, paidBy,splits,type);

        BalanceSheetService:: updateBalance(group, expense);
        BalanceSheetService:: printBalances(group);

        delete strat;
        return expense;
    }
};
class DebtSimplificationService{
    public:
    static void simplify(Group* group){
        unordered_map<string,double>net;
        for(auto u1:group->balances){
            for(auto u2:u1.second){
                net[u1.first]-=u2.second;
                net[u2.first]+=u2.second;
            }
        }
        priority_queue<pair<double,string>>credit;
        priority_queue<pair<double,string>>debt;

        for(auto& it:net){
            if(it.second>0) credit.push({it.second,it.first});
            else{
                debt.push({-it.second,it.first});
            }
        }
        cout<<"\n----------------Simplified Debts---------------\n";
        while(!credit.empty() && !debt.empty()){
            auto cred=credit.top(); credit.pop();
            auto deb=debt.top(); debt.pop();

            double amount=min(cred.first , deb.first);
            cout<<deb.second<<" pays "<< cred.second<<" : "<<amount<<endl;
            
            if(cred.first>amount){
                credit.push({cred.first-amount,cred.second});
            }
            if(deb.first>amount){
                debt.push({deb.first - amount, deb.second});
            }
        }
    }
};


int main(){
    User* A= new User("A", "Rahul");
    User* B= new User("B", "Sunil");
    User* C= new User("C", "Kamal");


    Group* group =new Group("id-1","Trip");

    group->addMember(A);
    group->addMember(B);
    group->addMember(C);

    vector<User*>participants={A,B,C};

    ExpenseServcie:: addExpense(group,"Dinner",300,A,participants,SplitType::EQUAL,{});
    ExpenseServcie:: addExpense(group,"Taxi",150, B,participants, SplitType:: EQUAL,{});
    BalanceSheetService::printBalances(group);
};