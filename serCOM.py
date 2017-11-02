from time import sleep
import serial
from serial.tools.list_ports import *
import threading

def conectar():
    (serA,dA)=conectarA()
    (serB,dB)=conectarB()
    return (serA,dA,serB,dB)
def conectarA():
    (devA,dA)=getA()
    if dA:
        serA= serial.Serial(devA, 9600) # Establish the connection on a specific port
    else:
        serA=dA
    return (serA,dA)
def conectarB():
    (devB,dB)=getB()
    if dB:
        serB= serial.Serial(devB, 9600) # Establish the connection on a specific port
    else:
        serB=dB
    return (serB,dB)
def send_ser(str,port,tmo=0):
    port.write(str.encode())
    if tmo==0:
        return port.readline()
def send_nr(str,port):
    port.write(str.encode())
def getA():
    dA=False
    devA=False
    ports = list(comports())
    for p in ports:
        f=p.device
        f=p.description
        f=p.hwid
        dev=list(grep("USB VID:PID=1A86:7523 LOCATION=1-1.3"))
        for d in dev:
            dA=True
            devA=d.device
    return (devA,dA)

def getB():
    dB=False
    devB=False
    ports = list(comports())
    for p in ports:
        f=p.device
        f=p.description
        f=p.hwid
        dev=list(grep("USB VID:PID=1A86:7523 LOCATION=1-1.2"))
        for d in dev:
            dB=True
            devB=d.device
    return (devB,dB)

def on_B():
    (_,dB)=getB()
    return(dB)
def on_A():
    (_,dA)=getA()
    return(dA)

    