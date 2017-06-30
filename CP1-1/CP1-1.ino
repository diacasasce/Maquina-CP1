/*
    CP1.1
    Movimiento del servomotor sin uso de libreria
    Ventaja mas velocidad
    desventaja? ... mas codigo
*/
//Configuracion pines I/O
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

// variables del sistema
int Pasos = 3200;
float Resol = 0.0016; // en mm
float Avance = 5; // mm/rev
long Fac_vel = 0;// el factor cambia de RPM a us para controlar la velocidad.
float Fac_ava = 0; // el factor cambia de mm a pasos para controlar el avance

//variables del proceso
float Pos_actual = 0;
int Stp_rec = 0; // pasos recorridos
long Vel_del = 0; // delay correspondiente a la velocidad convertida.
float Vel_actual = 0;
float Vel_max = 200;
int k = 60;
float Vobj = 0;

void setup() {
  // inicializacion de pines I/O
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
  // inicializa el puerto serial
  Serial.begin(9600);
  // se da valor a los factores de conversion
  Fac_ava = Pasos / Avance;
  Fac_vel = (Pasos / 30); // de RPM a Pasos por Segundo

}

void loop() {
  long pre = millis();
  avance(110, 10, 360);
  long act = millis();
  float rpm = (act - pre);
  rpm = 5 / rpm;
  rpm = rpm * 1000;
  rpm = 60 * rpm;
  delay(2000);
  k += 10;
}

void set_Vel(float RPM) {
  Vel_del = (1 / ((RPM / 1) * Fac_vel)) * 1000000;
}
long get_steps(float dist) {
  long result = dist * Fac_ava;
  return result;
}
float get_dist(long steps) {
  float result = steps;
  result = result / Fac_ava;
  return result;
}
void avance(float dist, int porcentaje, float Vel_fin) {
  Vobj = Vel_fin;
  int porcent = porcentaje;
  if (porcent < 10) {
    porcent = 10;
  }
  float intervalo = dist / 100;
  float dV = Vel_fin / porcentaje;
  Vel_actual = 0;
  float falta = dist;
  while (porcent > 0) {
    Vel_actual += dV;
    set_Vel(Vel_actual);
    //    Serial.println(Vel_actual);
    falta = recorrer(intervalo);
    if (abs(falta) > 0) {
      avance(falta, porcentaje / 2, dV);
      Serial.println(falta);
    }
    porcent -= 1;
  }
  set_Vel(Vel_fin);
  float restante = porcentaje;
  restante = (1 - (restante / 100)) * dist;
  //Serial.println(restante);
  falta = recorrer(restante);
  if (abs(falta) > 0) {
    //    Serial.println(falta);
    avance(falta, porcentaje / 2, Vel_fin);
  }
}
float recorrer(float dist) {
  long steps = get_steps(dist);
  long sig = abs(dist) / dist;
  long stp = abs(steps);
  if (sig > 0) { //giro horario
    digitalWrite(Dir, LOW);
  }
  if (sig < 0) { //giro anti-horario
    digitalWrite(Dir, HIGH);
  }
  long falta = sig * Steps(stp);
  if (abs(falta) > 0) {
    Serial.println(get_dist(falta));
  }
  return (get_dist(falta));
}
long Steps(long steps) {
  long stp = steps;
  while (stp > 0) {
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

/*
   FUNCION emergencia(Bool Paro)
   Bool Paro -> variable que indica si está activo el paro de emergencia o no lo está

   La funcion espera a que se desactive el paro de emergencia y continua el proceso desde donde se detuvo.
*/
bool emergencia() {
  bool stopped = false;
  while (digitalRead(Paro)) {
    stopped = true;
        Serial.println("wait");
  }
  if (stopped && (Vel_actual > (0.5 * Vobj))) {
    //    Serial.println("emergencia");
    Serial.println("entra");
    return true;

  }
  return false;
}


