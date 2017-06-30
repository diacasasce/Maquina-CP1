
unsigned long prev = 0;

const long interval = 100;
const long inc = 1;
long del = 200;
int rd = 0;
// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(10, OUTPUT);
  Serial.begin(9600);
  rd = digitalRead(10);
}

// the loop function runs over and over again forever
void loop() {
  unsigned long curr = millis();
  if (curr - prev >= interval) {
    prev = curr;
    del -= inc;
    if (del <= 0) {
      del = 0;
    }
    Serial.print(" pulso: ");
    Serial.println(del);
  }

  digitalWrite(11, LOW);
  digitalWrite(12, HIGH);   // turn the LED on (HIGH is the voltage level)
  delayMicroseconds(del);                       // wait for a second
  digitalWrite(12, LOW);
  digitalWrite(11, HIGH);    // turn the LED off by making the voltage LOW
  delayMicroseconds(del);

}
