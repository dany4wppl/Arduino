//#include <Keyboard.h>

// -- keyboard definition --
//#define kb1 A0
//#define kb2 A1
//#define kb3 A2
//#define kb4 A3
//int v1, v2, v3, v4;
// -- keyboard definition


// -- LCD display definition --
/*
  The circuit:
 * LCD RS pin to digital pin 43
 * LCD Enable pin to digital pin 45
 * LCD D4 pin to digital pin 47
 * LCD D5 pin to digital pin 49
 * LCD D6 pin to digital pin 51
 * LCD D7 pin to digital pin 53
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)
 * pin 15 V LED (backlight)
 * pin 16 GNC LED (backlight)
*/

// keypad
#include <Keypad.h>

const byte rows = 4; //four rows
const byte cols = 4; //three columns
char keys[rows][cols] = {
  {'1','2','3','A'}, 
  {'4','5','6','B'}, 
  {'7','8','9','C'}, 
  {'*','0','#','D'} 
};
byte rowPins[rows] = {38, 40, 42, 44}; //connect to the row pinouts of the keypad
byte colPins[cols] = {46, 48, 50, 52}; //connect to the column pinouts of the keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, rows, cols );

// include the library code:
#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connectedyy to
const int rs = 43, en = 45, d4 = 47, d5 = 49, d6 = 51, d7 = 53;
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

#define step1 2
#define step2 3
#define step3 4
#define step4 5

const int stepsPerRevolution = 512;  // change this to fit the number of steps per revolution
// initialize the stepper library on pins 8 through 11:
//pinMode(A4, OUTPUT);8, 10, 9, A4 => 2 3 4 5
Stepper myStepper(stepsPerRevolution, step1, step3, step2, step4);
// -- stepper motor definition --

// -- distance sensor/buzzer --
#define trig 35
#define echo 37
#define buzz 41
//-- distance sensor/buzzer --

// pin responsible for power for LCD higlihgting, LED strip, stepper 0motor
#define power 39

// RFID - card reader --
/* Typical pin layout used:
 * -----------------------------------------------------------------------------------------
 *             MFRC522      Arduino       Arduino   Arduino    Arduino          Arduino
 *             Reader/PCD   Uno/101       Mega      Nano v3    Leonardo/Micro   Pro Micro
 * Signal      Pin          Pin           Pin       Pin        Pin              Pin
 * -----------------------------------------------------------------------------------------
 * RST/Reset   RST          9             5         D9         RESET/ICSP-5     RST
 * SPI SS      SDA(SS)      10            53        D10        10               10
 * SPI MOSI    MOSI         11 / ICSP-4   51        D11        ICSP-4           16
 * SPI MISO    MISO         12 / ICSP-1   50        D12        ICSP-1           14
 * SPI SCK     SCK          13 / ICSP-3   52        D13        ICSP-3           15
 */

#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN         9          // Configurable, see typical pin layout above
#define SS_PIN          10         // Configurable, see typical pin layout above

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

// RFID - card reader --


// global variables definition
boolean SerialDebug = true;
unsigned long Timer1 ; //dont give it a value.

void setup() {

  pinMode(power, OUTPUT);
  PowerUp();

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
  digitalWrite(step1, LOW);
  digitalWrite(step2, LOW);
  digitalWrite(step3, LOW);
  digitalWrite(step4, LOW);
  
  // -- distance sensor/buzzer initialization
  pinMode(buzz, OUTPUT);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  digitalWrite(buzz, LOW);
  tone (buzz, 440,500);
  if (SerialDebug==true) {Serial.println("Buzzer initialized");}
  if (SerialDebug==true) {lcd.print("Buzzer initialized");delay(1000);lcd.clear();}
  if (SerialDebug==true) {lcd.print("PowerDown");}
  delay(1000);
  // prepare LCD in waiting state
  PowerDown();

  SPI.begin();      // Init SPI bus
  mfrc522.PCD_Init();   // Init MFRC522
  mfrc522.PCD_DumpVersionToSerial();  // Show details of PCD - MFRC522 Card Reader details
  if (SerialDebug==true) Serial.println(F("RFID initialized"));

}

// global variables definition
int IleKnoppersowZaladowano = 0;
boolean inUse=false;
boolean SomeoneInDistance;
//int waitTimeForSleep = 20; //iterations
int timeoutSec = 10;
//int currentWaitTimeForSleep=0;
int distanceInfo = 120;
int distanceStart = 60;


void loop() {
  // waiting for wakeup signal from distance sensor
  while (inUse==false){
    PowerDown();
    GoSleep();
    inUse = IsInUse();
    delay(250);  
    if (SerialDebug==true) {Serial.println("Wewnatrz while (inUse==false)" + String(inUse));};
  }
  if (SerialDebug==true) {Serial.println("z loop - czy w uzyciu?: " + String(inUse));};
    
  // start screen after wake up
  if (inUse==true) {PowerUp(); WakeUp();}
  //delay(500); 
  
  String key="";

  while (key==""){
    key = KyeboardCheck(false);

    // check if still in use
    
    inUse = IsInUse();
    if (inUse==false)
    {
      PowerDown();
      GoSleep();
      return;
    }
    
    if (SerialDebug==true) {Serial.println("z petli KyeboardCheck - czy w uzyciu?: " + String(inUse));};

    delay(100); 
  }

  if (key!="") {
    if (SerialDebug==true) {Serial.println(key);};
    lcd.print(key);
    Timer1 = millis(); //user kliknal wiec resetujemy timeout
  }
  delay(100);

  if (key=="A") {
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
void PowerDown(){
  digitalWrite(power, HIGH);
  Serial.print("Power z PowerDown()=" + String(digitalRead(power)));
}

// goodbye message
void GoSleep(){
  lcd.clear();
  lcd.print("----------------");
  lcd.setCursor(0, 1);
  lcd.print("---Wylaczony!---");
  Serial.print("Power z GoSleep()=" + String(digitalRead(power)));
}

// switch on display, ilumination, etc
void PowerUp(){
  digitalWrite(power, LOW);
  Serial.print("Power z PowerUp()=" + String(digitalRead(power)));
}

// welcome message
void WakeUp(){
  lcd.clear();
  lcd.print("Zapraszamy!  ");
  lcd.setCursor(0, 1);
  lcd.print("Twoj wybor: ");
  Serial.print("Power z WakeUp()=" + String(digitalRead(power)));
}



// base of distance sensor check if someone is close to automat
boolean IsInUse()
{
  if (inUse==false)
  {
    SomeoneInDistance = checkDistance(distanceInfo, distanceStart);
    if (SomeoneInDistance==true)
    {
      Timer1 = millis();
      return true;
    }
    else
      return false;
  }
  else
  {
    if (IsExpired()==true)
    {
      return false;
    }
    else 
      return true;
  }
}

boolean IsExpired()
{
  if (millis() - Timer1 > timeoutSec*1000)
    return true;
  else
   return false;
}


boolean checkDistance(int distanceInfo, int distanceStart)
{
  int czas, dist;
  digitalWrite(trig, HIGH);
  delayMicroseconds(1000);
  digitalWrite(trig, LOW);
  czas = pulseIn(echo, HIGH);
  dist = (czas/2)/29.1;
  if (SerialDebug==true) {Serial.println("Distance: " + String(dist));};

    // detect user (info) or enable automat (start)
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
    digitalWrite(step1, LOW);
    digitalWrite(step2, LOW);
    digitalWrite(step3, LOW);
    digitalWrite(step4, LOW);
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

  char key = keypad.getKey();
  
  if (SerialDebug==true)
  {
    Serial.println("Nacisniety klawisz: ");
    Serial.println(key);
    Serial.println("-------------------");
  }

  return String(key);
}

