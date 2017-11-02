import sys
import threading
from PyQt4 import QtGui,QtCore
from serCOM import *
from time import sleep

class Window(QtGui.QMainWindow):
    def __init__(self):
        super(Window,self).__init__()
        self.busy=False
        self.h=480
        self.w=800
        self.av=0
        self.ve=70
        self.ac=1
        self.re=0
        self.uc=0
        self.on=0
        self.ci=0
        self.un=0
        self.dA=False
        self.dB=False
        self.val_com = {'Bloque 5kg': (560,70,20,1,5),'Bloque 500g':(56,70,8,10,4), 'Bloque 2.5kg': (280,70,20,2,5),'Personalizado':(30,70,30,2,5)}
        self.ite_com=('Bloque 5kg','Bloque 2.5kg','Bloque 500g', 'Personalizado')
        self.selected=''
##      ---   informacion de la maquina
        lb1=self.label('Info maquina',size=(self.w,30))
##        conexion serial
        fr1=self.frame(size=(150,135) ,pos=(10,45))
        self.fMa=self.frame(size=(140,45) ,pos=(15,50))
        lb2=self.label('Motor A',pos=(20,50))
        self.lMa=self.label(' -- ',pos=(20,70))
        self.fMb=self.frame(size=(140,45) ,pos=(15,100))
        lb3=self.label('Motor B',pos=(20,100))
        self.lMb=self.label(' -- ',pos=(20,120))
        btt1=self.btn('Conectar',self.conecta,pos=(20,145))
        lp1=self.label('Posicion: ',pos=(200,70))
        self.lps=self.label('--',pos=(300,70),frame='White')
        bttP=self.btn(' leer Posicion',self.gp,pos=(200,120))
        bttP=self.btn('Volver al inicio',self.bk,pos=(350,120))
##        botones de avance manual
        fr1=self.frame(size=(200,135) ,pos=(555,45))
        btt2=self.btn('atras',self.bt3,size=(70,100),pos=(560,63))
        btt3=self.btn('adelante',self.bt2,size=(70,100),pos=(630,63))
        btt4=self.btn('corte',self.bt4,size=(50,100),pos=(700,63))
##        Informacion de recorridos
        lb4=self.label('Info Recorridos',size=(self.w,30),pos=(0,180),frame='White')
##        info secuencias
        fr1=self.frame(size=(480,250) ,pos=(10,210))
        lb5=self.label('Secuencias',size=(480,30),pos=(20,215))
        fr1=self.frame(size=(470,40) ,pos=(15,245))
        lb6=self.label('Seleccione una ',size=(200,30),pos=(20,250))
        cmb=self.combo(self.ite_com,size=(200,30),pos=(230,250))
        fr1=self.frame(size=(470,30) ,pos=(15,295))
        fr1=self.frame(size=(158,26) ,pos=(325,297))
        lb7=self.label('Avances',size=(220,20),pos=(20,300))
        self.lav=self.label('--',size=(50,20),pos=(330,300),frame='white')
        av_s=self.btn('+',self.avs,size=(40,20),pos=(400,300))
        av_r=self.btn('-',self.avr,size=(40,20),pos=(440,300))
        fr1=self.frame(size=(470,30) ,pos=(15,325))
        fr1=self.frame(size=(158,26) ,pos=(325,327))
        lb9=self.label('Velocidad',size=(220,20),pos=(20,330))
        self.lve=self.label('--',size=(50,20),pos=(330,330),frame='White')
        ve_s=self.btn('+',self.ves,size=(40,20),pos=(400,330))
        ve_r=self.btn('-',self.ver,size=(40,20),pos=(440,330))
        fr1=self.frame(size=(470,30) ,pos=(15,355))
        fr1=self.frame(size=(158,26) ,pos=(325,357))
        lbl10=self.label('Aceleracion',size=(220,20),pos=(20,360))
        self.lac=self.label('--',size=(50,20),pos=(330,360),frame='White')
        ac_s=self.btn('+',self.acs,size=(40,20),pos=(400,360))
        ac_r=self.btn('-',self.acr,size=(40,20),pos=(440,360))
        fr1=self.frame(size=(470,30) ,pos=(15,385))
        fr1=self.frame(size=(158,26) ,pos=(325,387))
        lb13=self.label('Repeticiones',size=(220,20),pos=(20,390))
        self.lre=self.label('--',size=(50,20),pos=(330,390),frame='White')
        re_s=self.btn('+',self.res,size=(40,20),pos=(400,390))
        re_r=self.btn('-',self.rer,size=(40,20),pos=(440,390))
        fr1=self.frame(size=(470,30) ,pos=(15,415))
        fr1=self.frame(size=(158,26) ,pos=(325,417))
        lb14=self.label('Unidades X ciclo',size=(220,20),pos=(20,420),)
        self.luc=self.label('--',size=(50,20),pos=(330,420),frame='White')
        uc_s=self.btn('+',self.ucs,size=(40,20),pos=(400,420))
        uc_r=self.btn('-',self.ucr,size=(40,20),pos=(440,420))
        
        lb15=self.label('Conteo',size=(300,30),pos=(500,220))
        lb16=self.label('Ciclos',size=(100,30),pos=(520,300))
        self.lci=self.label('--',size=(50,30),pos=(650,300),frame='White')
        lb18=self.label('Unidades',size=(100,30),pos=(520,350))
        self.lun=self.label('--',size=(50,30),pos=(650,350),frame='White')
        btt5=self.btn('Iniciar',self.iniciar,size=(260,50),pos=(520,400))
        self.setGeometry(50,50,800,480)
        self.setWindowTitle('Maquina cortadora')
        self.conecta()
        self.showFullScreen()
##        self.show()
    def frame(self,size=0,pos=0):
        frm=QtGui.QFrame(self)
        if size!=0:
            frm.resize(size[0],size[1])
        if pos!=0: 
            frm.move(pos[0],pos[1])
        frm.setFrameShape(QtGui.QFrame.Box)
        frm.setFrameShadow(QtGui.QFrame.Sunken)
        return frm
    def label(self,txt,size=0,pos=0,frame=0):
        if frame!=0: 
            lfr=self.frame(size,pos)
            stl="QFrame { background-color:"+frame+"}"
            lfr.setStyleSheet(stl)
        lbl=QtGui.QLabel(txt,self)
        if size!=0:
            lbl.resize(size[0],size[1])
        if pos!=0: 
            lbl.move(pos[0],pos[1])
        
        return lbl
        
    def btn(self,name,funct,size=0,pos=0):
        btn=QtGui.QPushButton(name,self)
        btn.clicked.connect(funct)
        if size!=0:
            btn.resize(size[0],size[1])
        if pos!=0:
            btn.move(pos[0],pos[1])
        return btn
    def combo(self,items,size=0,pos=0):
        cbx=QtGui.QComboBox(self)
        if type(items)==tuple:
            for i in range(len(items)):
                cbx.addItem(items[i])
        else:
            cbx.addItem(items[i])
        if size!=0:
            cbx.resize(size[0],size[1])
        if pos!=0:
            cbx.move(pos[0],pos[1])
        cbx.activated[str].connect(self.com_fun)
        return cbx
    def com_fun(self,text):
        self.selected=text
        self.ci=0
        self.lci.setText("0")
        seq=self.val_com[text]
        self.lav.setText(str(seq[0]))
        self.av=seq[0]
        self.lve.setText(str(seq[1]))
        self.ve=seq[1]
        self.lac.setText(str(seq[2]))
        self.ac=seq[2]
        self.lre.setText(str(seq[3]))
        self.re=seq[3]
        self.luc.setText(str(seq[4]))
        self.uc=seq[4]
        
    def conecta(self):
        print('Conectar')
        if not(self.dA):
            (self.SerA,self.dA)=conectarA()
            if self.dA:
                print('Ca')
                Ca=threading.Thread(name='sa',target=self.conA)
                Ca.start()
            
        if not(self.dB):
            (self.SerB,self.dB)=conectarB()
            if self.dB:
                print('cb')
                Cb=threading.Thread(name='sb',target=self.conB)
                Cb.start()
    def conB(self):
        self.dB=on_B()
        while self.dB==True:
            self.lMb.setText("activo")
            self.fMb.setStyleSheet("QFrame { background-color: Green }" ) 
            self.dB=on_B()
        self.lMb.setText("inactivo")
        self.fMb.setStyleSheet("QFrame { background-color: Red }" )
    def conA(self):
        self.dA=on_A()
        while self.dA==True:
            self.lMa.setText("activo")
            self.fMa.setStyleSheet("QFrame { background-color: Green }" ) 
            self.dA=on_A()
        self.lMa.setText("inactivo")
        self.fMa.setStyleSheet("QFrame { background-color: Red }" ) 
    def iniciar(self):
        if not(self.busy):
            Se=threading.Thread(target=self.Secuencia)
            Se.start()
    def bk(self):
        if not(self.busy):
            Se=threading.Thread(target=self.back)
            Se.start()    

    def gp(self):
        if self.dB and not(self.busy):
            print("?")
            posi=send_ser("?",self.SerB)
            print(posi);
            self.lps.setText(str(float(posi)));
    def bt4(self):
        if not(self.busy):
            Se=threading.Thread(target=self.Cor)
            Se.start()
    def bt3(self):
        if not(self.busy):
            Se=threading.Thread(target=self.Rem)
            Se.start()
    def bt2(self):
        if not(self.busy):
            Se=threading.Thread(target=self.Avm)
            Se.start()
    def back(self):
        self.busy=True
        if self.dB:
            send_ser("-1000;70;15",self.SerB)
        self.busy=False
    def Avm(self):
        self.busy=True
        if self.dB:
            send_ser("100;70;15",self.SerB)
        self.busy=False
    def Rem(self):
        self.busy=True
        if self.dB:
            send_ser("-100;70;15",self.SerB)
        self.busy=False
    def Cor(self):
        self.busy=False
        if self.dA:
            send_ser("1",self.SerA)
        self.busy=False
    def avs(self):
        self.av=self.av+1
        self.lav.setText(str(self.av))
    def avr(self):
        self.av=self.av-1
        self.lav.setText(str(self.av))

    def ves(self):
        self.ve=self.ve+1
        self.lve.setText(str(self.ve))        
    def ver(self):
        if self.ve>70:
            self.ve=self.ve-1
        self.lve.setText(str(self.ve))
        
    def acs(self):
        self.ac=self.ac+1
        self.lac.setText(str(self.ac))
    def acr(self):
        if self.ac>1:
            self.ac=self.ac-1
        self.lac.setText(str(self.ac))
        
    def res(self):
        self.re=self.re+1
        self.lre.setText(str(self.re))
    def rer(self):
        if self.re>0:
            self.re=self.re-1
        self.lre.setText(str(self.re))
    def ucs(self):
        self.uc=self.uc+1
        self.luc.setText(str(self.uc))
    def ucr(self):
        if self.uc>0:
            self.uc=self.uc-1
        self.luc.setText(str(self.uc))
    def getPorts(self):
        return (self.SerA,self.dA,self.SerB,self.dB)
        
    def Secuencia(self):
        self.busy=True
        for i in range(self.re):
            send_str=str(self.av)+";"+str(self.ve)+";"+str(self.ac)
            print(send_str)
            if self.dB:
                self.ci=self.ci+1;
                self.lci.setText(str(self.ci))
                self.lun.setText(str(self.ci*self.uc))
                send_ser(send_str,self.SerB)
                posi=send_ser("?",self.SerB)
                self.lps.setText(str(float(posi)));
                if self.dA:
                    send_ser("1",self.SerA)          
        print("800;70;15")
        if self.dB:
            send_ser("-800;70;15",self.SerB)
        posi=send_ser("?",self.SerB)
        self.lps.setText(str(float(posi)));
        self.busy=False

def main():
    app=QtGui.QApplication(sys.argv)
    GUI=Window()
    sys.exit(app.exec_())
    
    
gui=threading.Thread(name='gui',target=main)
gui.start()
