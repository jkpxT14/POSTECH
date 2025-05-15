'''
2025학년도 1학기 <프로그래밍과 문제해결> Assignment #3
Problem: 붕어빵 타이쿤
담당교수: 윤은영
담당조교: 송민주
학번: 20250205
학과: 무은재학부
이름: 김정우 Jungwoo Kim
POVIS ID: jkpxt14
명예서약(Honor code): 저는 이 프로그래밍 과제를 다른 사람의 부적절한 도움 없이 완수하였습니다.
'''

import random
import sys

'''
filling = 속재료
red bean = 팥
cream = 슈크림
matcha = 말차
pizza = 피자
sweet potato = 고구마
dough = 반죽

filling(_type) Ex. "팥"
filling_list Ex. ["팥", "슈크림"]
filling_list_all Ex. [["팥"], ["슈크림"], ["팥", "슈크림"]]
'''

reset_color="\033[0m"

class Filling:
    def __init__(self, name, color, price, stock):
        self.name=name
        self.color=color
        self.price=price
        self.stock=stock
        return

    def raw(self):
        return self.color+"▒▒▒"+reset_color

    def extra_cost(self):
        return 0

class RedBean(Filling):
    def __init__(self, name="팥", color="\033[91m", price=1000, stock=10):
        super().__init__(name, color, price, stock)
        return

class Cream(Filling):
    def __init__(self, name="슈크림", color="\033[93m", price=1000, stock=8):
        super().__init__(name, color, price, stock)
        return

    def extra_cost(self):
        return 100

class Matcha(Filling):
    def __init__(self, name="말차", color="\033[92m", price=1000, stock=10):
        super().__init__(name, color, price, stock)
        return
    
    def extra_cost(self):
        return 200

class Pizza(Filling):
    def __init__(self, name="피자", color="\033[33m", price=1000, stock=8):
        super().__init__(name, color, price, stock)
        return

    def extra_cost(self):
        return 300

class SweetPotato(Filling):
    def __init__(self, name="고구마", color="\033[95m", price=1000, stock=7):
        super().__init__(name, color, price, stock)
        return

    def extra_cost(self):
        return 400

class Customer:
    def __init__(self, game_manager, filling_number=1):
        self.game_manager=game_manager
        self.filling_number=filling_number
        self.filling_list_all=[]
    
    def order(self):
        while True:
            fishbread_number=input("몇 개 주문하시나요? (q: 종료): ")
            try:
                fishbread_number=int(fishbread_number)
            except:
                if fishbread_number=="q":
                    self.game_manager.end_game()
                print("숫자로 정확히 입력해 주세요!\n")
                continue
            else:
                if fishbread_number<=0:
                    print("1개 이상 주문해주세요!\n")
                    continue
                else:
                    break

        self.filling_list_all=[]
        print(f"{separating_line}각 붕어빵에 들어갈 속재료({self.filling_number}개)를 입력하세요.\n{separating_line}", end="")
        for i in range(fishbread_number):
            if self.filling_number==1:
                filling_list=input(f"{i+1}번째 붕어빵 재료: ")
            else:
                filling_list=input(f"{i+1}번째 붕어빵 재료 (콤마로 구분): ")
            filling_list=filling_list.split(", ")
            self.filling_list_all.append(filling_list)
        print(separating_line, end="")

    def filling_count_validation(self):
        for filling_list in self.filling_list_all:
            if len(filling_list)!=self.filling_number:
                print("일반 손님은 한 가지 맛만 주문할 수 있습니다! 손님 다시 받기\n")
                return False
        return True

    def filling_type_validation(self):
        invalidate_filling_type_set=set()
        for filling_list in self.filling_list_all:
            for filling in filling_list:
                if filling not in set(list(filling_type_kor_index.keys())):
                    invalidate_filling_type_set.add(filling)
        if invalidate_filling_type_set!=set():
            msg=""
            invalidate_filling_type_list=list(invalidate_filling_type_set)
            for i in range(len(invalidate_filling_type_list)):
                if i==len(invalidate_filling_type_list)-1:
                    msg+=f"'{invalidate_filling_type_list[i]}'은(는) 존재하지 않는 재료입니다! 손님 다시 받기\n"
                else:
                    msg+=f"'{invalidate_filling_type_list[i]}', "
            print(msg)
            return False
        return True
    
    def filling_sufficient_validation(self):
        required_filling_quantity_list=[0 for i in range(len(list(filling_type_kor_index.keys())))]
        for filling_list in self.filling_list_all:
            for filling in filling_list:
                required_filling_quantity_list[filling_type_kor_index[filling]]+=1
        insufficient_filling_set=set()
        for i in range(len(list(filling_type_kor_index.keys()))):
            if self.game_manager.inventory[i].stock<required_filling_quantity_list[i]:
                insufficient_filling_set.add(list(filling_type_kor_index.keys())[i])
        if insufficient_filling_set!=set():
            msg=""
            insufficient_filling_list=list(insufficient_filling_set)
            for i in range(len(insufficient_filling_list)):
                if i==len(insufficient_filling_list)-1:
                    msg+=f"{insufficient_filling_list[i]} 속재료가 부족합니다."
                else:
                    msg+=f"{insufficient_filling_list[i]}, "
            print(msg)
            return False
        return True
    
    def filling_sufficient_revalidation(self, current):
        required_filling_quantity_list=[0 for i in range(len(list(filling_type_kor_index.keys())))]
        for i in range(current, len(self.filling_list_all)):
            for filling in self.filling_list_all[i]:
                required_filling_quantity_list[filling_type_kor_index[filling]]+=1
        insufficient_filling_set=set()
        for i in range(len(list(filling_type_kor_index.keys()))):
            if self.game_manager.inventory[i].stock<required_filling_quantity_list[i]:
                insufficient_filling_set.add(list(filling_type_kor_index.keys())[i])
        if insufficient_filling_set!=set():
            msg=""
            insufficient_filling_list=list(insufficient_filling_set)
            for i in range(len(insufficient_filling_list)):
                if i==len(insufficient_filling_list)-1:
                    msg+=f"{insufficient_filling_list[i]} 속재료가 부족합니다."
                else:
                    msg+=f"{insufficient_filling_list[i]}, "
            print(msg)
            return False
        return True
    
class RareCustomer(Customer):
    def __init__(self, game_manager, filling_number=2):
        super().__init__(game_manager, filling_number)

    def filling_count_validation(self):
        for filling_list in self.filling_list_all:
            filling_set=set(filling_list)
            if len(filling_set)!=self.filling_number:
                print("레어 손님은 두 가지 맛을 주문할 수 있습니다! 손님 다시 받기\n")
                return False
        return True

class Mold:
    def __init__(self, game_manager, filling_list):
        self.game_manager=game_manager
        self.filling_list=filling_list

    def dough_shape(self):
        return [
            "    ___    ",
            "   /   \\   ",
            "  /     \\  ",
            "  \\     /  ",
            "   \\___/   "
        ]
    
    def draw_filling(self, filling):
        return f"/ {self.game_manager.inventory[filling_type_kor_index[filling]].raw()} \\"

    def draw_combined_fillings(self, filling_list):
        if not filling_list:
            combo = "▒▒▒"
        else:
            color_seq = []
            for i in range(3):
                f = self.game_manager.inventory[filling_type_kor_index[filling_list[i % len(filling_list)]]]
                color_seq.append(f.color + "▒")
            combo = ''.join(color_seq) + reset_color
        return f"/ {combo} \\"
    
    def take_out_shape(self, filling_list):
        if len(filling_list) == 1:
            inner_line = self.draw_filling(filling_list[0])
        else:
            inner_line = self.draw_combined_fillings(filling_list)
        return [
            "   _____   ",
            "   \\___/   ",
            "   / _ \\   ",
            "  "+inner_line+"  ",
            "  \\ \\_/ /  ",
            "   \\___/   "
        ]
        
    def pour_dough(self):  
        if self.game_manager.money<cost["dough"]:
            print(f"{separating_line}돈이 부족해서 반죽을 붓지 못했습니다.\n{separating_line}", end="")
            self.game_manager.end_game()
        else:
            print(">> 반죽을 붓습니다!")
            self.game_manager.money-=cost["dough"]
            print(f"{int(cost["dough"])}원을 사용했습니다. (현재 자본금: {int(self.game_manager.money)}원)")
            for line in self.dough_shape():
                print(line)
            print(separating_line, end="")

    def add_filling(self):
        if self.game_manager.money<len(self.filling_list)*cost["filling"]:
            print(f"돈이 부족해서 속을 채우지 못했습니다.\n{separating_line}", end="")
            self.game_manager.end_game()
        else:
            filling_list_str=""
            for i in range(len(self.filling_list)):
                if i==len(self.filling_list)-1:
                    filling_list_str+=f"{self.filling_list[i]}"
                else:
                    filling_list_str+=f"{self.filling_list[i]}+"
            print(f">> '{filling_list_str}'속을 채웁니다!")

            for filling in self.filling_list:
                self.game_manager.inventory[filling_type_kor_index[filling]].stock-=1
            
            self.game_manager.money-=len(self.filling_list)*cost["filling"]
            print(f"{int(len(self.filling_list)*cost["filling"])}원을 사용했습니다. (현재 자본금: {int(self.game_manager.money)}원)")

            print(separating_line, end="")
            return
    
    def bake(self):
        bakingstatus=BakingStatus(sorted(random.sample(range(4, 11), 2)))
        while True:
            baking_time=input("몇 초 굽겠습니까? (q: 종료): ")
            try:
                baking_time=int(baking_time)                
            except:
                if baking_time=="q":
                    self.game_manager.end_game()
                print("숫자로 입력해주세요!\n")
                continue
            else:
                if baking_time<=0:
                    print("양수로 입력해 주세요!\n")
                    continue
                bakingstatus.time+=baking_time
                if bakingstatus.is_burnt():
                    print("[Warning] 타버렸습니다! 다시 반죽부터 시작하세요.")
                    while True:
                        new_customer_decision=input("주문을 거절하고 손님을 다시 받으시겠습니까? (y/n): ")
                        if new_customer_decision not in ["y", "n"]:
                            print("y/n으로 입력하세요.")
                            continue
                        elif new_customer_decision=="y":
                            return 1
                        elif new_customer_decision=="n":
                            print(separating_line, end="")
                            return 2
                elif bakingstatus.is_raw():
                    print(f"[Warning] 설익었습니다! 조금 더 익혀주세요.\n{separating_line}", end="")
                    continue
                else:
                    print(f">> 총 '{bakingstatus.time}'초 동안 구웠습니다.")
                    return 3

    def take_out(self):
        for line in self.take_out_shape(self.filling_list):
            print(line)
        
        filling_list_str=""
        for i in range(len(self.filling_list)):
            if i==len(self.filling_list)-1:
                filling_list_str+=f"{self.filling_list[i]}"
            else:
                filling_list_str+=f"{self.filling_list[i]}+"
        print(f">> {filling_list_str} 붕어빵 완성")

        earned_money=0
        for filling in self.filling_list:
            earned_money+=self.game_manager.inventory[filling_type_kor_index[filling]].extra_cost()
        earned_money/=len(self.filling_list); earned_money+=1000; self.game_manager.money+=earned_money
        print(f">> {int(earned_money)}원을 벌었습니다. (현재 자본금: {int(self.game_manager.money)}원)\n{separating_line}")

    def baking(self):
        self.pour_dough()
        self.add_filling()
        bake=self.bake()
        if bake==3:
            self.take_out()
        return bake

class AutoMold(Mold):
    def __init__(self, game_manager, filling_list_all):
        self.game_manager=game_manager
        self.filling_list_all=filling_list_all
    
    def pour_dough_all(self):
        if self.game_manager.money<len(self.filling_list_all)*cost["dough"]:
            print(f"{separating_line}돈이 부족해서 반죽을 붓지 못했습니다.\n{separating_line}", end="")
            self.game_manager.end_game()
        else:
            print("[Auto] 반죽을 한 번에 붓습니다!")
            self.game_manager.money-=len(self.filling_list_all)*cost["dough"]
            print(f"{int(len(self.filling_list_all)*cost["dough"])}원을 사용했습니다. (현재 자본금: {int(self.game_manager.money)}원)")
            dough_shape_all=[]
            dough_shape_all_str=""
            for i in range(len(self.filling_list_all)):
                dough_shape_all.append(self.dough_shape())
            for i in range(len(self.dough_shape())):
                for j in range(len(self.filling_list_all)):
                    if j==len(self.filling_list_all)-1:
                        dough_shape_all_str+=f"{dough_shape_all[j][i]}\n"
                    else:
                        dough_shape_all_str+=f"{dough_shape_all[j][i]}    "
            print(dough_shape_all_str)
            print(separating_line, end="")

    def add_filling_all(self):
        for filling_list in self.filling_list_all:
            Mold(self.game_manager, filling_list).add_filling()
    
    def bake_all(self):
        print(f"빠르게 굽는 중...\n{separating_line}")
        return

    def take_out_all(self):
        earned_money_all=0

        take_out_shape_all=[]
        take_out_shape_all_str=""
        for filling_list in self.filling_list_all:
            take_out_shape_all.append(self.take_out_shape(filling_list))
        for i in range(len(self.take_out_shape(self.filling_list_all[0]))):
            for j in range(len(self.filling_list_all)):
                if j==len(self.filling_list_all)-1:
                    take_out_shape_all_str+=f"{take_out_shape_all[j][i]}\n"
                else:
                    take_out_shape_all_str+=f"{take_out_shape_all[j][i]}    "
        print(take_out_shape_all_str)

        for filling_list in self.filling_list_all:
            filling_list_str=""
            for i in range(len(filling_list)):
                if i==len(filling_list)-1:
                    filling_list_str+=f"{filling_list[i]}"
                else:
                    filling_list_str+=f"{filling_list[i]}+"
            print(f">> {filling_list_str} 붕어빵 완성")
            earned_money=0
            for filling in filling_list:
                earned_money+=self.game_manager.inventory[filling_type_kor_index[filling]].extra_cost()
            earned_money/=len(filling_list); earned_money+=1000; earned_money_all+=earned_money
        self.game_manager.money+=earned_money_all
        print(f">> {int(earned_money_all)}원을 벌었습니다. (현재 자본금: {int(self.game_manager.money)}원)\n{separating_line}")

    def baking_all(self):
        self.pour_dough_all()
        self.add_filling_all()
        self.bake_all()
        self.take_out_all()
        return

class BakingStatus:
    def __init__(self, limits):
        self.limits=limits
        self.time=0
        return

    def is_burnt(self):
        return self.time>self.limits[1]

    def is_raw(self):
        return self.time<self.limits[0]
    
class GameManager:
    def __init__(self, shop_name="KYL", init_money=10000):
        if shop_name=="":
            shop_name="KYL"
        self.shop_name=shop_name
        self.money=init_money
        self.auto_mold=False
        self.inventory=[RedBean(), Cream(), Matcha(), Pizza(), SweetPotato()]
    
    def open_shop(self):
        print(f"{separating_line}'{self.shop_name}'네 붕어빵 오픈 완료! 초기 자본금: {int(self.money)}원\n{separating_line}")
        return

    def print_menu(self):
        print(f"{separating_line}[메뉴판] {self.inventory[0].name}(재고: {self.inventory[0].stock}), {self.inventory[1].name}(재고: {self.inventory[1].stock}), {self.inventory[2].name}(재고: {self.inventory[2].stock}), {self.inventory[3].name}(재고: {self.inventory[3].stock}), {self.inventory[4].name}(재고: {self.inventory[4].stock})\n{separating_line}", end="")
        return

    def run_out_of_all_fillings(self):
        run_out_of_all_fillings=True
        for i in range(len(filling_type_kor_index)):
            if self.inventory[i].stock!=0:
                run_out_of_all_fillings=False
        return run_out_of_all_fillings

    def run_game(self):
        self.open_shop()
        reorder=False
        redough=False
        auto_mold_yes_or_no="n"
        while True:
            if self.run_out_of_all_fillings():
                self.print_menu()
                print(f"모든 재료의 재고가 소진되었습니다. 게임을 종료합니다.\n{separating_line}", end="")
                self.end_game()

            self.print_menu()
            if not reorder:
                customer_type=input("손님 유형 선택 (1: 일반 혹은 2: 레어, q: 종료): ")
                if customer_type=="q":
                    self.end_game()
                elif customer_type=="1":
                    customer=Customer(self)
                elif customer_type=="2":
                    customer=RareCustomer(self)
                else:
                    print("1 또는 2를 입력해주세요!\n")
                    continue
            reorder=False

            customer.order()
            if not customer.filling_count_validation():
                continue
            elif not customer.filling_type_validation():
                continue
            elif not customer.filling_sufficient_validation():
                random_decision=random.randint(1, 2)
                if random_decision==1:
                    print("손님이 나가셨습니다. 다시 손님을 받습니다.\n")
                    continue
                elif random_decision==2:
                    print("다른 재료로 주문받습니다.")
                    reorder=True
                    continue

            if self.auto_mold:
                while True:
                    auto_mold_yes_or_no=input("[Auto] 자동으로 붕어빵을 만드시겠습니까? (y/n): ")
                    if auto_mold_yes_or_no not in ["y", "n"]:
                        print("y/n으로 입력하세요.\n")
                        continue
                    else:
                        print(separating_line, end="")
                        break

            if auto_mold_yes_or_no=="y":
                automold=AutoMold(self, customer.filling_list_all)
                automold.baking_all()

            elif auto_mold_yes_or_no=="n":
                i=0
                while i<len(customer.filling_list_all):
                    if redough:
                        if not customer.filling_sufficient_revalidation(i):
                            random_decision=random.randint(1, 2)
                            if random_decision==1:
                                print("손님이 나가셨습니다. 다시 손님을 받습니다.\n")
                                break
                            elif random_decision==2:
                                print("다른 재료로 주문받습니다.")
                                reorder=True
                                break
                    elif not redough:
                        print(f"[{i+1}번째 붕어빵: {", ".join(customer.filling_list_all[i])}]")
                        mold=Mold(self, customer.filling_list_all[i])
                    redough=False

                    i+=1

                    baking=mold.baking()
                    if baking==1:
                        print("")
                        break
                    elif baking==2:
                        i-=1
                        redough=True
                    # elif baking==3:
                    #     pass

                if not self.auto_mold and self.money>=15000:
                    print("수익이 5000원을 돌파했습니다! AutoMold 기능이 추가됩니다!\n")
                    self.auto_mold=True

    def end_game(self):
        if self.money-10000<0:
            print(f"'{self.shop_name}'네 붕어빵을 종료합니다. (총 수익금: 0원)\n{separating_line}", end="")
        else:
            print(f"'{self.shop_name}'네 붕어빵을 종료합니다. (총 수익금: {int(self.money-10000)}원)\n{separating_line}", end="")
        sys.exit(0)

# filling_type_eng_index={"RedBean": 0, "Cream": 1, "Matcha": 2, "Pizza": 3, "SweetPotato": 4}
filling_type_kor_index={"팥": 0, "슈크림": 1, "말차": 2, "피자": 3, "고구마": 4}
cost={"dough": 100, "filling": 200}
separating_line="========================================\n"

if __name__ == '__main__':
    shop_name = input("가게 이름을 입력하세요: ")
    game_manager = GameManager(shop_name)
    game_manager.run_game()

    # game_manager = GameManager()
    # mold = Mold(game_manager, 10000)

    # f1 = RedBean()
    # f2 = Cream()

    # print("1. 단일 속재료:")
    # print(mold.draw_filling(f1.name))

    # print("\n2. 혼합 속재료:")
    # print(mold.draw_combined_fillings([f1.name, f2.name]))

    # print("3. 완성 붕어빵(단일):")
    # fb1 = mold.take_out_shape([f1.name])
    # for line in fb1:
    #     print(line)

    # print("4. 완성 붕어빵(혼합):")
    # fb2 = mold.take_out_shape([f1.name, f2.name])
    # for line in fb2:
    #     print(line)