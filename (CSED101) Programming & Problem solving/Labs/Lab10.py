class BankAccount:
    def __init__(self, name="None", balance=0):
        self.name=name
        self.balance=balance
        return

    def deposit(self, amount):
        self.balance+=amount
        return

    def withdraw(self, amount):
        if self.balance<amount:
            print("잔액 부족!")
            return
        else:
            self.balance-=amount
            return

    def get_info(self):
        print(f"이름: {self.name}, 잔액: {self.balance}")
        return

    def transfer(self, amount, target):
        if self.balance<amount:
            print("잔액 부족!")
            return
        else:
            self.balance-=amount
            target.balance+=amount
            return

    def __str__(self):
        return f"이름: {self.name}, 잔액: {self.balance}"
    
    def __add__(self, amount):
        self.deposit(amount)
        return self

    def __iadd__(self, amount):
        self.deposit(amount)
        return self

    def __sub__(self, amount):
        self.withdraw(amount)
        return self

    def __isub__(self, amount):
        self.withdraw(amount)
        return self

class MinBalAccount(BankAccount):
    def __init__(self, name="None", balance=0, min_bal=0):
        super().__init__(name, balance)
        self.min_bal=min_bal

    def withdraw(self, amount):
        if self.balance-self.min_bal<amount:
            print(f"최소 잔액({self.min_bal})을 유지해야 합니다!")
            return
        else:
            self.balance-=amount
            return