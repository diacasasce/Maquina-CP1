from time import sleep
import serial
from serial.tools.list_ports import *
def conectar():
    dA=False
    dB=False
    ports = list(comports())
    for p in ports:
        f=p.device
        print(f)
        f=p.description
        print(f)
        f=p.hwid
        print(f)
        dev=list(grep("USB VID:PID=1A86:7523 LOCATION=1-1.3"))
        for d in dev:
            dA=True
            print(d.device)
            devA=d.device
        dev=list(grep("USB VID:PID=1A86:7523 LOCATION=1-1.2"))
        for d in dev:
            print(d.device)
            dB=True
            devB=d.device
    if dA:
        serA= serial.Serial(devA, 9600,timeout=2) # Establish the connection on a specific port
    else:
        serA=dA
    if dB:
        serB= serial.Serial(devB, 9600,timeout=2) # Establish the connection on a specific port
    else:
        serB=dB
    return (serA,dA,serB,dB)
def conectarA():
    dA=False
    ports = list(comports())
    for p in ports:
        f=p.device
        print(f)
        f=p.description
        print(f)
        f=p.hwid
        print(f)
        dev=list(grep("USB VID:PID=1A86:7523 LOCATION=1-1.3"))
        for d in dev:
            dA=True
            print(d.device)
            devA=d.device
    if dA:
        serA= serial.Serial(devA, 9600,timeout=2) # Establish the connection on a specific port
    else:
        serA=dA
    return (serA,dA)
def conectarB():
    dB=False
    ports = list(comports())
    for p in ports:
        f=p.device
        print(f)
        f=p.description
        print(f)
        f=p.hwid
        print(f)
        dev=list(grep("USB VID:PID=1A86:7523 LOCATION=1-1.2"))
        for d in dev:
            dB=True
            print(d.device)
            devB=d.device
    if dB:
        serB= serial.Serial(devB, 9600,timeout=2) # Establish the connection on a specific port
    else:
        serB=dB
    return (serB,dB)
def send_ser(str,port):
    port.write(str.encode())
    return port.readline()
def send_nr(str,port):
    port.write(str.encode())
