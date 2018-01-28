/*
  Blink

  Turns an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the UNO, MEGA and ZERO
  it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN is set to
  the correct LED pin independent of which board is used.
  If you want to know what pin the on-board LED is connected to on your Arduino
  model, check the Technical Specs of your board at:
  https://www.arduino.cc/en/Main/Products

  modified 8 May 2014
  by Scott Fitzgerald
  modified 2 Sep 2016
  by Arturo Guadalupi
  modified 8 Sep 2016
  by Colby Newman

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/Blink
*/

// deklaracje diod
int zolta1 = 7;
int czerwona1 = 6;
int zielona1 = 5;
int zolta2 = 4;
int czerwona2 = 3;
int zielona2 = 2;

int opoznienie = 200;

// the setup function runs once when you press reset or power the board
void setup() {

  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(zolta1, OUTPUT);
  pinMode(czerwona1, OUTPUT);
  pinMode(zielona1, OUTPUT);
  pinMode(zolta2, OUTPUT);
  pinMode(czerwona2, OUTPUT);
  pinMode(zielona2, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {


  
  digitalWrite(zielona2, LOW);    // turn the LED off by making the voltage LOW
  digitalWrite(zolta1, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(opoznienie);                       // wait for a second
  digitalWrite(czerwona1, HIGH);    // turn the LED off by making the voltage LOW
  digitalWrite(zolta1, LOW);   // turn the LED on (HIGH is the voltage level)
  delay(opoznienie);                       // wait for a second
  digitalWrite(zielona1, HIGH);    // turn the LED off by making the voltage LOW
  digitalWrite(czerwona1, LOW);    // turn the LED off by making the voltage LOW
  delay(opoznienie);                       // wait for a second
  digitalWrite(zielona1, LOW);    // turn the LED off by making the voltage LOW
  digitalWrite(zolta2, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(opoznienie);                       // wait for a second
  digitalWrite(czerwona2, HIGH);    // turn the LED off by making the voltage LOW
  digitalWrite(zolta2, LOW);   // turn the LED on (HIGH is the voltage level)
  delay(opoznienie);                       // wait for a second
  digitalWrite(zielona2, HIGH);    // turn the LED off by making the voltage LOW
  digitalWrite(czerwona2, LOW);    // turn the LED off by making the voltage LOW
  delay(opoznienie);                       // wait for a second
}
