#!/usr/bin/env python3

from tkinter import *
from PIL import Image, ImageTk
import subprocess
from math import *

if __name__ == "__main__":
    root = Tk()
    centerx = 0
    centery = 0
    zoomlevel = 0
    s = 4

    #setting up a tkinter canvas with scrollbars
    frame = Frame(root, width=550, height=550, bd=1)
    frame.pack()

    usage = Label(root, text="Press '+' to zoom in; '-' to zoom out.\nClick on a location on the map to re-center and zoom.\n")
    usage.pack()

    iframe5 = Frame(frame, bd=2, relief=SUNKEN)
    iframe5.pack(expand=1, fill=X, pady=10, padx=5)
    c = Canvas(iframe5, width=500, height=500)
    c.pack()

    #adding the image
    subprocess.call('./mandel') #default image create
    File = 'mandel.bmp'
    img = ImageTk.PhotoImage(Image.open(File))
    myimg = c.create_image(0,0,image=img,anchor="nw")
    #function to be called when mouse is clicked
    def zoomAndCenter(event):
        #outputting x and y coords to console
        #print (event.x,event.y)
        global centerx, centery, s, zoomlevel
        if (zoomlevel < 50): #smooth zoom
            zoomlevel = zoomlevel + 1
            s = 4 * (exp(log(.000001/4)/(50))) ** zoomlevel
        else:
            s = s/2
        centerx =scale(event.x, s)
        centery = -scale(event.y, s)

        cmd = ['./mandel']
        args = ' -s ' + str(s) + ' -x ' + str(centerx) + ' -y ' + str(centery)
        if (zoomlevel > 10):
            args = args + '-m 5000'
        cmd = cmd + args.split()
        subprocess.call(cmd)
        update()

    #mouseclick event
    def scale(unscaled, s=4, center=0):
        min = center - s
        max = center + s
        size = 500
        return (min + unscaled*(max-min)/size)

    def update():
        global img
        myFile = 'mandel.bmp'
        img = ImageTk.PhotoImage(Image.open(myFile))
        c.delete("all")
        c.create_image(0,0,image=img,anchor="nw")
        c.pack()

    def key(event):
        global s, centerx, centery, zoomlevel
        cmd = ['./mandel']
        if(event.char == '+'):
            if (zoomlevel < 50): #smooth zoom
                zoomlevel = zoomlevel + 1
                s = 4 * (exp(log(.000001/4)/(50))) ** zoomlevel    
            else:
                s = s/2
        elif(event.char == '-'):
            if (zoomlevel > 0): #smooth zoom
                zoomlevel = zoomlevel - 1
                s = 4 * (exp(log(.000001/4)/(50))) ** zoomlevel
            else:
                s = s/2
        args = ' -s ' + str(s) + ' -x ' + str(centerx) + ' -y ' + str(centery)
        if (zoomlevel > 10):
            args = args + ' -m 5000'
        cmd = cmd + args.split()
        subprocess.call(cmd)
        update()
    
    def reset(event):
        subprocess.call('./mandel')
        update()

    root.bind("<Key>", key)
    root.bind('<Escape>', lambda e: root.destroy())
    root.bind('<Return>', reset)
    c.bind("<Button-1>",zoomAndCenter)
    c.pack()
    
    
    root.mainloop()
