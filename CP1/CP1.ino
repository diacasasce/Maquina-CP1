
// manejado por GitHub

#include <Stepper.h>
// variables del sistema
const int Pasos = 3200;  // Número de pasos * 4
const int Avance = 5; // avance del tornillo en mm(550 mm)
const int Rec_total = 550; // recorrido total posible en la maquina
const float Resol = 0.1; // resolucion del movimiento en mm (para mas resolucion mas pequeñp el valor se recomienda la resolucion permita que la variable factor sea un valor entero si no el movimiento no sera preciso)
//Definicion de PIN I/O
float Factor = 0; // factor de conversion para el movimiento de mm a pasos
const int fin_av = 7; // final de carrera en el punto de corte -> IN
const int fin_ret = 6; // final de carrera en el punto inicial -> IN
const int Modo = 5; // interruptor para seleccionar manual ( 1 ) o automatico (0) -> IN
const int Paro = 4; // interruptor paro de emergencia-> IN
const int Next = 3; // pulsador de Siguente-> IN
const int Av_man = 13; // pulsador avance manual -> IN
const int Re_man = 12; // pilsador retroceso manual -> IN


// inicializa la libreria 'stepper' en los pines 8 a 11
Stepper myStepper(Pasos, 8, 9, 10, 11);
// variables del programa
int recorrido = 0;

void setup() {
  // inicializacion de pines I/O
  pinMode(fin_av, INPUT);
  pinMode(fin_ret, INPUT);
  pinMode(Modo, INPUT);
  pinMode(Next, INPUT);
  pinMode(Av_man, INPUT);
  pinMode(Re_man, INPUT);
  // inicializa el puerto serial
  Serial.begin(9600);
  // se calcula el factor de conversion
  Factor = (Pasos / Avance) * Resol;
  Serial.println(Factor);


}

void loop() {
  // Gira sentido horario
  int avance = 5;
  int vel = 200;
  int av_final = 20;

  avanza(vel, 50, av_final);
  recorrido += av_final;
  Serial.println(digitalRead(Modo));
  if (!digitalRead(Modo)) {  // si esta en modo manual
    while (!digitalRead(Next)) { // mientras no se presione el pulsador de siguiene
      Serial.println(digitalRead(Next));
      delay(100); // espere
      Serial.println("wait");
    }
  } else {
    delay(500);
  }
  Serial.println(recorrido);
  Serial.println("done");
}
/*
  --------------------
  Funciones creadas
  --------------------
*/
/*
   FUNCION  avanza (int Vfin , int puntos, float avance )
   int Vfin -> velocidad final maxima
   int puntos -> numero de intervalos para dividir el incremento de la velocidad
   float avance -> avance total en milimetros (tener en cuenta la resolucion )

   La funcion se encarga de mover el motor un avance determinado teniendo en cuenta una aceleracion gradual
*/
void avanza(int Vfin, int puntos, float avance) {
  Serial.print("avanza : ");
  //  if (avance <= 50 && avance >= -50)
  //int sig = abs(avance) / avance;
  float av = avance;
  Serial.print(av);
  Serial.print(" ; ");
  Serial.print(av);
  Serial.print(" ; ");
  float k = puntos; // cont ador de incrementos
  float dV = Vfin / k; // Variacion de la velocidad por intervalo
  Serial.println(k);
  //Serial.print(" ; ");
  float Vel = 0;
  while (k > 0) {
    Vel += dV; // variable de invremento de la velocidad;
    Serial.println("entro");
    myStepper.setSpeed(Vel);
    Serial.println(Vel);
    recorrer(av / (4 * puntos));
    k--;
  }
  recorrer((3 * av) / 4);
}
/*
   FUNCION recorer( float avance)
   float avance -> avance en mm deseado, permite decimales , es importante tener en cuenta la resolucion
   La Funcion se encarga de mover el motor un avance determinado.
*/
void recorrer(float avance) {
  int av = avance / Resol;
  int sig = abs(avance) / avance;
  float steps = (sig * avance) / Resol;
  steps = steps * (Factor);
  while (steps > 0) {
    myStepper.step(sig);
    steps--;
    emergencia();
  }
}
/*
   FUNCION emergencia(Bool Paro)
   Bool Paro -> variable que indica si está activo el paro de emergencia o no lo está

   La funcion espera a que se desactive el paro de emergencia y continua el proceso desde donde se detuvo.
*/
void emergencia() {
  while (digitalRead(Paro)) {
    Serial.println("emergencia");
    man_Avance();
  }
}
/*Funcion man_Avance()*/
void man_Avance() {
  if (digitalRead(Av_man)) {
    recorrer(Resol);
  }
  if (digitalRead(Re_man)) {
    recorrer(-Resol);
  }
}
