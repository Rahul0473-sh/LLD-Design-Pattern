classDiagram

class User {
  string id
  string name
}

class Split {
  User user
  double amount
}

class Expense {
  string description
  double amount
  User paidBy
  vector~Split~ splits
  SplitType type
}

class Group {
  string id
  string name
  vector~User~ members
  map~string, map~string, double~~ balances
  +addMember(User user)
}

class SplitStrategy {
  <<interface>>
  +split(amount, participants, metadata)
}

class EqualSplitStrategy {
  +split(...)
}

class PercentageSplitStrategy {
  +split(...)
}

class SplitStrategyFactory {
  +getStrategy(type)
}

class BalanceSheetService {
  +updateBalance(group, expense)
  +printBalances(group)
}

class ExpenseService {
  +addExpense(group, des, amount, paidBy, participants, type, metadata)
}

class DebtSimplificationService {
  +simplify(group)
}

%% Relationships
User <|-- Split : uses
Expense --> Split
Expense --> User : paidBy
Group --> User : members

SplitStrategy <|-- EqualSplitStrategy
SplitStrategy <|-- PercentageSplitStrategy

SplitStrategyFactory --> SplitStrategy
ExpenseService --> SplitStrategyFactory
ExpenseService --> BalanceSheetService
BalanceSheetService --> Group
BalanceSheetService --> Expense
DebtSimplificationService --> Group
