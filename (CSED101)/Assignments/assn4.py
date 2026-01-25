'''
2025학년도 1학기 <프로그래밍과 문제해결> Assignment #4
Problem: Flappy Bird Game
담당교수: 윤은영
담당조교: 유상현
학번: 20250205
학과: 무은재학부
이름: 김정우 Jungwoo Kim
POVIS ID: jkpxt14
명예서약(Honor code): 저는 이 프로그래밍 과제를 다른 사람의 부적절한 도움 없이 완수하였습니다.
'''

import tkinter as tk
import random

class GameObject: # 게임에서 사용하는 Object의 공통 부모 클래스로, Canvas에 그려지는 객체의 이동, 삭제, 위치 확인 기능을 제공
    def __init__(self, canvas):
        self.canvas=canvas # Canvas 객체를 전달받아 저장
        self.item=None # Canvas에 생성된 Object ID를 저장

    def move(self, dx, dy): # 현재 item을 (dx, dy)만큼 이동
        self.canvas.move(self.item, dx, dy)

    def delete(self): # Canvas에서 item을 삭제
        self.canvas.delete(self.item)

    def get_position(self): # item의 현재 좌표를 리스트로 반환
        return self.canvas.coords(self.item)


class Bird(GameObject): # 플레이어가 조작하는 Bird Class
    def __init__(self, canvas): # x=70, y=300 삭제
        super().__init__(canvas)
        self.speed=0

        self.bird_img=tk.PhotoImage(file="img/bird.png") # tk_img 대신 bird_img

        self.item=canvas.create_image(70, 300, image=self.bird_img) # Bird의 초기 위치를 (70, 300)로 설정

    def flap(self, event=None): # space 키 입력 시 호출
        self.speed=-10

    def gravity(self):
        self.speed+=1 # 중력 효과
        self.move(0, self.speed)
        return 0<self.get_position()[1]<590 # Bird가 천장과 바닥 사이에 있는지 그 여부를 반환, 이때 600 대신 590을 사용한 이유는 새가 땅에 떨어져서 화면에서 없어질 때 새가 화면에서 없어지는 경우를 최대한 방지하기 위함

    def get_bbox(self):
        return self.canvas.bbox(self.item) # 충돌 판정용으로 Bird 이미지의 bounding box 좌표 반환


class PipePart(GameObject):
    def __init__(self, canvas, x1, y1, x2, y2):
        super().__init__(canvas)
        self.item = self.canvas.create_rectangle(x1, y1, x2, y2, fill='green')

class Pipe:
    def __init__(self, canvas):
        self.canvas = canvas
        
        self.scored=False # 점수 지급 여부로, Pipe가 완전히 Bird를 지나친 경우 True로 변경

        top_height=random.randint(10, 40)*10 # 상단 Pipe의 높이를 100~400 사이의 값 중 10의 배수로 랜덤 선택
        gap=random.randint(120, 150) # 상단 Pipe와 하단 Pipe 사이의 간격을 120~150 사이의 값으로 랜덤 선택
        self.top=PipePart(self.canvas, 400, 0, 460, top_height) # 화면의 오른쪽 경계에 상단 Pipe 생성
        self.bottom=PipePart(self.canvas, 400, top_height+gap, 460, 600) # 화면의 오른쪽 경계에 하단 Pipe 생성

    def move(self): # 파이프를 왼쪽으로 이동
        self.top.move(-5, 0)
        self.bottom.move(-5, 0)
        return self.top.get_position()[2]>0 # 파이프가 화면 내에 있으면 True, 화면을 벗어나면 False를 반환

    def delete(self): # 파이프 쌍을 캔버스에서 제거
        self.top.delete()
        self.bottom.delete()

    def check_collision(self, bird_bbox): # Bird의 bounding box와 Pipe의 충돌 여부를 반환
        return (bird_bbox[0]<=self.top.get_position()[2] and bird_bbox[2]>=self.top.get_position()[0] and bird_bbox[1]<=self.top.get_position()[3] and bird_bbox[3]>=self.top.get_position()[1]) \
               or (bird_bbox[0]<=self.bottom.get_position()[2] and bird_bbox[2]>=self.bottom.get_position()[0] and bird_bbox[1]<=self.bottom.get_position()[3] and bird_bbox[3]>=self.bottom.get_position()[1]) # AABB 충돌 판정


class FlappyBirdGame: # 게임 전체를 관리하는 Class
    def __init__(self, root):
        self.root = root

        # self.WIDTH, self.HEIGHT, self.PIPE_SPACING 삭제

        self.canvas=tk.Canvas(root, width=400, height=600) # 전체 화면
        self.canvas.pack()

        self.bg_img=tk.PhotoImage(file="img/background.png")
        self.canvas.create_image(0, 0, anchor='nw', image=self.bg_img)

        self.bird = Bird(self.canvas)
        self.pipes = [Pipe(self.canvas)] # 게임 화면에 존재하는 Pipe 객체들을 저장하는 리스트

        self.running=True
        self.score=0

        self.score_text=self.canvas.create_text(200, 20, text=f"Score: {self.score}", font=('Helvetica', 20), fill='white') # 게임 점수 텍스트: "Score: 0" 형태로 상단 중앙에 표시

        self.root.bind("<space>", self.bird.flap) # 이벤트 바인딩(space 키와 flap() 연결) 설정

        self.game_loop()

    def game_loop(self):
        # 1) 게임 종료 상태 확인
        if not self.running:
            self.canvas.create_text(200, 300, text="Game Over", font=('Helvetica', 30), fill='red') # 게임 종료 시 "Game Over" 텍스트를 화면 중앙에 배치
            return # 게임 종료
        
        # 2) Bird의 기본 움직임
        if not self.bird.gravity():
            self.running=False # Bird과 화면을 벗어난 경우 게임 종료

        # 3) Pipe 이동 및 점수 체크 / 점수 텍스트 업데이트
        else:
            for i, pipe in enumerate(self.pipes):
                if not pipe.move(): # 화면 밖으로 이동하여 유효하지 않은 Pipe 객체는 리스트에서 제거하고 Canvas에서도 삭제
                    del self.pipes[i]
                    pipe.delete()
                else:
                    if not pipe.scored and self.bird.get_bbox()[0]>pipe.top.get_position()[2]:
                        self.score+=1 # Bird가 Pipe에 부딪히지 않고 통과하고 해당 Pipe가 아직 점수 부여에 사용되지 않은 경우 점수를 1점 추가
                        pipe.scored=True
                        self.canvas.itemconfig(self.score_text, text=f"Score: {self.score}") # 점수가 추가될 때 게임 점수 텍스트를 업데이트

        # 4) Bird와 Pipe의 충돌 검사
        for pipe in self.pipes:
            if pipe.check_collision(self.bird.get_bbox()):
                self.running=False # 충돌 시 게임 종료

        # 5) 새 Pipe 추가
        if len(self.pipes)<2 and self.pipes[-1].top.get_position()[0]<40:
            self.pipes.append(Pipe(self.canvas))
        
        # 6) 점수 텍스트가 다른 객체에 가리지 않도록 조정
        self.canvas.tag_raise(self.score_text)

        # 7) 게임 반복 실행
        self.root.after(30, self.game_loop)

if __name__ == "__main__":
    root = tk.Tk()
    root.title("Assignment4_20250205") # 프로그램 이름 설정
    game = FlappyBirdGame(root) # 게임 인스턴스 생성
    root.mainloop() # 이벤트 루프 시작