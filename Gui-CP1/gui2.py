#!/usr/bin/python3
# -*- coding: utf-8 -*-
from tkinter import *
import tkinter.ttk as ttk
from serCOM import *
self = Tk()
#self.attributes("-fullscreen", True)
self.geometry("640x445+100+00")
self.title("Cortadora de Palitos")
titulo=Label(text='Interfaz de usuario ',font="Verdanda 25 bold",anchor=CENTER).pack(fill=X)
indic = Frame(self, relief=RAISED, borderwidth=1)
indic.pack(fill=BOTH, expand=True,)
titulo=Label(indic,text='Indicadores de la maquina',font="Verdanda 20 bold",anchor=CENTER).pack(fill=X)
conect = Frame(indic, relief=RAISED, borderwidth=1)
conect.pack(fill=BOTH,side=LEFT)
MotA = Frame(conect, relief=RAISED, borderwidth=1)
MotA.pack(fill=BOTH, expand=True)
Lma=Label(MotA,text="Motor A").pack(fill=BOTH)
MotB = Frame(conect, relief=RAISED, borderwidth=1)
MotB.pack(fill=BOTH, expand=True)
Lmb=Label(MotB,text="Motor B").pack(fill=BOTH)
def conA():
    (SerA,dA,SerB,dB)=conectar()
    conectado(dA,dB)
bmA = Button(conect, text="Conectar", bg="green", command=conA).pack(fill=BOTH)


manual = Frame(indic, relief=RAISED, borderwidth=1)
manual.pack(fill=BOTH, expand=True)
def avance():
    print ("click! a") 
B_av = Button(manual, text=" Avance >", command=avance).pack(fill=Y, side=RIGHT)
def retro():
    print ("click! r") 
B_av = Button(manual, text="< Regreso", command=retro).pack(fill=Y, side=RIGHT)
def corte():
    cor="1"
#    print ("click! c")
    if dA:
        print("cortando")
        stA=send_ser("?",SerA)
        if stA[0]==50:
            send_nr(cor,SerA)
B_av = Button(manual, text=" Corte", command=corte).pack(fill=Y, side=RIGHT)


data = Frame(self, relief=RAISED, borderwidth=1)
data.pack(fill=BOTH, expand=True)

param = Frame(data, relief=RAISED, borderwidth=1)
param.pack(fill=BOTH, expand=True,side=LEFT)
ti_par=Label(param,text='Parametros de secuencia ',font="Verdanda 15 bold").pack(fill=X)

sec = Frame(param, relief=RAISED, borderwidth=1)
sec.pack(fill=BOTH, expand=True)
choices = { 'Bloque 5kg','Bloque 2.5kg','Personalizado',''}
tkvar = StringVar(self)
tkvar.set('Bloque 5kg')
popupMenu = OptionMenu(sec, tkvar,*choices).pack(fill=X,side=RIGHT)
def change_dropdown(*args):
    vars=secuen[val_com[tkvar.get()]]
    print(vars)
    avan.set(vars[0])
    velo.set(vars[1])
    acel.set(vars[2])
    repe.set(vars[3])
    
tkvar.trace('w', change_dropdown)
val_com = {'Bloque 5kg': 1, 'Bloque 2.5kg': 2,'Personalizado':0}
secuen=((50,70,20,0),(60,70,20,1),(30,70,30,2))

la_txt_sec=Label(sec,text='Seleccione una Secuencia ',font="Verdanda 10").pack(fill=BOTH,side=LEFT)


ava = Frame(param, relief=RAISED, borderwidth=1)
ava.pack(fill=BOTH, expand=True)
la_txt_ava=Label(ava,text='Avance ',font="Verdanda 12").pack(side=LEFT)
avan = IntVar(self)
avan.set(0);
def Rav():
    avan.set(avan.get()-1)
    print(avan.get())
r_av = Button(ava, text="-", command=Rav).pack(fill=Y, side=RIGHT)

def Sav():
    avan.set(avan.get()+1)
    print(avan.get())
    
s_av = Button(ava, text="+", command=Sav).pack(fill=Y, side=RIGHT)
val_av=Label(ava,textvariable=avan,font="Verdada 10").pack(side=RIGHT)

vel = Frame(param, relief=RAISED, borderwidth=1)
vel.pack(fill=BOTH, expand=True)
la_txt_vel=Label(vel,text='Velocidad',font="Verdanda 12").pack(fill=BOTH,side=LEFT)
velo = IntVar(self)
velo.set(0);
def Rve():
    velo.set(velo.get()-1)
    print(velo.get())
r_ve = Button(vel, text="-", command=Rve).pack(fill=Y, side=RIGHT)

def Sve():
    velo.set(velo.get()+1)
    print(velo.get())
s_ve = Button(vel, text="+", command=Sve).pack(fill=Y, side=RIGHT)
val_ve=Label(vel,textvariable=velo,font="Verdada 10").pack(side=RIGHT) 

ace = Frame(param, relief=RAISED, borderwidth=1)
ace.pack(fill=BOTH, expand=True)
la_txt_ace=Label(ace,text='Aceleracion',font="Verdanda 12").pack(fill=BOTH,side=LEFT)
acel = IntVar(self)
acel.set(0);
def Rac():
    acel.set(acel.get()-1)
    print(acel.get())
r_ac = Button(ace, text="-", command=Rac).pack(fill=Y, side=RIGHT)

def Sac():
    acel.set(acel.get()+1)
    print(acel.get())
s_ac = Button(ace, text="+", command=Sac).pack(fill=Y, side=RIGHT)
val_ac=Label(ace,textvariable=acel,font="Verdada 10").pack(side=RIGHT)

rep = Frame(param, relief=RAISED, borderwidth=1)
rep.pack(fill=BOTH, expand=True)
la_txt_rep=Label(rep,text='Repeticiones',font="Verdanda 12").pack(fill=BOTH,side=LEFT)
repe = IntVar(self)
repe.set(0);
def Rre():
    repe.set(repe.get()-1)
    print(repe.get())
r_re = Button(rep, text="-", command=Rre).pack(fill=Y, side=RIGHT)

def Sre():
    repe.set(repe.get()+1)
    print(repe.get())
s_re = Button(rep, text="+", command=Sre).pack(fill=Y, side=RIGHT)
val_re=Label(rep,textvariable=repe,font="Verdada 10").pack(side=RIGHT)


conteo = Frame(data, relief=RAISED, borderwidth=1)
conteo.pack(fill=BOTH, expand=True,side=LEFT)
ti_con=Label(conteo,text='Conteo ',font="Verdanda 15 bold").pack(fill=X, side=TOP)
cont = Frame(conteo, relief=RAISED, borderwidth=1)
cont.pack(fill=BOTH, expand=True)
unid = Frame(cont, relief=RAISED, borderwidth=1)
unid.pack(fill=BOTH, expand=True)
V_uni=IntVar(self)
V_uni.set(0)
val_un=Label(unid,text='Unidades:',font="Verdada 10").pack(side=LEFT)
l_uni=Label(unid,textvariable=V_uni,font="Verdada 10").pack(side=RIGHT)
cicl = Frame(cont, relief=RAISED, borderwidth=1)
cicl.pack(fill=BOTH, expand=True)
V_cic=IntVar(self)
V_cic.set(0)
val_ci=Label(cicl,text='Ciclos:',font="Verdada 10").pack(side=LEFT)
l_cic=Label(cicl,textvariable=V_uni,font="Verdada 10").pack(side=RIGHT)
cbut = Frame(cont, relief=RAISED, borderwidth=1)
cbut.pack(fill=BOTH, expand=True)
def go():
    print('go')
    instruccion="%4.2f;%4.2f;%4.2f " %(avan.get(),velo.get(),acel.get())
    print(instruccion)
    if dB:
        stA=send_ser("?",SerB)
        if stA[0]==50:
            send_nr(instruccion,SerB)
B_go = Button(cbut, text="INICIAR", command=go).pack(fill=BOTH, expand=True)

def stop():
    print('stop')
B_st = Button(cbut, text="DETENER", command=stop).pack(fill=BOTH, expand=True)

def conectado(da,db):
    if da:
        MotA.config(bg="green")
    else:
        MotA.config(bg="red")
    if db:
        MotB.config(bg="green")
    else:
        MotB.config(bg="red")
(SerA,dA,SerB,dB)=conectar()
conectado(dA,dB)
change_dropdown()
print(dA)
print(dB)
