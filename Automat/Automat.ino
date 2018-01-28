// -- keyboard definition --
#define kb1 A0
#define kb2 A1
#define kb3 A2
#define kb4 A3
int v1, v2, v3, v4;
// -- keyboard definition


// -- LCD display definition --
/*
  The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 13
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)
 * pin 15 V LED (backlight)
 * pin 16 GNC LED (backlight)
*/

// include the library code:
#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connectedyy to
const int rs = 12, en = 13, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
//LiquidCrystal lcd(12, 13, 5, 4, 3, 2);
// -- LCD display definition --


// -- stepper motor definition --
/* stepper
 Stepper Motor Control - one revolution
 This program drives a unipolar or bipolar stepper motor.
 The motor is attached to digital pins 8 - 11 of the Arduino.
 The motor should revolve one revolution in one direction, then
 one revolution in the other direction.
 */

// include the Stepper library code:
#include <Stepper.h>

const int stepsPerRevolution = 512;  // change this to fit the number of steps per revolution
// initialize the stepper library on pins 8 through 11:
Stepper myStepper(stepsPerRevolution, 8, 10, 9, 11);
// -- stepper motor definition --

// -- distance sensor/buzzer --
#define trig 6
#define echo 7
#define buzz A5
//-- distance sensor/buzzer --

#define power A4

// global variables definition
boolean SerialDebug = true;

void setup() {

  pinMode(power, OUTPUT);
  WakeUp();

  //start serial communication
  if (SerialDebug==true) {Serial.begin(9600);}

  // LCD initialization
  lcd.begin(16, 2);
  // Print a message to the LCD.
  if (SerialDebug==true) {Serial.println("LCD initialized");}
  if (SerialDebug==true) {lcd.print("LCD initialized");delay(1000);lcd.clear();}

  // Steper motor initialization - set the speed at 60 rpm:
  myStepper.setSpeed(60);
  if (SerialDebug==true) {Serial.println("Stepper motor initialized");}
  if (SerialDebug==true) {myStepper.step(256);delay(300);myStepper.step(-256);}
  if (SerialDebug==true) {lcd.print("Stepper init.");delay(1000);lcd.clear();}
  digitalWrite(8, LOW);
  digitalWrite(9, LOW);
  digitalWrite(10, LOW);
  digitalWrite(11, LOW);
  
  // -- distance sensor/buzzer initialization
  pinMode(buzz, OUTPUT);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  digitalWrite(buzz, LOW);
  tone (buzz, 440,200);
  if (SerialDebug==true) {Serial.println("Buzzer initialized");}
  if (SerialDebug==true) {lcd.print("Buzzer initialized");delay(1000);lcd.clear();}

  // prepare LCD in waiting state
  GoSleep();

}

// global variables definition
int IleKnoppersowZaladowano = 0;
boolean wUzyciu=false;
int waitTimeForSleep = 20; //iterations
int currentWaitTimeForSleep=0;
int distanceInfo = 120;
int distanceStart = 60;


void loop() {
  // waiting for wakeup signal from distance sensor
  while (wUzyciu==false){
    wUzyciu = CzyKtosKorzysta(distanceInfo, distanceStart, wUzyciu);
    delay(500); 
  }
  if (SerialDebug==true) {Serial.println("z loop - czy w uzyciu?: " + String(wUzyciu));};
    
  // start screen after wake up
  WakeUp();
  
  String key="";

  while (key==""){
    key = KyeboardCheck(false);

    // check if still in use
    
      wUzyciu = CzyKtosKorzysta(distanceInfo, distanceStart, wUzyciu);
      if (SerialDebug==true) {Serial.println("z petli KyeboardCheck - czy w uzyciu?: " + String(wUzyciu));};

      if (wUzyciu==false){
        currentWaitTimeForSleep++; //increase waittime
      } else
        currentWaitTimeForSleep=0; // reset waittime

      if (SerialDebug==true) {Serial.println("currentWaitTimeForSleep?: " + String(currentWaitTimeForSleep));};
        
      if (currentWaitTimeForSleep > waitTimeForSleep){
          if (SerialDebug==true) {Serial.println("GoSleep()");};
          GoSleep();
          return;
      }
     
    delay(250); 
  }

  if (key!="") {
    if (SerialDebug==true) {Serial.println(key);};
    lcd.print(key);
  }
  delay(250);

  if (key=="16") {
    UzupelnienieTowaru();  
  } else if (key=="1") {
    lcd.setCursor(0, 0);
    lcd.print("Twoj wybor: " + key);
    // Czy towar dostepny?
    if (IleKnoppersowZaladowano<=0) {
      KoniecTowaru(key);
      return;
    }
    // Pobiez oplate
    lcd.setCursor(0, 1);
    lcd.print("Dzis za darmo :)");
    
    // Wydaj produkt "1"
    WydajTowar(key);
    // Zmniejsz ilos dostepnych    
    IleKnoppersowZaladowano--;

  } else
  {
    KoniecTowaru(key);
  }
  
}

// switch off display, ilumination, etc, to save energy
void GoSleep(){
  digitalWrite(power, LOW);
  lcd.clear();
  lcd.print("----------------");
  lcd.setCursor(0, 1);
  lcd.print("---Wylaczony!---");
}

// switch on display, ilumination, etc
void WakeUp(){
  digitalWrite(power, HIGH);
  lcd.clear();
  lcd.print("Zapraszamy!  ");
  lcd.setCursor(0, 1);
  lcd.print("Twoj wybor: ");
}

// base of distance sensor check if someone is close to automat
boolean CzyKtosKorzysta(int distanceInfo, int distanceStart, boolean wUzyciu){

  int czas, dist;
  digitalWrite(trig, HIGH);
  delayMicroseconds(1000);
  digitalWrite(trig, LOW);
  czas = pulseIn(echo, HIGH);
  dist = (czas/2)/29.1;
  if (SerialDebug==true) {Serial.println("Distance: " + String(dist));};

  // if not in use detect user (info) or enable automat (start)
  if (wUzyciu==false) {
    if (dist > 0 && dist <= distanceStart)
    {
      tone (buzz, 1000,100);
      delay(200);
      tone (buzz, 2000,100);
      delay(200);
      tone (buzz, 3000,100);
      return true;    
    }
    // just beep to get user attention
    if (dist > distanceStart && dist <= distanceInfo)
    {
      tone (buzz, 2000,100);
      delay(200);
      tone (buzz, 2000,100);
      delay(200);
      tone (buzz, 2000,100);
    }
    return false;
  } else // function called with wUzyciu=true, to detect if user left
  {
    if (dist > distanceInfo)
    {
      /// user left - distance > distanceInfo
      return false;
    }
    else 
      return true;
  }
}

void UzupelnienieTowaru(){
      // -- uzupelnienie towaru
    lcd.setCursor(0, 0);
    lcd.print("Uzupelnianie");
    lcd.setCursor(0, 1);
    lcd.print("Ile towaru [1]?");
    
    String key="";
    while (key==""){
      key = KyeboardCheck(false);
      delay(250); 
    }
  
    if (key!="") {
      if (SerialDebug==true) {Serial.println(key);};
      lcd.print(key);
      IleKnoppersowZaladowano = key.toInt();
    }
    delay(2000);

}

// function for run particular stepper/servo (in the future)
void WydajTowar (String Towar){
    myStepper.step(2048);
    digitalWrite(8, LOW);
    digitalWrite(9, LOW);
    digitalWrite(10, LOW);
    digitalWrite(11, LOW);
}

// Out od order
void KoniecTowaru(String Towar){
    lcd.setCursor(0, 0);
    lcd.print("Przykro nam...");
    lcd.setCursor(0, 1);
    lcd.print("Skonczylo sie :(");
    delay(3000);
}

// chek if any key was pressed on keyboard
String KyeboardCheck(boolean SerialDebug)
{
  String key="";
  
  v1 = analogRead(kb1);
  v2 = analogRead(kb2);
  v3 = analogRead(kb3);
  v4 = analogRead(kb4);

  //---- row 1 --------
  if (v4>150 && v4<300) {
    key="1";
  } 
  if (v4>=300 && v4<550) {
    key="2";
  } 
  if (v4>=550 && v4<800) {
    key="3";
  } 
  if (v4>=960) {
    key="4";
  } 
  //---- row 2 --------  
  if (v3>150 && v3<300) {
    key="5";
  } 
  if (v3>=300 && v3<550) {
    key="6";
  } 
  if (v3>=550 && v3<800) {
    key="7";
  } 
  if (v3>=960) {
    key="8";
  } 
  //---- row 3 --------  
  if (v2>150 && v2<300) {
    key="9";
  } 
  if (v2>=300 && v2<550) {
    key="10";
  } 
  if (v2>=550 && v2<800) {
    key="11";
  } 
  if (v2>=960) {
    key="12";
  } 
  //---- row 4 --------  
  if (v1>150 && v1<300) {
    key="13";
  } 
  if (v1>=300 && v1<550) {
    key="14";
  } 
  if (v1>=550 && v1<800) {
    key="15";
  } 
  if (v1>=960) {
    key="16";
  } 

  if (SerialDebug==true)
  {
    // send the values from A0-A3 to the Serial Monitor
    Serial.println("Pin A0 = " + String(v1));
    Serial.println("Pin A1 = " + String(v2));
    Serial.println("Pin A2 = " + String(v3));
    Serial.println("Pin A3 = " + String(v4));
    Serial.println("Nacisniety klawisz: ");
    Serial.println(key);
    Serial.println("-------------------");
  }

  return key;
}
