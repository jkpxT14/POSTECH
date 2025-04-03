inputMoney=int(input("투입한 돈: "))
price=int(input("물건 값: "))
change=inputMoney-price
print("거스름돈: {}원".format(change))
print("500원짜리: {}개".format(change//500))
print("100원짜리: {}개".format((change%500)//100))