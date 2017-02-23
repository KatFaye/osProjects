from tkinter import *
from PIL import Image, ImageTk
import subprocess

if __name__ == "__main__":
    root = Tk()

    #setting up a tkinter canvas with scrollbars
    frame = Frame(root, width=550, height=550, bd=1)
    frame.pack()

    iframe5 = Frame(frame, bd=2, relief=SUNKEN)
    iframe5.pack(expand=1, fill=X, pady=10, padx=5)
    c = Canvas(iframe5, width=500, height=500)
    c.pack()

    #adding the image
    File = 'mandel2.bmp'
    img = ImageTk.PhotoImage(Image.open(File))
    myimg = c.create_image(0,0,image=img,anchor="nw")
    #function to be called when mouse is clicked
    def printcoords(event):
        #outputting x and y coords to console
        print (event.x,event.y)
        print (scale(event.x), -scale(event.y))

    #mouseclick event
    def scale(unscaled, center=0, scale=4):
        min = center - scale
        max = center + scale
        size = 500
        return (min + unscaled*(max-min)/size)

    def key(event):
        global img
        print("Pressed %c" % (event.char))
        subprocess.call(['C:\\Users\\Laptop\\AppData\\Local\\lxss\\rootfs\\bin\\bash'])
        myFile = 'mandel.bmp'
        img = ImageTk.PhotoImage(Image.open(myFile))
        c.delete("all")
        c.create_image(0,0,image=img,anchor="nw")
        c.pack()

    root.bind("<Key>", key)
    root.bind('<Escape>', lambda e: root.destroy())
    c.bind("<Button-1>",printcoords)
    c.pack()
    
    
    root.mainloop()
