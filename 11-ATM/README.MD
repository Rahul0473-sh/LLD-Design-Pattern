classDiagram

%% =======================
%% ATM SYSTEM
%% =======================
class ATMSystem {
    -AtomicLong transactionCounter
    -ATMState currentState
    -Card currentCard
    -CashDispenser cashDispenser
    -BankService bankService

    +setOperation(OperationType, int)
    +getCard(String) Card
    +enterPin(String)
    +depositCash(int)
    +changeState(ATMState)
    +getCurrentCard() Card
    +withdrawCash(int)
    +getInstance() ATMSystem
    +checkBalance()
    +authenticate(String) boolean
    +getBankService() BankService
    +setCurrentCard(Card)
    +insertCard(String)
}

%% =======================
%% ENUM
%% =======================
class OperationType {
    <<enumeration>>
    WITHDRAW_CASH
    DEPOSIT_CASH
    CHECK_BALANCE
}

%% =======================
%% STATE PATTERN
%% =======================
class ATMState {
    <<interface>>
    +insertCard(ATMSystem, String)
    +enterPin(ATMSystem, String)
    +selectOperation(ATMSystem, OperationType, int)
    +ejectCard(ATMSystem)
}

class IdleState
class HasCardState
class AuthenticatedState

ATMState <|.. IdleState
ATMState <|.. HasCardState
ATMState <|.. AuthenticatedState

ATMSystem --> ATMState

%% =======================
%% BANK SERVICE
%% =======================
class BankService {
    -Map~String, Card~ cards
    -Map~Card, Account~ cardAccountMap
    -Map~String, Account~ accounts

    +linkCardToAccount(Card, Account)
    +getBalance(Card) double
    +depositMoney(Card, double)
    +createAccount(String, double) Account
    +createCard(String, String) Card
    +getCard(String) Card
    +withdrawMoney(Card, double)
    +authenticate(Card, String) boolean
}

ATMSystem --> BankService

%% =======================
%% ACCOUNT & CARD
%% =======================
class Account {
    -Map~String, Card~ cards
    -double balance
    -String accountNumber

    +withdraw(double) boolean
    +deposit(double)
}

class Card {
    -String cardNumber
    -String pin
}

BankService --> Account
BankService --> Card
Account --> Card

%% =======================
%% CASH DISPENSER (CHAIN OF RESPONSIBILITY)
%% =======================
class CashDispenser {
    -DispenseChain chain
    +canDispenseCash(int) boolean
    +dispenseCash(int)
}

class DispenseChain {
    <<interface>>
    +dispense(int)
    +setNextChain(DispenseChain)
    +canDispense(int) boolean
}

class NoteDispenser {
    -int noteValue
    -DispenseChain nextChain
    -int numNotes

    +dispense(int)
    +canDispense(int) boolean
    +setNextChain(DispenseChain)
}

class NoteDispenser100
class NoteDispenser50
class NoteDispenser20

DispenseChain <|.. NoteDispenser
NoteDispenser <|-- NoteDispenser100
NoteDispenser <|-- NoteDispenser50
NoteDispenser <|-- NoteDispenser20

CashDispenser --> DispenseChain
ATMSystem --> CashDispenser