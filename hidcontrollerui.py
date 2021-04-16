from tkinter import *
from tkinter import colorchooser
import math
from functools import partial
import os

pathtocontroller = "hidcontroller\\hidcontroller.exe"

root = Tk()
root.title("RGB HID bm16s")

buttons = {}
colors = {}
removing = 0

def get_color(num):
    if(removing == 0):
        if(colors.__contains__(num)):
            color = colorchooser.askcolor(color=colors[num][1])
        else:
            color = colorchooser.askcolor()
        if(color[0] != None):
            colors[num] = color
            buttons[num].configure(bg=colors[num][1])
    else:
        buttons[num].configure(bg='SystemButtonFace')
        if(colors.__contains__(num)):
            colors.pop(num)
    print(colors)
5
def reset():
    global colors
    colors = {}
    for button in buttons:
        buttons[button].configure(bg='SystemButtonFace')

def send():
    os.system(pathtocontroller + " " + get_color_string())

def copy():
    root.clipboard_clear()
    root.clipboard_append(get_color_string())


def remove():
    global removing
    if(removing == 0):
        removebutton.configure(bg='red')
        removing = 1
    else:
        removebutton.configure(bg='SystemButtonFace')
        removing = 0

def get_color_string():
    out = ""
    for color in colors:
        out = out + " " + "{}".format(color) + " " + "{}".format(colors[color][0][0]) + " " + "{}".format(colors[color][0][1]) + " " + "{}".format(colors[color][0][2])
    return out


buttonframe = Frame(root, bd=20)
buttonframe.grid(column=1,row=1)
row = 1
for i in range(1,17):
    col = (i - 1) % 4 
    buttons[i] = Button(buttonframe, text=i, command=partial(get_color,i), height=4, width=10, bd=5)
    buttons[i].grid(column=col,row=row)
    if(i % 4 == 0):
        row+=1

optionframe = Frame(root, bd=20)
optionframe.grid(column=1,row=2)
restbutton = Button(optionframe, text="RESET", command=reset, height=2, width=10, bd=5)
restbutton.grid(column=1,row=1)
removebutton = Button(optionframe, text="REMOVE", command=remove, height=2, width=10, bd=5)
removebutton.grid(column=2,row=1)
sendbutton = Button(optionframe, text="SEND", command=send, height=2, width=10, bd=5)
sendbutton.grid(column=3,row=1)
copybutton = Button(optionframe, text="COPY", command=copy, height=2, width=10, bd=5)
copybutton.grid(column=4,row=1)
root.resizable(False, False)
root.mainloop()