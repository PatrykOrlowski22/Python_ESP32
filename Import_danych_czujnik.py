# -*- coding: utf-8 -*-
"""
Created on Tue Nov 22 15:30:18 2022

@author: Patryk
"""

import tkinter
import Import_strony as odczyt

window = tkinter.Tk()

window.geometry("500x500")
window.title("Odczyt czujników")
window.configure(background = "grey")

br_with = 3

def Rose_temp():
    global whitelabel 
    temp = float(odczyt.download()['temp'])
    hum = float(odczyt.download()['hum'])
    rose_t = float((hum/100.0)**(1/8)*(112.0+(0.9*temp))+(0.1*temp)-112.0)
    whitelabel = tkinter.Label(window,text = "Rose Temperature: " + str(format(rose_t,".2f"))) 
    whitelabel.grid(row=5,column=1,padx=10,pady=10)

def Click_temp():
    global whitelabel 
    temp =  odczyt.download()['temp']
    whitelabel = tkinter.Label(window,text = "Temperature: " + temp) 
    whitelabel.grid(row=0,column=1,padx=10,pady=10)
def Click_hum():
    global whitelabel 
    hum =  odczyt.download()['hum']
    whitelabel = tkinter.Label(window,text = "Humidity: " + hum) 
    whitelabel.grid(row=1,column=1,padx=10,pady=10)
def Click_pres():
    global whitelabel 
    pres =  odczyt.download()['pres']
    whitelabel = tkinter.Label(window,text = "Pressure: " + pres) 
    whitelabel.grid(row=2,column=1,padx=10,pady=10)
def Click_gas():
    global whitelabel 
    gas =  odczyt.download()['gas']
    whitelabel = tkinter.Label(window,text = "Gas: " + gas) 
    whitelabel.grid(row=3,column=1,padx=10,pady=10)
def Click_sea():
    global whitelabel 
    sealvl =  odczyt.download()['sealvl']
    whitelabel = tkinter.Label(window,text = "Sealevel: " + sealvl) 
    whitelabel.grid(row=4,column=1,padx=10,pady=10)
    
temp_key = tkinter.Button(window, text = "Temperature", fg = "blue", 
         borderwidth = br_with, width = 12,  bg = "white", command = Click_temp)
temp_key.grid(row=0,column=0,padx=10,pady=10)

hum_key = tkinter.Button(window, text = "Humidity", fg = "blue",
          borderwidth = br_with, width = 12, bg = "white", command = Click_hum)
hum_key.grid(row=1,column=0,padx=10,pady=10)

press_key = tkinter.Button(window, text = "Pressure", fg = "blue",
          borderwidth = br_with, width = 12, bg = "white", command = Click_pres)
press_key.grid(row=2,column=0,padx=10,pady=10)

gas_key = tkinter.Button(window, text = "Gas VCO", fg = "blue",
           borderwidth = br_with, width = 12, bg = "white", command = Click_gas)
gas_key.grid(row=3,column=0,padx=10,pady=10)

sealevel_key = tkinter.Button(window, text = "Sealevel", fg = "blue",
           borderwidth = br_with, width = 12, bg = "white", command = Click_sea)
sealevel_key.grid(row=4,column=0,padx=10,pady=10)

delete_key = tkinter.Button(window, text = "Rose Temp", fg = "blue",
           borderwidth = br_with, width = 12, bg = "white", command = Rose_temp)
delete_key.grid(row=5,column=0,padx=10,pady=10)

window.mainloop()

print(odczyt.download())