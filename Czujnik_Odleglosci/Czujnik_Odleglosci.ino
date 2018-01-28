#define trig 2
#define echo 3
int buzz=9;

void setup () {
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  Serial.begin(9600);
  digitalWrite(buzz, LOW);
  tone (buzz, 440,200);
}

void loop(){

  int czas, dist;
  digitalWrite(trig, HIGH);
  delayMicroseconds(1000);
  digitalWrite(trig, LOW);
  czas = pulseIn(echo, HIGH);
  dist = (czas/2)/29.1;
  
  Serial.println(dist);
  delay(500);  
  tone (buzz, 3000-10*dist,100);


}

