/*
   CP1.1
   Movimiento del servomotor sin uso de libreria
   Ventaja mas velocidad
   desventaja? ... mas codigo
*/
// Pines de Selector
const int sw1 = 52;
const int sw2 = 53;
//Configuracion pines I/O
const int pEna = 14; // Pulso positivo Piston
const int pPul = 17; // Pulso negativo Piston
const int pDir = 16; // Direccion Piston
const int Pulp = 12; // este pin se conecta al Pul+ del driver control de velocidad -> IN
const int Puln = 11; // este pin se conecta al Pul- del driver control de velocidad -> IN
const int Dir = 10; // este pin de conecta a dir- en el driver control de direccion -> IN
const int fin_av = 9; // final de carrera en el punto de corte -> IN
const int fin_ret = 2; // final de carrera en el punto inicial -> IN
const int Modo = 7; // interruptor para seleccionar manual ( 1 ) o automatico (0) -> IN
const int Paro = 6; // interruptor paro de emergencia-> IN
const int Next = 5; // pulsador de Siguente-> IN
const int Av_man = 4; // pulsador avance manual -> IN
const int Re_man = 8; // pilsador retroceso manual -> IN
const int Co_man = 3; // pilsador retroceso manual -> IN
const int ModP = A0;
const int ModV = A1;
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
  // inicializacion pines de selector
  pinMode(sw1, INPUT_PULLUP);
  pinMode(sw2, INPUT_PULLUP);
  // inicializacion de pines I/O
  pinMode(pPul, OUTPUT); //p14
  pinMode(pEna, OUTPUT); //p15
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
  Serial.println("Set");
  wait_Next();
  avance(-800, 20, 600);
  //wait_Next();
  digitalWrite(pEna, LOW);
  //  sube_Corte(20);
  digitalWrite(pEna, LOW);
//  avance(150, 20, 600);
  Serial.println("Start");
}

void loop() {


  bool s1 = digitalRead(sw1);
  bool s2 = digitalRead(sw2);

  /*

    if (s1 && s2) {
    wait_Next();
    BloqueM25k();
    } else if (s1 && !s2) {//verde
    wait_Next();
    BloqueM5k();
    } else if (s2 && !s1) {
    avance(200,10,500);
    wait_Next();
    BloqueC();
    }
  */
  wait_Next();
  BloqueM5k();


  delay(1000);

  //av_manual();

}
/*
  -+   Funciones Creadas
*/
/* Funcion set_Vel ( foat RPM)
   Se encarga de convertir el de la velocidad en RPM a el valor del tiempo de espera entre los cambios de la señal de control del motor.
   Parametros : float RPM -> corresponde a la velocidad en revoluciones por minuto.
*/
long set_Vel(float RPM) {
  long vel_del;
  vel_del = (1 / ((RPM / 1) * Fac_vel)) * 1000000;
  return vel_del;
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
  float corr = 1.052631579;
  dist = dist * corr;
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
    Vel_del = set_Vel(Vel_actual);
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
void secuencia(float pasos, float avances[], float aceleraciones[], float velocidades[], bool corte[] ) {
  for (int i = 0; i < pasos; i++) {
    float modP = analogRead(A0);
    modP = map(modP, 0, 1023, 0, 2000);
    modP = modP / 1000;
    float modV = analogRead(A1);
    modV = map(modV, 0, 1023, 0, 2000);
    modV = modV / 1000;
    modV = 1;
    modP = 1;
    float av = avances[i] * modP;
    float ac = aceleraciones[i];
    float ve = velocidades[i] * modV;
    bool co = corte[i];
    if (av != 0) {
      avance(av, ac, ve);
    } else {
      wait_Next();
    }
    Serial.println(Pos_actual);
    ;
    if (av > 0 && co) {
      if (!digitalRead(Modo) ) {
        wait_Next();
        // Corte(25);
        wait_Next();
      } else {
        //Corte(25);
      }
    } else {
      if (!digitalRead(Modo)) {
        wait_Next();
      } else {
        delay(100);
      }
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

  float ac = 0;
  float k = 10;
  int dir;
  if (digitalRead(Co_man)) {
    Serial.println("cut");
    digitalWrite(pEna, LOW);
    Corte(25);
  } else {
    while (digitalRead(Re_man) || digitalRead(Av_man)) {
      digitalWrite(pEna, LOW);
      ac = k / 50;
      k++;
      Serial.println(k);

      Vel_del = set_Vel(300 * ac);
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
        if ((digitalRead(fin_av) && dir < 0) || (digitalRead(fin_ret) && dir > 0)) {
          return 0;
        } else {
          digitalWrite(Pulp, LOW);
          digitalWrite(Puln, HIGH);   // turn the LED on (HIGH is the voltage level)
          delayMicroseconds(Vel_del);                       // wait for a second
          digitalWrite(Puln, LOW);
          digitalWrite(Pulp, HIGH);    // turn the LED off by making the voltage LOW
          delayMicroseconds(Vel_del);
        }
      }
    }
  }
  Vel_del = set_Vel(Vac);
}
/*Funcion go_home()
   Secuencia de inicio para configurar el punto cero de la maquina.
*/
void go_home() {
  Serial.println("go home");
  while (!digitalRead(fin_ret)) {
    Serial.println("go back");
    Vel_del = set_Vel(100);
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
    av_manual();
  }
  return;
}
/* funcion Corte()
  define la rutina de corte completa un avance y un retroceso
*/

void Corte (float dist) {
  baja_Corte(10);
  wait_Next();
  sube_Corte(10);
}
void baja_Corte(float dist) {
  float corr = 0.7018;
  Serial.println("");
  Serial.println("baja");
  long   stp = get_steps(4 * dist * corr);
  int inter = stp / 40;
  long stpi = stp;
  float ac = 0;
  float k = 10;
  delay(1000);
  Serial.println("cut");
  ac = 250;

  while (stpi > 0) {
    digitalWrite(pEna, HIGH);
    digitalWrite(pDir, HIGH);
    ac = k / 500;

    long vec = set_Vel(200 * ac);
    Serial.println(vec);
    if (vec > 50) {
      k++;
    }
    if (vec > 150) {
      vec = 150;
    }
    int sp = 25;
    if (sp > stpi) {
      sp = stpi;
    }
    for (int i = 0; i < sp; i++) {
      digitalWrite(pPul, HIGH);   // turn the LED on (HIGH is the voltage level)
      delayMicroseconds(vec);                       // wait for a second
      digitalWrite(pPul, LOW);    // turn the LED off by making the voltage LOW
      delayMicroseconds(vec);
    }

    // wait for a second
    stpi -= sp;
  }
  digitalWrite(pEna, LOW);
  digitalWrite(pEna, LOW); //activado
}
void sube_Corte(float dist) {
  float corr = 0.7018;
  Serial.println("");
  Serial.println("cortando");
  long   stp = get_steps(4 * dist * corr);
  int inter = stp / 40;
  long stpi = stp;
  float ac = 0;
  float k = 10;
  delay(1000);
  Serial.println("cut");
  ac = 250;

  while (stpi > 0) {
    digitalWrite(pEna, HIGH);
    digitalWrite(pDir, LOW);
    ac = k / 500;

    long vec = set_Vel(200 * ac);
    Serial.println(vec);
    if (vec > 50) {
      k++;
    }
    if (vec > 150) {
      vec = 150;
    }
    int sp = 25;
    if (sp > stpi) {
      sp = stpi;
    }
    for (int i = 0; i < sp; i++) {
      digitalWrite(pPul, HIGH);   // turn the LED on (HIGH is the voltage level)
      delayMicroseconds(vec);                       // wait for a second
      digitalWrite(pPul, LOW);    // turn the LED off by making the voltage LOW
      delayMicroseconds(vec);
    }

    // wait for a second
    stpi -= sp;
  }
}
void BloqueM5k() {
  float  avan[] = {500, -130, 0, 100, -800};
  float  acel[] = {20, 10, 10, 10 , 10};
  float  velo[] = {450, 600, 0, 200, 600};
  bool  cort[] = {0, 0, 0, 0, 0};
  int  movs = 5;
  secuencia(movs, avan, acel, velo, cort);
}
void BloqueM25k() {
  float  avan[] = {295, -130, 0, 270, -800};
  float  acel[] = {20, 10, 10, 10 , 10};
  float  velo[] = {350, 600, 0, 200, 600};
  bool  cort[] = {1, 0, 0, 0, 0};
  int  movs = 5;
  secuencia(movs, avan, acel, velo, cort);
}
void BloqueC() {
  float  avan[] = {300, 50, -800, 150};
  float  acel[] = {10,   10 , 10, 10};
  float  velo[] = {600, 100,  600, 600};
  bool  cort[] = {0, 0, 0, 0, 0};
  int  movs = 4;
  secuencia(movs, avan, acel, velo, cort);
}
