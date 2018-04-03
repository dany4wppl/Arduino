#include <Servo.h>

Servo servo_pin_6;

void setup()
{
  pinMode( 13 , OUTPUT);
  servo_pin_6.attach(6);
}

void loop()
{
  digitalWrite( 13 , HIGH );
  delay( 5000 );
  digitalWrite( 13 , LOW );
  servo_pin_6.write( 90 );
  delay( 5000 );
  servo_pin_6.write( 0 );
}


