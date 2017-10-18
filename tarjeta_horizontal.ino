#include <TimerOne.h>

int velocidad = 11;
int direccion = 10;
int enable = 9;
int sw1 = 6; // pedal
int sw2 = 7; // switch
int sw3 = 5; // near
int sw4 = 4; // far
int s1;
int s2;
int s3;
int s4;

int del = 1000;
int Max = 1000;
int Min = 100;
int pasos = 3000;
int dir;
int ena;
int k = 0;
int paro = 0;
bool di = false;
int go = 2;
long tick = 0;
int stp = 0;
long stpm = 0;
float dist = 50;
float crte = 0;
float fcd = 16; // 1600 tick por cm => 16 po 0.1mm
int lA ;
int pas;
int snt=0;
int com = false;
int listo = 0;
// the setup routine runs once when you press reset:
void setup() {
  Serial.begin(9600);
  // initialize the digital pin as an output.
  pinMode(velocidad, OUTPUT);
  pinMode(direccion, OUTPUT);
  pinMode(enable, OUTPUT);
  pinMode(sw1, INPUT_PULLUP);
  pinMode(sw2, INPUT_PULLUP);
  pinMode(sw3, INPUT_PULLUP);
  pinMode(sw4, INPUT_PULLUP);
  digitalWrite(enable, LOW);
  del = Max;
  digitalWrite(enable, HIGH);
  Timer1.initialize(10000); // set a timer of length 100000 microseconds (or 0.1 sec - or 10Hz => the led will blink 5 times, 5 cycles of on-and-off, per second)
  Timer1.attachInterrupt( control ); // attach the service routine here
  s3 = digitalRead(sw3);
  s4 = digitalRead(sw4);
  digitalWrite(direccion, HIGH);  // This LOW to HIGH change is what creates the
  while (s3 == true) {
    s3 = digitalRead(sw3);
    s4 = digitalRead(sw4);
    digitalWrite(velocidad, HIGH);  // This LOW to HIGH change is what creates the
    delayMicroseconds(100);  // Regula la velocidad, cuanto mas bajo mas velocidad.
    digitalWrite(velocidad, LOW); // al A4988 de avanzar una vez por cada pulso de energia.
    delayMicroseconds(100);  // Regula la velocidad, cuanto mas bajo mas velocidad.
  }
  //  Serial.print("full-Back");
}


// the loop routine runs over and over again forever:
void loop() {
  if (go >= 1) {
    if (snt == 0) {
      Serial.print("2");
      snt=1;
    }
    String str = Serial.readString();
    int nst = str.toInt();
    float rest;
    s2 = digitalRead(sw2);
    if (paro == 0 and (nst == 1 or not(s2))) {
      lA = BloqueM500g(0);    // CAMBIAR ESTA LINEA POR LA SECUENCIA QUE SE QUIERA lA="secuencia deseada"     !!! 1 !!!
      go = 0;
      pas = 0;
    }
    /*
      Serial.println(go);
      Serial.print(del - Min);
      Serial.print("-");
      Serial.print(del);
      Serial.print("-");
      Serial.println(Min);
    */
  }
  /*

    else if (pas < lA) {
    BloqueM5k(pas);
    go=0;
    com = true;
    } else {
    com = false;
    }
  */
  if (paro == 1 and listo == 1) {
    String str = Serial.readString();
    int nst = str.toInt();
    if (nst == 2) {
      paro = 0;
      listo = 0;
    } else {
      paro = 1;
      listo = 1;
    }
  }
  if (del < Max and tick < stpm and paro == 0 and listo ==0) {
    tick++;
    digitalWrite(velocidad, HIGH);  // This LOW to HIGH change is what creates the
    delayMicroseconds(del);  // Regula la velocidad, cuanto mas bajo mas velocidad.
    digitalWrite(velocidad, LOW); // al A4988 de avanzar una vez por cada pulso de energia.
    delayMicroseconds(del);  // Regula la velocidad, cuanto mas bajo mas velocidad.
  }
}

void secuencia(float avance, int aceleracion, float velocida, float corte) {


  float rest;
  rest = abs((dist * 10) * fcd) + 480;
  if (dist < 0) {
    digitalWrite(direccion, true);
  } else {
    digitalWrite(direccion, false);
  }
  if (listo == 0) {
    dist = avance;
    Min = velocida;
    stp = aceleracion;
    stpm = (long)rest;
    tick = 0;

    if (crte == 1) {
      Serial.println("1");
      paro = 1;
      listo = 1;
    }
    crte = corte;
  }
  if (paro == 0) {
    //    Serial.print(stpm);
    //    Serial.print("-");
    //    Serial.print(tick);
  }
}

void control() {
  s1 = digitalRead(sw1);
  s2 = digitalRead(sw2);
  s3 = digitalRead(sw3);
  s4 = digitalRead(sw4);
  //int stp = 10; // mas valor para mas aceleracion
  if (paro == 1) {
    if (s2 == 0) {
      paro = 0;
    }
  }
  if (go < 1) {
    if (pas < lA) {
      if (stpm > tick) {
        if (not(s3) and dist < 0) {
          stpm = tick;
          digitalWrite(enable, LOW);
          //          Serial.println("finalA");
        } else if (not(s4)&dist > 0) {
          stpm = tick;
          digitalWrite(enable, LOW);
          //          Serial.println("finalB");
        } else {
          digitalWrite(enable, HIGH);
        }

        if (del > Min) {
          if (stp < (del - Min)) {
            del = del - stp;
          } else {
            del = Min;
          }
        } else {
          del = Min;
        }

      } else {
        k = 0;
        pas++;

        digitalWrite(enable, LOW);
        del = Max + stp;
        //        Serial.println(pas);
        //        Serial.println("+++");
        lA = BloqueM500g(pas);          // CAMBIAR ESTA LINEA POR LA SECUENCIA QUE SE QUIERA lA="secuencia deseada"     !!! 1 !!!

      }
    } else {
      go = go + 1;
      snt=0;
      //      Serial.print(go);
      //      Serial.println("--");
    }

  }
}



// -------
//// SECUENCIAS DISPONIBLES
// -------
int palitos11c(int paso) {
  float  avan[] = {110, 110, 110, 110, -130,0, 120, -800};
  float  acel[] = {20, 20, 20, 20, 20, 0, 20, 100};
  float  velo[] = {60, 60, 60, 60, 60,  0, 60,  80};
  float  cort[] = { 1, 1, 1, 1, 0, 0,  0, 0};
  int  movs = 8;
  if (avan[paso] == 0) {
    //Serial.print("pausa");
    paro = 1;
  } else {
    secuencia(avan[paso], acel[paso], velo[paso], cort[paso]);
  }
  return movs;
}
int BloqueM500g(int paso) {
  float  avan[] = {56, 56, 56, 56, 56, -130, 0, 56, 56, 56, 56, 56, 56, -800};
  float  acel[] = {20, 20, 20, 20, 20, 100, 0, 20, 20, 20, 20, 20, 20, 100};
  float  velo[] = {70, 70, 70, 70, 70, 70, 0, 70, 70, 70, 70, 70, 70, 80};
  float  cort[] = { 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0};
  int  movs = 14;
  if (avan[paso] == 0) {
    //Serial.print("pausa");
    paro = 1;
  } else {
    secuencia(avan[paso], acel[paso], velo[paso], cort[paso]);
  }
  return movs;
}
int BloqueM5k(int paso) {
  float  avan[] = {500, -130, 0, 200, -800};
  float  acel[] = {20, 100, 5000, 100 , 100};
  float  velo[] = {60, 60, 0, 70, 80};
  float  cort[] = { 0, 0 , 0 , 0 , 0};
  int  movs = 5;
  if (avan[paso] == 0) {
    //Serial.print("pausa");
    paro = 1;
  } else {
    secuencia(avan[paso], acel[paso], velo[paso], cort[paso]);
  }
  return movs;
}
int BloqueM25k(int paso) {
  float  avan[] = {295, -130, 0, 270, -800};
  float  acel[] = {20, 20, 10, 20 , 20};
  float  velo[] = {60, 70, 0, 70, 70};
  float  cort[] = { 1, 0, 0, 0, 0};
  int  movs = 5;
  if (avan[paso] == 0) {
    //Serial.print("pausa");
    paro = 1;
  } else {
    secuencia(avan[paso], acel[paso], velo[paso], cort[paso]);
  }
  return movs;
}
int BloqueC(int paso) {
  float  avan[] = {300, 50, -800, 150};
  float  acel[] = {10,   10 , 10, 10};
  float  velo[] = {60, 100,  60, 60};
  float cort[] = {0, 0, 0, 0};
  int  movs = 4;
  if (avan[paso] == 0) {
    //Serial.print("pausa");
    paro = 1;
  } else {
    secuencia(avan[paso], acel[paso], velo[paso], cort[paso]);
  }
  return movs;
}
int Corte(int paso) {
  float  avan[] = {150, -150};
  float  acel[] = {10 , 10};
  float  velo[] = {60,  60};
  float cort[] = {0, 0, 0};
  int  movs = 3;
  if (avan[paso] == 0) {
    //Serial.print("pausa");
    paro = 1;
  } else {
    secuencia(avan[paso], acel[paso], velo[paso], cort[paso]);
  }
  return movs;
}
