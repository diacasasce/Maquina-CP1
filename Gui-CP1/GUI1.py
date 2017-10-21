import sys
from tkinter import *

Gui= Tk()
lab_color='#aaa';
Gui.geometry('640x445+100+00')
Gui.title('CP1')
titulo=Label(text='Interfaz de usuario ',font="Verdanda 25 bold").pack()
# Sección Para los indicadores de la maquina  #
la_indic=Label(text='Indicadores de la maquina',font="Verdanda 20 bold",padx='200',bg=lab_color).pack()

# Seccion para la informacion de las secuencias  #
la_seq=Label(text='Informacion de Secuencias',padx='90',pady=5,font="Verdanda 10 bold",bg=lab_color).place(y='200')

la_txt_ava=Label(text='Avance ',font="Verdanda 12",padx='50').place(y='250')
la_txt_vel=Label(text='Velocidad',font="Verdanda 12",padx='50').place(y='275')
la_txt_ace=Label(text='Aceleracion',font="Verdanda 12",padx='50').place(y='300')
la_txt_rep=Label(text='Repeticiones',font="Verdanda 12",padx='50').place(y='350')

#Seccion para la zona de contadores
la_cont=Label(text='Conteo',padx='50',bg=lab_color,font="Verdanda 20 bold").place(y='200',x='402')

la_con_uni=Label(text='Unidades ',font="Verdanda 12",padx='20').place(y='250',x='402')
la_con_cic=Label(text='Ciclos ',font="Verdanda 12",padx='20').place(y='350',x='402')
