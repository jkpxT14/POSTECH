'''
2025학년도 1학기 <프로그래밍과 문제해결> Assignment #2
Problem: 빙고 게임
담당교수: 윤은영
담당조교: 이동엽
학번: 20250205
학과: 무은재학부
이름: 김정우 Jungwoo Kim
POVIS ID: jkpxt14
명예서약(Honor code): 저는 이 프로그래밍 과제를 다른 사람의 부적절한 도움 없이 완수하였습니다.
'''

'''
[Importing modules]
Modules(2):
    random
    time
'''
import random
import time

'''
[Defining functions]
Functions(8):
    Required functions(7):
        create_random_board: 1(Random) mode가 선택되었을 때 랜덤하게 플레이어와 컴퓨터의 빙고판을 생성해주는 함수
        create_board_from_file: 2(File) mode가 선택되었을 때 파일에서 플레이어와 컴퓨터의 빙고판을 불러오는 함수
        boards_to_string: 현재 게임 상태를 문자열로 만들어 반환하는 함수
        check_bingo: 주어진 빙고판에 완성된 빙고가 있는지 판단하는 함수
        get_player_choice: 플레이어의 선택을 return하는 함수
        get_computer_choice: 컴퓨터의 선택을 return하는 함수
        mark_board: 선택된 문자를 빙고판에서 #으로 표시하는 함수
    Additional function(1):
        is_game_over: 게임이 종료되었는지 확인하고, 종료되었다면 게임 결과를 출력하는 함수
'''
def create_random_board(): # 필수 사용자 정의 함수(Required function)
    '''
    1(Random) mode가 선택되었을 때 랜덤하게 플레이어와 컴퓨터의 빙고판을 생성해주는 함수
    주어진 크기(board_size)에 맞는 랜덤한 빙고판(2차원 리스트)을 생성하여 반환
        
    Variables(4):
        Global(4):
            alphabet_used
            board_size
            player_board
            computer_board
                   
    Returns(2):
        player_board
        computer_board
    '''
    random.shuffle(alphabet_used) # 랜덤하게 플레이어의 빙고판을 생성하기 위해 빙고판 생성에 사용되는 알파벳이 담긴 리스트를 shuffle하여 활용
    for i in range(board_size):
        for j in range(board_size):
            player_board[i][j]=alphabet_used[i*board_size+j] # alphabet_used 리스트에 shuffle되어 들어있는 (board_size)^2 개의 알파벳을 (board_size) x (board_size)의 2차원 리스트인 player_board에 순서대로 담는다
    random.shuffle(alphabet_used) # 랜덤하게 컴퓨터의 빙고판을 생성하기 위해 빙고판 생성에 사용되는 알파벳이 담긴 리스트를 shuffle하여 활용
    for i in range(board_size):
        for j in range(board_size):
            computer_board[i][j]=alphabet_used[i*board_size+j] # alphabet_used 리스트에 shuffle되어 들어있는 (board_size)^2 개의 알파벳을 (board_size) x (board_size)의 2차원 리스트인 computer_board에 순서대로 담는다
    return player_board, computer_board

def create_board_from_file(): # 필수 사용자 정의 함수(Required function)
    '''
    2(File) mode가 선택되었을 때 파일에서 플레이어와 컴퓨터의 빙고판을 불러오는 함수
    board.txt 파일에서 데이터를 읽어 와 빙고판(2차원 리스트)을 생성하여 반환
        
    Variables(4):
        Global(3):
            board_size
            player_board
            computer_board
        Local(1):
            line: board.txt에서 한 줄씩 가져와 list 형태로 line에 담음
                   
    Returns(2):
        player_board
        computer_board
    '''
    for i in range(board_size):
        line=list(f.readline())
        for j in range(board_size):
            player_board[i][j]=line[2*j] # 공백과 줄바꿈을 고려하면 line[2*j]에 알파벳이 들어 있음
    f.readline()
    for i in range(board_size):
        line=list(f.readline())
        for j in range(board_size):
            computer_board[i][j]=line[2*j] # 공백과 줄바꿈을 고려하면 line[2*j]에 알파벳이 들어 있음
    return player_board, computer_board

def boards_to_string(hide_computer): # 필수 사용자 정의 함수(Required function)
    '''
    현재 게임 상태를 문자열로 만들어 반환하는 함수

    Parameter(1):
        hide_computer(True/False): True인 경우 컴퓨터 빙고판 내 글자를 숨기고 False인 경우 컴퓨터 빙고판 내 글자를 숨기지 않는다.
        
    Variables(5):
        Global(3):
            board_size
            player_board
            computer_board
        Local(2):
            boards
            separating_line
                   
    Return(1):
        현재 게임 상태롤 문자열로 만들어 반환한다.
    '''
    boards=""
    separating_line=f"+{"---+"*board_size}    +{"---+"*board_size}\n"
    boards+=f"[Player]{" "*(len(f"+{"---+"*board_size}    ")-len("[Player]"))}[Computer]\n"
    if hide_computer:
        boards+=separating_line
        for i in range(board_size):
            boards+="|"
            for j in range(board_size):
                boards+=f" {player_board[i][j]} |"
            boards+="    |"
            for j in range(board_size):
                if computer_board[i][j]=="#":
                    boards+=" # |"
                else:
                    boards+="   |"
            boards+=f"\n{separating_line}"
    else: # not hide_computer
        boards+=separating_line
        for i in range(board_size):
            boards+="|"
            for j in range(board_size):
                boards+=f" {player_board[i][j]} |"
            boards+="    |"
            for j in range(board_size):
                boards+=f" {computer_board[i][j]} |"
            boards+=f"\n{separating_line}"

    return boards

def check_bingo(player_or_computer): # 필수 사용자 정의 함수(Required function)
    '''
    주어진 빙고판에 완성된 빙고가 있는지 판단하는 함수
    전달받은 보드판에서 가로, 세로, 대각선으로 빙고가 존재하면 True, 없으면 False 반환

    Parameter(1):
        player_or_comptuer(1 or 2): 1이라면 플레이어의 빙고판, 2라면 컴퓨터의 빙고판에 완성된 빙고가 있는지 판단한다.
        
    Variables(5):
        Global(3):
            player_board
            computer_board
            board_size
        Local(2):
            board(player_board or computer_board)
            check(True/False)
                   
    Return:
        True/False: 주어진 빙고판에 완성된 빙고판이 있다면 True를, 없다면 False를 return한다.
    '''
    if player_or_computer==1:
        board=player_board
    else: # player_or_computer==2
        board=computer_board

    # 가로로 빙고가 존재하는지 확인
    for i in range(board_size):
        check=True
        for j in range(board_size):
            if board[i][j]!="#":
                check=False
        if check:
            return True
        
    # 세로로 빙고가 존재하는지 확인
    for i in range(board_size):
        check=True
        for j in range(board_size):
            if board[j][i]!="#":
                check=False
        if check:
            return True
        
    # 대각선으로 빙고가 존재하는지 확인
    check=True
    for i in range(board_size):
        if board[i][i]!="#":
            check=False
    if check:
        return True
    
    check=True
    for i in range(board_size):
        if board[board_size-1-i][i]!="#":
            check=False
    if check:
        return True
    
    return False

def get_player_choice(): # 필수 사용자 정의 함수(Required function)
    '''
    플레이어의 선택을 return하는 함수
    플레이어의 입력을 받아 해당 문자가 유효한지 체크하여 유효한 경우 해당 문자 반환
        
    Variables(3):
        Global(2):
            alphabet_used
            alphabet_used_used_set
        Local(1):
            player_choice
                   
    Return:
        player_choice: 플레이어의 선택을 return한다.
    '''
    while True:
        player_choice=input("Your choice: ")
        if (player_choice not in alphabet_used) or (player_choice in alphabet_used_used_set): # 현재 빙고판에서 사용할 수 없는 알파벳이 입력되거나 이미 선택된 알파벳이 입력되거나 다른 문자나 형식이 입력된 경우
            print("Invalid input. Try again.") # 에러 메세지를 출력
            continue
        else:
            print("")
            return player_choice

def get_computer_choice(): # 필수 사용자 정의 함수(Required function)
    '''
    컴퓨터의 선택을 return하는 함수
    빙고판에서 아직 선택되지 않은 문자를 찾아 그 중에서 하나를 랜덤하게 선택하여 반환
        
    Variables(3):
        Global(2):
            alphabet_used
            alphabet_used_used_set
        Local(1):
            alphabet_used_set
                   
    Return(1):
        컴퓨터의 선택을 return한다.
    '''
    alphabet_used_set=set(alphabet_used) # 차집합 연산을 수행하기 위해 alphabet_used를 set화 한 alphabet_used_set을 생성
    return random.choice(list(alphabet_used_set-alphabet_used_used_set)) # 차집합 연산을 수행하고 마지막에는 random.choice()를 활용하기 위해 list화

def mark_board(choice): # 필수 사용자 정의 함수(Required function)
    '''
    선택된 문자를 빙고판에서 #으로 표시하는 함수
    보드판에서 문자(choice)를 "#"로 표시

    Parameter(1):
        choice
        
    Variables(4):
        Global(4):
            board_size
            player_board
            computer_board
            alphabet_used_used_set
    '''
    for i in range(board_size):
        for j in range(board_size):
            if player_board[i][j]==choice:
                player_board[i][j]="#"
            if computer_board[i][j]==choice:
                computer_board[i][j]="#"
    alphabet_used_used_set.add(choice) # 선택된 알파벳을 alphabet_used_used_set의 원소로 추가
    return

def is_game_over(): # 추가 사용자 정의 함수(Additional function)
    '''
    게임이 종료되었는지 확인하고, 종료되었다면 게임 결과를 출력하는 함수
    
    Print(1):
        Draw!/You Win!/Computer Wins!
                   
    Return(1):
        True/False: 게임이 종료되었다면 True를, 종료되지 않았다면 False를 return한다.
    '''
    if check_bingo(1) and check_bingo(2): # 동시에 빙고를 완성한 경우
        print("Draw!\n")
        f.write("Draw!\n") # result.txt 파일에 게임 결과 기록
        return True
    elif check_bingo(1): # 플레이어가 먼저 빙고를 완성한 경우
        print("You Win!\n")
        f.write("You Win!\n") # result.txt 파일에 게임 결과 기록
        return True
    elif check_bingo(2): # 컴퓨터가 먼저 빙고를 완성한 경우
        print("Computer Wins!\n")
        f.write("Computer Wins!\n") # result.txt 파일에 게임 결과 기록
        return True
    else: # 아직 아무도 빙고를 완성하지 못한 경우
        return False

'''
[Defining variables]
Variables(9):
    alphabet: 모든 알파벳을 담아둔 리스트
    mode(1 or 2): 빙고판을 생성하는 방법
    board_size(3 or 4 or 5): 빙고판의 크기
    alphabet_used: 빙고판 크기에 맞는, 빙고 게임에서 사용 가능한 알파벳만을 포함하는 리스트 Ex. 빙고판 크기가 3x3이라면 A부터 I까지 포함하는 리스트
    alphabet_used_used_set: 빙고판 크기에 맞는, 빙고 게임에서 사용 가능한 알파벳 중에서도 빙고 게임에서 이미 사용된 알파벳만을 포함하는 집합
    player_board: 플레이어의 빙고판, 2차원 리스트로 생성 후 사용
    computer_board: 컴퓨터의 빙고판, 2차원 리스트로 생성 후 사용
    player_choice: 플레이어가 선택한 알파벳
    computer_choice: 컴퓨터가 선택한 알파벳
'''
alphabet=["A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z"]
while True: # 올바른 값이 입력될 때까지 반복
    mode=input("Select mode (1: Random, 2: File): ")
    if mode=="1" or mode=="2":
        mode=int(mode)
        print("")
        break
    else: # 1(Random), 2(File) 외의 입력에 대해서는
        print("Invalid input. Enter 1 or 2.") # 에러 메세지 출력
        continue

if mode==1:
    while True:
        board_size=input("Enter board size (3~5): ")
        if board_size=="3" or board_size=="4" or board_size=="5":
            board_size=int(board_size)
            print("")
            break
        else: # 3(x3), 4(x4), 5(x5) 이외의 입력에 대해서는
            print("Invalid input. Enter 3 to 5.") # 에러 메세지 출력
            continue

    alphabet_used=alphabet[:board_size**2] # 빙고판 크기에 맞는, 빙고 게임에서 사용 가능한 알파벳만을 포함하는 리스트 Ex. 빙고판 크기가 3x3이라면 A부터 I까지 포함하는 리스트
    alphabet_used_used_set=set() # 빙고판 크기에 맞는, 빙고 게임에서 사용될 알파벳 중에서도 진행되고 있는 빙고 게임에서 이미 사용된 알파벳만을 포함하는 집합
    player_board=[["?" for i in range(board_size)] for j in range(board_size)] #  일단 모든 칸을 ?로 채워 플레이어의 빙고판을 2차원 리스트로 생성
    computer_board=[["?" for i in range(board_size)] for j in range(board_size)] # 일단 모든 칸을 ?로 채워 컴퓨터의 빙고판을 2차원 리스트로 생성
    player_board, computer_board = create_random_board() # 플레이어와 컴퓨터의 빙고판을 랜덤하게 생성

else: # mode==2
    f=open("C:/Users/jkpxt/Desktop/POSTECH/1-1/Programming & Problem solving/Assignments/Assignment 2/board.txt", "r") # 빙고판을 만들기 위해 board.txt 파일을 open
    board_size=int(f.readline())
    alphabet_used=alphabet[:board_size**2] # 빙고판 크기에 맞는, 빙고 게임에서 사용 가능한 알파벳만을 포함하는 리스트 Ex. 빙고판 크기가 3x3이라면 A부터 I까지 포함하는 리스트
    alphabet_used_used_set=set() # 빙고판 크기에 맞는, 빙고 게임에서 사용될 알파벳 중에서도 진행되고 있는 빙고 게임에서 이미 사용된 알파벳만을 포함하는 집합
    player_board=[["?" for i in range(board_size)] for j in range(board_size)] # 일단 모든 칸을 ?로 채워 플레이어의 빙고판을 2차원 리스트로 생성
    computer_board=[["?" for i in range(board_size)] for j in range(board_size)] # 일단 모든 칸을 ?로 채워 컴퓨터의 빙고판을 2차원 리스트로 생성
    player_board, computer_board = create_board_from_file() # 플레이와 컴퓨터의 빙고판을 파일에서 불러옴
    f.close() # 빙고판을 다 만든 후 board.txt 파일을 close

f=open("C:/Users/jkpxt/Desktop/POSTECH/1-1/Programming & Problem solving/Assignments/Assignment 2/result.txt", "w") # 게임을 기록할 result.txt 파일을 open
print(boards_to_string(True)) # 생성된 빙고판이 출력되며 게임 시작
f.write(boards_to_string(False)) # result.txt 파일에 게임 기록
f.write("\n") # result.txt 파일에 게임 기록
while True:
    player_choice=get_player_choice() # 플레이어가 먼저 알파벳을 입력하여 게임 시작
    f.write(f"Player's choice: {player_choice}\n") # result.txt 파일에 게임 기록
    f.write("\n") # result.txt 파일에 게임 기록
    mark_board(player_choice) # 플레이어의 선택을 빙고판에 기록
    print(boards_to_string(True)) # 현재 게임 상태 출력
    f.write(boards_to_string(False)) # result.txt 파일에 게임 기록
    f.write("\n") # result.txt 파일에 게임 기록
    if is_game_over(): # 빙고가 하나라도 완성이 되면 게임 종료
        break # 게임 종료
    time.sleep(0.5) # 0.5초의 지연 후
    computer_choice=get_computer_choice() # 컴퓨터가 자신의 순서에서 알파벳을 선택
    print(f"Computer's choice: {computer_choice}\n")
    f.write(f"Computer's choice: {computer_choice}\n") # result.txt 파일에 게임 기록
    f.write("\n") # result.txt 파일에 게임 기록
    mark_board(computer_choice) # 컴퓨터의 선택을 빙고판에 기록
    print(boards_to_string(True)) # 현재 게임 상태 출력
    f.write(boards_to_string(False)) # result.txt 파일에 게임 기록
    f.write("\n") # result.txt 파일에 게임 기록
    if is_game_over(): # 빙고가 하나라도 완성이 되면 게임 종료
        break # 게임 종료

f.close() # 게임이 끝까지 기록된 result.txt 파일을 close