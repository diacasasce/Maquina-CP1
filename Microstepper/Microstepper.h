/*
  Microstepper.h
Libreria para el uso de driver microstepper
Creada por Diego Alejandro Casas, Julio 12, 2017

*/
#ifndef Microstepper_h
#define Microstepper_h

#include "Arduino.h"

class Microstepper
{
  public:
    Microstepper(int Pulp,int Puln,int Dir,float Resol,float Avance);
    void set_Vel(float RPM);
    void set_paro(int Paro);
    void set_manual(int avance,int retro);
    void avance(float dist, int porcentaje, float Vel_fin);
    float recorrer(float dist);
    long Steps(long steps, int dir);
    bool emergencia();
    int av_manual();

int go_home();
  private:
    int _Pulp;
    int _Puln;
    int _Dir;
    int _Paro;
    int _Av_man;
    int _Re_man;
    float _Resol;
    float _Avance;
    float _Fac_velo;
    float _Fac_ava;
    float _Vel_del;
    flaot _Vel_actual;
};
#endif