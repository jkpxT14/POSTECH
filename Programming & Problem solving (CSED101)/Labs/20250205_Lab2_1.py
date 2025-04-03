date=input("날짜(연-월-일)입력: ")
y=int(date.split("-")[0])
y+=10
m=int(date.split("-")[1])
d=int(date.split("-")[2])
print("입력한 날짜의 10년 후는 {}년 {}월 {}일".format(y, m, d))