import tkinter as tk

def move_left(event=None):
    canvas.move(bird, -10, 0)
    coordinate_update()

def move_right(event=None):
    canvas.move(bird, 10, 0)
    coordinate_update()

def move_up(event=None):
    canvas.move(bird, 0, -10)
    coordinate_update()

def move_down(event=None):
    canvas.move(bird, 0, 10)
    coordinate_update()

def coordinate_update(event=None):
    canvas.itemconfig(coordinate, text=f"({int(canvas.coords(bird)[0])}, {int(canvas.coords(bird)[1])})")

root=tk.Tk()

canvas=tk.Canvas(root, width=500, height=300, bg="skyblue")
canvas.pack()

img=tk.PhotoImage(file="Labs/Lab11/ponix.png")

bird=canvas.create_image(250, 150, image=img, anchor="center")

fontstyle=("Consolas", 15)
coordinate=canvas.create_text(250, 25, text="(250, 150)", fill="white", font=fontstyle)

root.bind("<Left>", move_left)
root.bind("<Right>", move_right)
root.bind("<Up>", move_up)
root.bind("<Down>", move_down)
root.bind("<Key>", coordinate_update)

root.mainloop()