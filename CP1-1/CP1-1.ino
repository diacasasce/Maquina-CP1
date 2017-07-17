/*
    CP1.1
    Movimiento del servomotor sin uso de libreria
    Ventaja mas velocidad
    desventaja? ... mas codigo
*/
//Configuracion pines I/O
const int pPulp = 14; // Pulso positivo Piston
const int pPuln = 15; // Pulso negativo Piston
const int pDir = 16; // Direccion Piston
const int Pulp = 12; // este pin se conecta al Pul+ del driver control de velocidad -> IN
const int Puln = 11; // este pin se conecta al Pul- del driver control de velocidad -> IN
const int Dir = 10; // este pin de conecta a dir- en el driver control de direccion -> IN
const int fin_av = 9; // final de carrera en el punto de corte -> IN
const int fin_ret = 8; // final de carrera en el punto inicial -> IN
const int Modo = 7; // interruptor para seleccionar manual ( 1 ) o automatico (0) -> IN
const int Paro = 6; // interruptor paro de emergencia-> IN
const int Next = 5; // pulsador de Siguente-> IN
const int Av_man = 4; // pulsador avance manual -> IN
const int Re_man = 3; // pilsador retroceso manual -> IN
const int Co_man = 2; // pilsador retroceso manual -> IN
// variables del sistema
int Pasos = 3200;
float Resol = 0.0016; // en mm
float Avance = 5; // mm/rev
long Fac_vel = 0;// el factor cambia de RPM a us para controlar la velocidad.
float Fac_ava = 0; // el factor cambia de mm a pasos para controlar el avance
int Fac_unit = 4;
float Pos_max = 550;

//variables del proceso
float Pos_actual = 0;
long Vel_del = 0; // delay correspondiente a la velocidad convertida.
float Vel_actual = 0;
float Vel_max = 200;
int k = 60;
float Vobj = 0;
long int Unit = 0;
long int Ciclos;
void setup() {
  // inicializacion de pines I/O
  pinMode(pPulp, OUTPUT); //p14
  pinMode(pPuln, OUTPUT); //p15
  pinMode(pDir, OUTPUT); //p16
  pinMode(Pulp, OUTPUT); //p12
  pinMode(Puln, OUTPUT); //p11
  pinMode(Dir, OUTPUT); //10
  pinMode(fin_av, INPUT); //p9
  pinMode(fin_ret, INPUT); //p8
  pinMode(Modo, INPUT);  //p7
  pinMode(Paro, INPUT);  //p6
  pinMode(Next, INPUT);  //p5
  pinMode(Av_man, INPUT);  //p4
  pinMode(Re_man, INPUT);  //p3
  pinMode(Co_man, INPUT);  //p3
  // inicializa el puerto serial
  Serial.begin(9600);
  // se da valor a los factores de conversion
  Fac_ava = Pasos / (1.35 * Avance);
  Fac_vel = (Pasos / 30); // de RPM a Pasos por Segundo
  Fac_unit = 4;
  avance(-500, 10, 600);
  wait_Next();
  Serial.println("Start");
}

void loop() {
  /*
    Serial.print("ciclos: ");
    Serial.print(Ciclos);
    Serial.print("unidades: ");
    Serial.println(Unit);
  */
  
  /*
  int avan[] = {100, 100, 100, 100, 1510, -550};
  int acel[] = {25, 25, 25, 25, 25 , 10};
  int velo[] = {150, 150, 150, 150, 150, 600};*/
  int avan[] = {560, -560};
  int acel[] = {10 , 10};
  int velo[] = {300, 600};
  long pre = millis();
  //secuencia(2, avan, acel, velo);
  Corte(5);
  wait_Next();
  //av_manual();

}
/*
  -+   Funciones Creadas
*/
/* Funcion ser_Vel ( foat RPM)
   Se encarga de convertir el de la velocidad en RPM a el valor del tiempo de espera entre los cambios de la señal de control del motor.
   Parametros : float RPM -> corresponde a la velocidad en revoluciones por minuto.
*/
void set_Vel(float RPM) {

  Vel_del = (1 / ((RPM / 1) * Fac_vel)) * 1000000;
}
/* Funcion get_steps(float dist)
   Se encarga de convertir el valor de distancia de avance requerida en pasos del motor, teniendo en cuenta las configuraciones del motor.
   Esta funcion es la funcion contraria a get_dist.
   Parametros: float dist -> corresponde a la distancia deseada en unidades de mm.
   Salidas: long result -> numero de pasos del motor correspondientes a la distancia requerida.
*/
long get_steps(float dist) {
  long result = dist * Fac_ava;
  return result;
}
/* Funcion get_dist(long Steps)
   Se encarga de devolver la distancia en mm correspondiente al numero de pasos ingresados como parametro, segun las condiciones del motor.
   Esta funcion es la funcion contraria a get_steps.
   Parametros:  long steps -> corresponde al numero de pasos del motor.
   Salidas: float result -> distanci en mm correspondiente con el numero de pasos ingresados.
*/
float get_dist(long steps) {
  float result = steps;
  result = result / Fac_ava;
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
void avance(float dist, int porcentaje, float Vel_fin) {
  Serial.println("avanza");
  float Pos_final = 0;
  int porcent = porcentaje;
  float intervalo = dist / 100;
  float dV = Vel_fin / porcentaje;
  float falta = dist;
  Vel_actual = 0;
  Pos_final = Pos_actual + dist;

  if (porcent < 5) {
    porcent = 5;
  }
  if (porcent > 90) {
    porcent = 90;
  }
  // ya esta definido el porcentaje del recorrido correspndiente a la aceleracion.
  for (int i = 0; i < 100; i++) {
    if (i < porcent) {
      Vel_actual += dV;
    } else {
      Vel_actual = Vel_fin;
    }
    set_Vel(Vel_actual);
    recorrer(intervalo);
  }
}
/* Funcion recorrer(float dist)
   Esta funcion se encarga de realizar desplazamientos del dispositivo a velocidad constante, preciamente definida usando la funcion set_vel
   Parametros: float dist-> distncia deseada para el desplazamiento en mm
   Salidas: faltan-> la cantidad de mm faltantes para completar el recorrido, usalmente se espera que el valor devuelto sea 0 pero en el caso de no serlo se devuelve el valor para poder realizar los ajustes necesarios para completar elrecorrido, la variacion en este resultado se debe al empleo del paro de emergencias.
*/
float recorrer(float dist) {

  long steps = get_steps(dist);
  long sig = abs(dist) / dist;
  long stp = abs(steps);
  if (sig > 0) { //giro horario
    digitalWrite(Dir, HIGH);
  }
  if (sig < 0) { //giro anti-horario
    digitalWrite(Dir, LOW);
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
long Steps(long steps, int dir) {
  long stp = steps;
  while (stp > 0) {
    if (digitalRead(fin_av) && dir > 0) {
      return 0;
    }
    if (digitalRead(fin_ret) && dir < 0) {
      return 0;
    }
    //Serial.println(stp);
    digitalWrite(Pulp, LOW);
    digitalWrite(Puln, HIGH);   // turn the LED on (HIGH is the voltage level)
    delayMicroseconds(Vel_del);                       // wait for a second
    digitalWrite(Puln, LOW);
    digitalWrite(Pulp, HIGH);    // turn the LED off by making the voltage LOW
    delayMicroseconds(Vel_del);
    stp--;
    if (emergencia()) {
      Serial.println(stp);
      return stp;
    }
  }
  return 0;
}
/* Funcion secuencia(int pasos, int avances[], int aceleraciones[], float velocidades[])
   Esta funcion se encarga de interpretar y correr la secuencia de operacion deseada, teniendo en cuenta las condiciones de desplazamiento aceleracion y velocidad de cada paso de la secuencia, asi como el modo de oeracion (manual - automatico).
   Parametros
   int pasos -> numero de acciones o pasos a desarrollar en la secuencia.
   int avances[] -> arreglo en el cual estan consignadas las diferentes condiciones de avance de cada paso.
   int aceleraciones[] -> arreglo en el cual estan consignadas las diferentes condiciones de aceleracion de cada paso.
   int velocidades[] -> arreglo en el cual estan consignadas las diferentes condiciones de velocidad de cada paso.
   - NOTA IMPORTANTE-
   SE RECOMIENDA QUE LOS 3 ARREGLOS TENGAN LA MISMA CANTIDAD DE ELEMENTOS PARA EVITAR INCONVENIENTES EN LA OPERACION
*/
void secuencia(int pasos, int avances[], int aceleraciones[], int velocidades[] ) {
  for (int i = 0; i < pasos; i++) {
    avance(avances[i], aceleraciones[i], velocidades[i]);
    Serial.println(Pos_actual);
    if (!digitalRead(Modo)) {
      wait_Next();
    } else {
      delay(1000);
    }
  }
  Ciclos++;
  Unit = Ciclos * Fac_unit;
}
/* Funcion emergencia()
    esta funcion se encarga de la rutia de paro de emergencia definida por el proceso.
  - funcion espera a que se desactive el paro de emergencia y continua el proceso desde donde se detuvo.
    en caso de que el dispositivo llevara una velocidad considerable para reiniciar desde la ultima condicion de velocidad, se devolvera un valor verdadero, en el caso contrario se deolvera falso.
*/
bool emergencia() {
  bool stopped = false;
  if (!digitalRead(Paro)) {
    Serial.println("Paro de emergencia");
  }
  while (!digitalRead(Paro)) {
    stopped = true;
    Serial.println("wait");
    delay(1000);
    av_manual();
    delay(100);
  }
  if (stopped && (Vel_actual > (0.5 * Vobj))) {
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
int av_manual() {
  int Vac = Vel_actual;

  float ac;
  float k = 10;
  int dir;
  if (digitalRead(Co_man)) {
    Corte(5);
  } else {
    while (digitalRead(Re_man) || digitalRead(Av_man)) {

      k++;
      ac = k / 50;
      Serial.println(k);

      set_Vel(300 * ac);
      if (digitalRead(Re_man)) {
        digitalWrite(Dir, HIGH);
        dir = -1;
        //      Serial.println("backward");
        Pos_actual += get_dist(-1);
        //     Serial.println(Pos_actual);
      }
      if (digitalRead(Av_man)) {
        digitalWrite(Dir, LOW);
        dir = 1;
        //    Serial.println("forward");
        Pos_actual += get_dist(-1);
        //     Serial.println(Pos_actual);
      }


      //Serial.println(stp);
      for (int i = 0; i < 100; i++) {
        if (digitalRead(fin_av) && dir > 0) {
          return 0;
        }
        if (digitalRead(fin_ret) && dir < 0) {
          return 0;
        }
        digitalWrite(Pulp, LOW);
        digitalWrite(Puln, HIGH);   // turn the LED on (HIGH is the voltage level)
        delayMicroseconds(Vel_del);                       // wait for a second
        digitalWrite(Puln, LOW);
        digitalWrite(Pulp, HIGH);    // turn the LED off by making the voltage LOW
        delayMicroseconds(Vel_del);
      }
    }
  }
  set_Vel(Vac);
}
/*Funcion go_home()
   Secuencia de inicio para configurar el punto cero de la maquina.
*/
void go_home() {
  Serial.println("go home");
  while (!digitalRead(fin_ret)) {
    Serial.println("go back");
    set_Vel(100);
    digitalWrite(Dir, LOW);
    Steps(1, -1);
  }
  Pos_actual = 0;
}
/*funcion wait_Next()
   espera a que se precione el boton siquiente;
*/
void wait_Next() {
  while (!digitalRead(Next)) {
    delay(1);
    Serial.print("next?");
    delay(100);
  }
  return;
}
/* funcion Corte()
  define la rutina de corte completa un avance y un retroceso
*/

void Corte(float dist) {
  Serial.println("");
  Serial.println("cortando");
  long   stp = get_steps(dist);
  
  long stpi = stp;
  Serial.println(stpi);
  long  Vel_crt = (1 / ((100 / 1) * Fac_vel)) * 1000000;
  digitalWrite(pDir, LOW);
  while (stpi > 0) {
    //Serial.println(stp);
    digitalWrite(pPulp, LOW);
    digitalWrite(pPuln, HIGH);   // turn the LED on (HIGH is the voltage level)
    delayMicroseconds(Vel_crt);                       // wait for a second
    digitalWrite(pPuln, LOW);
    digitalWrite(pPulp, HIGH);    // turn the LED off by making the voltage LOW
    delayMicroseconds(Vel_crt);
    stpi--;
    if (emergencia()) {
      Serial.println(stpi);
      //      return stpi;
    }
  }
  delay(500);
  digitalWrite(pDir, HIGH);
  while (stpi < stp) {
    //Serial.println(stp);
    digitalWrite(pPulp, LOW);
    digitalWrite(pPuln, HIGH);   // turn the LED on (HIGH is the voltage level)
    delayMicroseconds(Vel_crt);                       // wait for a second
    digitalWrite(pPuln, LOW);
    digitalWrite(pPulp, HIGH);    // turn the LED off by making the voltage LOW
    delayMicroseconds(Vel_crt);
    stpi++;
    if (emergencia()) {
      Serial.println(stpi);
      //      return stpi;
    }
  }
}
