#include <Servo.h>

Servo servoX;
Servo servoY;

int joyX = 0;
int joyY = 1;

int joyVal;

void setup() {
  // put your setup code here, to run once:

  servoX.attach(2);
  servoY.attach(3);
}

void loop() {
  // put your main code here, to run repeatedly:

  joyVal = analogRead(joyX);
  joyVal = map(joyVal, 0, 1023, 0, 180);
  servoX.write(joyVal);
  
  joyVal = analogRead(joyY);
  joyVal = map(joyVal, 0, 1023, 0 , 180);
  servoY.write(joyVal);
  delay(15);
}  
