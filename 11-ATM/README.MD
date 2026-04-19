classDiagram

class User {
  +id: string
  +name: string
}

class Group {
  +id: string
  +name: string
  +members: List
  +balances: Map
}

class Expense {
  +description: string
  +amount: double
  +paidBy: User
  +splits: List
  +splitType: SplitType
}

class Split {
  +user: User
  +amount: double
}

class SplitStrategy
<<interface>> SplitStrategy

class EqualSplitStrategy {
  +split()
}

class PercentageSplitStrategy {
  +split()
}

class SplitStrategyFactory {
  +getStrategy()
}

class ExpenseService {
  +addExpense()
}

class BalanceSheetService {
  +updateBalances()
  +printBalances()
}

class DebtSimplificationService {
  +simplify()
}

User --> Group
Group --> Expense
Expense --> Split
Expense --> SplitStrategy
SplitStrategy <|-- EqualSplitStrategy
SplitStrategy <|-- PercentageSplitStrategy
SplitStrategyFactory --> SplitStrategy
ExpenseService --> SplitStrategyFactory
ExpenseService --> BalanceSheetService
BalanceSheetService --> Group
DebtSimplificationService --> Group
