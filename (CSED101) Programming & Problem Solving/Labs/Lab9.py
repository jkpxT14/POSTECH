class BankAccount:
    def __init__(self, name="None", balance=0):
        self.name=name
        self.balance=balance

    def deposit(self, amount):
        self.balance+=amount

    def withdraw(self, amount):
        if self.balance<amount:
            print("잔액 부족!")
        else:
            self.balance-=amount

    def get_info(self):
        print(f"이름: {self.name}, 잔액: {self.balance}")

    def transfer(self, amount, target):
        if self.balance<amount:
            print("잔액 부족!")
        else:
            self.balance-=amount
            target.balance+=amount

    def __str__(self):
        return f"이름: {self.name}, 잔액: {self.balance}"
    