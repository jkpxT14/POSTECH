'''
주석(Comment) 작성 언어: 한국어, English (주석을 작성할 때 두 언어를 혼용한 점 양해 부탁드립니다.)
한 줄 짜리 주석은 #으로, 두 줄 이상의 주석은 ''''''으로 작성하였습니다.
'''

'''
2025학년도 1학기 <프로그래밍과 문제해결> Assignment #1
Problem: 다마고치 키우기(Raising Tamagotchi)
담당교수: 윤은영
학번: 20250205
학과: 무은재학부
이름: 김정우
POVIS ID: jkpxt14
명예서약(Honor code): 저는 이 프로그래밍 과제를 다른 사람의 부적절한 도움 없이 완수하였습니다.
'''

'''
준말(Abbreviations)
    gl=growth level=성장 단계
    dm=daily mission=데일리 미션
    gm=growth mission=성장 미션
    _p=_parameter
'''

'''
Defining functions
Functions(9):
    print_Tamagotchi: 현재 성장 단계에 맞는 다마고치 이미지를 출력하는 함수이다.
    print_current_status: 다마고치의 현재 상태 화면(이하 "상태 화면")을 출력하는 함수이다.
    daily_mission_update: 데일리 미션이 입력될 때마다 오늘의 데일리 미션 수행 여부를 업데이트하는 함수이다.
    growth_mission_update: 성장 미션이 완료될 때마다 현재 성장 단계의 성장 미션 수행 여부를 업데이트하는 함수이다.
    is_daily_complete: 오늘 세 가지 데일리 미션을 모두 수행하였는지 여부를 확인하는 함수이다.
    is_growth_complete: 현재 성장 단계의 성장 미션을 수행하였는지 여부를 확인하는 함수이다.
    is_growth_right: 입력 받은 성장 미션이 현재 성장 단계에 알맞은 성장 미션인지 확인하는 함수이다.
    growth_check: 현재 성장 단계의 데일리 미션과 성장 미션을 모두 완료해 다마고치가 성장할 때가 되었는지를 판단하는 함수이다.
    next_growth_status: 성장 단계를 다음 단계로 변경하는 함수이다.
'''
def print_Tamagotchi(gl_p):
    '''
    현재 성장 단계에 맞는 다마고치 이미지를 출력하는 함수이다.

    Parameters(1):
        gl_p: 현재 성장 단계를 매개변수로 받는다.

    Prints:
        현재 성장 단계에 맞는 다마고치 이미지를 출력한다.

    '''
    if gl_p==0:
        # print("알")
        print("        ⬛⬛⬛⬛")
        print("     ⬛⬜⬜⬜⬜⬛")
        print("    ⬛⬜⬜⬜⬜⬜⬛")
        print("    ⬛⬜⬜⬜⬜⬜⬛")
        print("      ⬛⬛⬛⬛⬛\n")
    elif gl_p==1:
        # print("영유아")
        print("        ⬛⬛⬛⬛⬛")
        print("     ⬛⬜⬜⬜⬜⬜⬛")
        print("    ⬛⬜⬜⬜⬜⬜⬜⬛")
        print("    ⬛⬜⬛⬜⬜⬛⬜⬛")
        print("    ⬛⬜⬜⬜⬜⬜⬜⬛")
        print("       ⬛⬛⬛⬛⬛⬛\n")
    elif gl_p==2:
        # print("어린이")
        print("        ⬛⬛⬛⬛⬛")
        print("     ⬛⬜⬜⬜⬜⬜⬛")
        print("    ⬛⬜⬜⬜⬜⬜⬜⬛")
        print("    ⬛⬜⬛⬜⬜⬛⬜⬛")
        print("    ⬛⬜⬜⬜⬜⬜⬜⬛")
        print("       ⬛⬛⬛⬛⬛⬛")
        print("          ⬛     ⬛\n")
    elif gl_p==3:
        # print("청소년")
        print("              ⬛   ")
        print("           ⬛⬛⬛⬛⬛")
        print("        ⬛⬜⬛⬜⬜⬜⬛")
        print("       ⬛⬜⬜⬜⬜⬜⬜⬛")
        print("    ⬛⬛⬜⬛⬜⬜⬛⬜⬛⬛")
        print("       ⬛⬜⬜⬜⬜⬜⬜⬛")
        print("          ⬛⬛⬛⬛⬛⬛")
        print("             ⬛     ⬛")
        print("             ⬛     ⬛\n")
    elif gl_p==4:
        # print("성체")
        print("              ⬛   ⬛")
        print("           ⬛⬛⬛⬛⬛")
        print("        ⬛⬜⬛⬛⬛⬜⬛")
        print("       ⬛⬜⬜⬜⬜⬜⬜⬛")
        print("    ⬛⬛⬜⬛⬜⬜⬛⬜⬛⬛")
        print("       ⬛⬜⬜⬜⬜⬜⬜⬛")
        print("       ⬛⬜⬜⬜⬜⬜⬜⬛")
        print("          ⬛⬛⬛⬛⬛⬛")
        print("             ⬛     ⬛")
        print("             ⬛     ⬛")
        print("             ⬛     ⬛")
        print("          ⬛⬛   ⬛⬛\n")
    elif gl_p==5:
        # print("다마고치 죽음")
        print("         ⬛")
        print("       ⬛  ⬛")
        print("    ⬛  X  X  ⬛")
        print("       ⬛  ⬛")
        print("         ⬛\n")
        
    return

def print_current_status(today_p, gl_p, today_dm_status_p, current_gl_dm_status_p, current_gl_gm_status_p, gl_list_p, XO_p):
    '''
    다마고치의 현재 상태 화면(이하 "상태 화면")을 출력하는 함수이다.

    print_Tamagotchi가 다마고치의 이미지만을 출력하는 함수라면, print_current_status는
    print_Tamagotchi를 활용하는 함수로 오늘이 며칠차인지, 다마고치가 현재 어떤 성장 단계에
    있는지, 데일리 미션과 성장 미션의 진행 상황은 어떤지를 모두 담은 상태 화면을 게임을
    시작하거나 미션을 입력할 때마다 출력하는 함수이다.

    Parameters(7):
        today_p: 오늘이 며칠차인지를 매개변수로 받는다.
        gl_p: 현재 성장 단계를 매개변수로 받는다.
        today_dm_status_p: 오늘의 데일리 미션 진행 상황을 매개변수로 받는다.
        current_gl_dm_status_p: 현재 성장 단계의 데일리 미션 진행 상황을 매개변수로 받는다.
        current_gl_gm_status_p: 현재 성장 단계의 성장 미션 진행 상황을 매개변수로 받는다.
        gl_list_p: 성장 단계 목록을 매개변수로 받는다.
        XO_p: X, O 출력을 위한 리스트를 매개변수로 받는다.

    Prints:
        상태 화면을 출력한다.

    '''
    print(f"[{today_p}일차] - 성장단계: {gl_list_p[gl_p]}\n")
    print(f"오늘 진행된 데일리 미션: 먹이주기({XO_p[today_dm_status_p[0]]}), 배설물치우기({XO_p[today_dm_status_p[1]]}), 샤워시키기({XO_p[today_dm_status_p[2]]})\n")
    print_Tamagotchi(gl_p)
    print(f"({gl_list_p[gl_p]})단계 데일리 미션: {XO_p[current_gl_dm_status_p]}\n")
    print(f"({gl_list_p[gl_p]})단계 성장 미션: {XO_p[current_gl_gm_status_p]}\n")
    print(separating_line)
    return

def daily_mission_update(mission_p, today_dm_status_p):
    '''
    데일리 미션이 입력될 때마다 오늘의 데일리 미션 수행 여부를 업데이트하는 함수이다.
    이때 이미 완료된 데일리 미션의 경우 안내 문구를 출력한다.

    Parameters(2):
        mission_p: 입력된 미션을 매개변수로 받는다.
        today_dm_status_p: 오늘의 데일리 미션 진행 상황을 매개변수로 받는다.

    Prints:
        이미 완료된 데일리 미션의 경우 안내 문구를 출력한다.

    Returns:
        today_dm_status_p: 오늘의 데일리 미션 진행 상황을 업데이트하여 return한다.
    '''
    if mission_p=="먹이주기":
        if today_dm_status_p[0]==1:
            print(f"({mission_p})는 이미 완료된 데일리 미션입니다.\n{separating_line}")
        today_dm_status_p[0]=1
    elif mission_p=="배설물치우기":
        if today_dm_status_p[1]==1:
            print(f"({mission_p})는 이미 완료된 데일리 미션입니다.\n{separating_line}")
        today_dm_status_p[1]=1
    else: # elif mission_p=="샤워시키기"
        if today_dm_status_p[2]==1:
            print(f"({mission_p})는 이미 완료된 데일리 미션입니다.\n{separating_line}")
        today_dm_status_p[2]=1
    return today_dm_status_p

def growth_mission_update(mission_p, current_gl_gm_status_p):
    '''
    성장 미션이 완료될 때마다 현재 성장 단계의 성장 미션 수행 여부를 업데이트하는 함수이다.
    이미 완료된 성장 미션의 경우 안내 문구를 출력한다.

    Parameters(2):
        mission_p: 입력된 미션을 매개변수로 받는다.
        current_gl_gm_status_p: 현재 성장 단계의 성장 미션 진행 상황을 매개변수로 받는다.

    Prints:
        이미 완료된 성장 미션의 경우 안내 문구를 출력한다.

    Returns:
        current_gl_gm_status_p: 현재 성장 단계의 미션 진행 상황을 업데이트하여 return한다.
    '''
    if current_gl_gm_status_p==1:
        print(f"({mission_p})는 이미 완료된 성장 미션입니다.\n{separating_line}")
    current_gl_gm_status_p=1
    return current_gl_gm_status_p

def is_daily_complete(today_dm_status_p):
    '''
    오늘 세 가지 데일리 미션을 모두 수행하였는지 여부를 확인하는 함수이다.

    Parameters(1):
        today_dm_status_p: 오늘의 데일리 미션 진행 상황을 매개변수로 받는다.

    Returns:
        True/False: 오늘 세 가지 데일리 미션을 모두 수행하였다면 True, 수행하지 않았다면 False를 return한다.
    '''
    if today_dm_status_p==[1, 1, 1]:
        return True
    else:
        return False

def is_growth_complete(current_gl_gm_status_p):
    '''
    현재 성장 단계의 성장 미션을 수행하였는지 여부를 확인하는 함수이다.

    Parameters(1):
        current_gl_gm_status_p: 현재 성장 단계의 성장 미션 진행 상황을 매개변수로 받는다.

    Returns:
        True/False: 현재 성장 단계의 성장 미션을 수행하였다면 True, 수행하지 않았다면 False를 return한다.
    '''
    if current_gl_gm_status_p==1:
        return True
    else:
        return False

def is_growth_right(mission_p, gl_p, gm_list_p):
    '''
    입력 받은 성장 미션이 현재 성장 단계에 알맞은 성장 미션인지 확인하는 함수이다.

    Parameters(3):
        mission_p: 입력된 미션을 매개변수로 받는다.
        gl_p: 현재 성장 단계를 매개변수로 받는다.
        gm_list_p: 성장 미션 목록을 매개변수로 받는다.

    Returns:
        입력 받은 성장 미션이 현재 성장 단계에 맞다면 True를, 맞지 않는다면 False를 return한다.
    '''
    if mission_p==gm_list_p[gl_p]:
        return True
    else:
        return False

def growth_check(current_gl_dm_status_p, current_gl_gm_status_p):
    '''
    현재 성장 단계의 데일리 미션과 성장 미션을 모두 완료해 다마고치가 성장할 때가 되었는지를 판단하는 함수이다.

    Parameters(2):
        current_gl_dm_status_p: 현재 성장 단계의 데일리 미션 진행 상황을 매개변수로 받는다.
        current_gl_gm_status_p: 현재 성장 단계의 성장 미션 진행 상황을 매개변수로 받는다.

    Returns:
        True/False: 성장에 필요한 데일리 미션과 성장 미션을 모두 완료했다면 True를, 완료하지 않았다면 False를 return한다.
    '''
    if current_gl_dm_status_p==1 and current_gl_gm_status_p==1:
        return True
    else:
        return False

def next_growth_status(gl_p, gl_list_p):
    '''
    성장 단계를 다음 단계로 변경하는 함수이다.

    Parameters(2):
        gl_p: 현재 성장 단계를 매개변수로 받는다.
        gl_list_p: 성장 단계 목록을 매개변수로 받는다.

    Prints:
        다마고치가 성공적으로 성장했음을 알리는 안내 문구를 출력한다.

    Returns:
        gl_p: 성장 단계를 업데이트해 return한다.
    '''
    gl_p+=1
    print(f"다마고치가 성공적으로 {gl_list_p[gl_p]}이(가) 되었습니다.\n{separating_line}")
    return gl_p


'''
[Defining variables]
Variables(10):
    today: 오늘이 며칠차인지 나타내는 변수이다.
    gl: 현재 성장 단계를 나타내는 변수이다. 0단계부터 5단계까지 각각 알, 영유아, 어린이, 청소년, 성체, 죽음 단계에 해당한다.
    today_dm_status: 오늘의 데일리 미션 3가지의 진행 여부를 나타내는 변수이다.
    current_gl_dm_status: 현재 성장 단계에서의 데일리 미션 진행 여부를 나타내는 변수이다.
    current_gl_gm_status: 현재 성장 단계에서의 성장 미션 진행 여부를 나타내는 변수이다.
    gl_list: 성장 단계 목록이다.
    dm_list: 데일리 미션 목록이다.
    gm_list: 성장 미션 목록이다.
    XO: X와 O를 출력하는데 사용되는 변수이다. Ex. 먹이주기("O")
    separating_line: 출력 단계에서 반복적으로 사용되는 구분선을 편의를 위해 변수에 저장해두었다.
'''
today=1 # 1일차부터 시작하므로 today를 1로 초기화한다.
gl=0 # 다마고치는 알 단계부터 시작하므로 gl을 0으로 초기화한다.
today_dm_status=[0, 0, 0] # 게임 시작 단계에서는 그 어떤 미션도 수행되지 않은 상태이므로 모두 0으로 초기화한다. (0: 수행하지 않음, 1: 수행함)
current_gl_dm_status=0
current_gl_gm_status=0

gl_list=["알", "영유아", "어린이", "청소년", "성체", "다마고치 죽음"]
dm_list=["먹이주기", "배설물치우기", "샤워시키기"]
gm_list=["잠재우기", "쓰다듬기", "놀아주기", "학교보내기"]
XO=["X", "O"]
separating_line="========================================\n"

# [Main]
print(f"{separating_line}[다마고치 키우기]\n{separating_line}")
print_current_status(today, gl, today_dm_status, current_gl_dm_status, current_gl_gm_status, gl_list, XO)
while True:
    mission=input("미션 입력: ") # 게임이 종료될 때까지 계속 미션을 입력받는다.
    print(f"{separating_line}")
    if mission=="quit": # quit이 입력된 경우 게임을 종료한다.
        print(f"게임을 종료합니다!\n{separating_line}")
        gl=5
        print("다마고치가 죽었습니다.\n")
        print_Tamagotchi(gl)
        print(f"게임 종료\n{separating_line}")
        break
    elif mission=="help": # help가 입력된 경우 현재 성장 단계에 맞는 성장 미션을 표시한다.
        print(f"({gl_list[gl]}) 단계 성장 미션: {gm_list[gl]}\n{separating_line}")
    elif mission in dm_list: # 데일리 미션이 입력된 경우
        today_dm_status=daily_mission_update(mission, today_dm_status) # daily_mission_update을 통해 오늘의 데일리 미션 수행 여부를 업데이트한다.
        if is_daily_complete(today_dm_status): # N일차의 데일리 미션 3가지가 모두 완료된 경우
            today+=1 # N일차에서 (N+1)일차가 되고,
            today_dm_status=[0, 0, 0] # (N+1)일차의 데일리 미션 3가지는 모두 수행되지 않았으므로 today_dm_status를 초기화하고,
            current_gl_dm_status=1 # 현재 성장 단계에서의 데일리 미션 진행 여부는 진행됨으로 처리된다.
            print(f"다음 날이 되었습니다!\n{separating_line}")
            if today==8 and gl<4: # 만약 다마고치를 7일 안에 성체로 성장시키지 못하면 다마고치는 죽게 된다.
                gl=5
                print("다마고치가 죽었습니다.\n")
                print_Tamagotchi(gl)
                print(f"게임 종료\n{separating_line}")
                break
    elif mission in gm_list: # 성장 미션이 입력된 경우
        if is_growth_right(mission, gl, gm_list): # is_growth_right 함수로 입력 받은 성장 미션이 현재 성장 단계에 맞는 성장 미션인지 확인한다.
            current_gl_gm_status=growth_mission_update(mission, current_gl_gm_status) # 맞다면, growth_mission_update를 통해 성장 미션 수행 여부를 업데이트한다.
        else: # 맞지 않다면, 안내 문구를 출력한다.
            print(f"({mission})는 ({gl_list[gl]})단계에 맞지 않는 성장 미션입니다.\n{separating_line}")
    else: # 잘못된 입력의 경우 안내 문구를 출력한다.
        print(f"잘못된 입력입니다. 다시 입력하세요.\n{separating_line}")
    
    if growth_check(current_gl_dm_status, current_gl_gm_status): # 다마고치가 성장할 조건을 모두 만족했다면
        gl=next_growth_status(gl, gl_list) # next_growth_status로 다마고치를 성장시키고
        if gl==4: # 만약 성체로 성장했다면 게임을 종료한다.
            print(f"[{today}일차] - 성장단계: 성체\n")
            print_Tamagotchi(gl)
            print(f"게임종료\n{separating_line}")
            break
        current_gl_dm_status=0 # 성장하긴 헀지만 아직 성체가 아닌 경우
        current_gl_gm_status=0 # 현재 성장 단계의 데일리 미션과 성장 미션의 수행 여부를 초기화하고 다음 미션을 입력받을 준비를 한다.
    print_current_status(today, gl, today_dm_status, current_gl_dm_status, current_gl_gm_status, gl_list, XO) # 다음 미션을 입력받기 전에 상태 화면을 출력한다.