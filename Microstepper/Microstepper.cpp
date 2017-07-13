/*
  Microstepper.cpp
Libreria para el uso de driver microstepper
Creada por Diego Alejandro Casas, Julio 12, 2017

*/

#include "Arduino.h"
#include "Microstepper.h"

Microstepper::Microstepper(int Pulp,int Puln,int Dir,int Pasos,float Avance){
    pinMode(Pulp, OUTPUT); //p12
    _Pulp=Pulp;
  pinMode(Puln, OUTPUT); //p11
    _Puln=Puln;
  pinMode(Dir, OUTPUT); //10
    _Dir=Dir;
    _Pasos=Pasos;
    _Avance=Avance;
    _Fac_ava = _Pasos / _Avance;
  _Fac_vel = (_Pasos / 30);
    _Vel_del=0;
    _vel_actual=0;
}

void Microstepper::set_Paro(int Paro){
    _Paro=Paro;
}
void Microstepper::set_Manual(int avance,int retro){
    _Av_man=avance;
    _Av_ret=retro;
}
void Microstepper::set_final(int fin_av, int fin_re){
    _fin_av=fin_av;
    _fin_ret=fin_re;
}
/* Funcion ser_Vel ( foat RPM)
   Se encarga de convertir el de la velocidad en RPM a el valor del tiempo de espera entre los cambios de la señal de control del motor.
   Parametros : float RPM -> corresponde a la velocidad en revoluciones por minuto.
*/
void Microstepper::set_Vel(float RPM) {

  _Vel_del = (1 / ((RPM / 1) * _Fac_vel)) * 1000000;
}
/* Funcion get_steps(float dist)
   Se encarga de convertir el valor de distancia de avance requerida en pasos del motor, teniendo en cuenta las configuraciones del motor.
   Esta funcion es la funcion contraria a get_dist.
   Parametros: float dist -> corresponde a la distancia deseada en unidades de mm.
   Salidas: long result -> numero de pasos del motor correspondientes a la distancia requerida.
*/
long Microstepper::get_steps(float dist) {
  long result = dist * _Fac_ava;
  return result;
}
/* Funcion get_dist(long Steps)
   Se encarga de devolver la distancia en mm correspondiente al numero de pasos ingresados como parametro, segun las condiciones del motor.
   Esta funcion es la funcion contraria a get_steps.
   Parametros:  long steps -> corresponde al numero de pasos del motor.
   Salidas: float result -> distanci en mm correspondiente con el numero de pasos ingresados.
*/
float Microstepper::get_dist(long steps) {
  float result = steps;
  result = result / _Fac_ava;
  return result;
}
/* Funcion avance(float dist, int porcentaje, float Vel_fin)
   Esta funcion permite el control de avance del dispositivo con un periodo de aceleracion, desde 0 hasta la velocidad requerida, segun lo establezca el usuario.
   La aceleracion se realiza de forma lineal.
   Parametros:
   float dist -> Distancia de avance deseada en mm.
   int porcentaje -> prorcentaje del recorrido total que se desea como periodo de aceleracion 10-90.
   float vel_final -> Velocidad final del desplazamiento.
*/
void Microstepper::avance(float dist, int porcentaje, float Vel_fin) {
  Serial.println("avanza");
  float Pos_final = 0;
  int porcent = porcentaje;
  float intervalo = dist / 100;
  float dV = Vel_fin / porcentaje;
  float falta = dist;
  _Vel_actual = 0;
  Pos_final = _Pos_actual + dist;
  if (porcent < 5) {
    porcent = 5;
  }
  if (porcent > 90) {
    porcent = 90;
  }
  // ya esta definido el porcentaje del recorrido correspndiente a la aceleracion.
  for (int i = 0; i < 100; i++) {
    if (i < porcent) {
      _Vel_actual += dV;
    } else {
      _Vel_actual = Vel_fin;
    }
    set_Vel(_Vel_actual);
    recorrer(intervalo);
  }
}
/* Funcion recorrer(float dist)
   Esta funcion se encarga de realizar desplazamientos del dispositivo a velocidad constante, preciamente definida usando la funcion set_vel
   Parametros: float dist-> distncia deseada para el desplazamiento en mm
   Salidas: faltan-> la cantidad de mm faltantes para completar el recorrido, usalmente se espera que el valor devuelto sea 0 pero en el caso de no serlo se devuelve el valor para poder realizar los ajustes necesarios para completar elrecorrido, la variacion en este resultado se debe al empleo del paro de emergencias.
*/
float Microstepper::recorrer(float dist) {
  
  long steps = get_steps(dist);
  long sig = abs(dist) / dist;
  long stp = abs(steps);
  if (sig > 0) { //giro horario
    digitalWrite(_Dir, HIGH);
  }
  if (sig < 0) { //giro anti-horario
    digitalWrite(_Dir, LOW);
  }
  long falta = sig * Steps(stp, sig);
  float faltan = get_dist(falta);
  if (abs(falta) > 0) {
    Serial.println(faltan);
  }
  Pos_actual += (dist - faltan);
  return (faltan);
}
/*Funcion Steps(long steps)
    esta funcion realiza el movimiento del dispositivo una cantidad establecida de pasos.
    Parametros: long steps -> corresponde al numero de pasos que se desea recorrer.
    Salidas: long stp -> corresponde al numero de pasos faltantes para terminar el recorrido del elemento, este valor se mantiene en 0, pero puede variar segun el uso del paro de emergencia.
*/
long Microstepper::Steps(long steps, int dir) {
  long stp = steps;
  while (stp > 0) {
    if (!digitalRead(_fin_av) && dir > 0) {
      return 0;
    }
    if (!digitalRead(_fin_ret) && dir < 0) {
      return 0;
    }
    //Serial.println(stp);
    digitalWrite(_Pulp, LOW);
    digitalWrite(_Puln, HIGH);   // turn the LED on (HIGH is the voltage level)
    delayMicroseconds(_Vel_del);                       // wait for a second
    digitalWrite(_Puln, LOW);
    digitalWrite(_Pulp, HIGH);    // turn the LED off by making the voltage LOW
    delayMicroseconds(_Vel_del);
    stp--;
    if (emergencia()) {
      Serial.println(stp);
      return stp;
    }
  }
  return 0;
}
/* Funcion emergencia()
    esta funcion se encarga de la rutia de paro de emergencia definida por el proceso.
    - funcion espera a que se desactive el paro de emergencia y continua el proceso desde donde se detuvo.
    en caso de que el dispositivo llevara una velocidad considerable para reiniciar desde la ultima condicion de velocidad, se devolvera un valor verdadero, en el caso contrario se deolvera falso.
*/
bool Microstepper::emergencia() {
  bool stopped = false;
  while (digitalRead(_Paro)) {
    stopped = true;
    Serial.println("wait");
    av_manual();
    delay(100);
  }
  if (stopped && (_Vel_actual > (0.5 * Vobj))) {
    //    Serial.println("emergencia");
    Serial.println("entra");
    return true;

  }
  return false;
}
/*Funcion av_manual()
   Reliza avances manuales a baja velocidad y ajusta la posicion mientras lo hace para poder hacer un avance a la siguiente posicion.
   Se establece una velocidad de 10 RPM para los movimientos en ambas direcciones
*/
int Microstepper::av_manual() {
  int Vac = _Vel_actual;
  set_Vel(10);
  if (digitalRead(_Av_man)) {
    digitalWrite(_Dir, LOW);
    Steps(1, 1);
    Pos_actual += get_dist(1);
    set_Vel(Vac);
    Serial.println(Pos_actual);
  } else if (digitalRead(_Re_man)) {
    digitalWrite(_Dir, HIGH);
    Steps(-1, -1);
    Pos_actual += get_dist(-1);
    set_Vel(Vac);
    Serial.println(_Pos_actual);
  }

}
