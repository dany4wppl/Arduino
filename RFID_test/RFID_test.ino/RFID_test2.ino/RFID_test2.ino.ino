/*
 * --------------------------------------------------------------------------------------------------------------------
 * Example sketch/program showing how to read data from a PICC to serial.
 * --------------------------------------------------------------------------------------------------------------------
 * This is a MFRC522 library example; for further details and other examples see: https://github.com/miguelbalboa/rfid
 * 
 * Example sketch/program showing how to read data from a PICC (that is: a RFID Tag or Card) using a MFRC522 based RFID
 * Reader on the Arduino SPI interface.
 * 
 * When the Arduino and the MFRC522 module are connected (see the pin layout below), load this sketch into Arduino IDE
 * then verify/compile and upload it. To see the output: use Tools, Serial Monitor of the IDE (hit Ctrl+Shft+M). When
 * you present a PICC (that is: a RFID Tag or Card) at reading distance of the MFRC522 Reader/PCD, the serial output
 * will show the ID/UID, type and any data blocks it can read. Note: you may see "Timeout in communication" messages
 * when removing the PICC from reading distance too early.
 * 
 * If your reader supports it, this sketch/program will read all the PICCs presented (that is: multiple tag reading).
 * So if you stack two or more PICCs on top of each other and present them to the reader, it will first output all
 * details of the first and then the next PICC. Note that this may take some time as all data blocks are dumped, so
 * keep the PICCs at reading distance until complete.
 * 
 * @license Released into the public domain.
 * 
 * Typical pin layout used:
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
#include <Servo.h>

#define RST_PIN         9          // Configurable, see typical pin layout above
#define SS_PIN          10         // Configurable, see typical pin layout above

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance
Servo servo1;

void setup() {
  Serial.begin(9600);   // Initialize serial communications with the PC
  while (!Serial);    // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
  SPI.begin();      // Init SPI bus
  mfrc522.PCD_Init();   // Init MFRC522
  mfrc522.PCD_DumpVersionToSerial();  // Show details of PCD - MFRC522 Card Reader details
  Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));
  servo1.attach(6);
  servo1.write(10);
}

String oldNumber="", newNumber="";
unsigned long Timer1 ; //dont give it a value.

void loop() {
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  newNumber = GetCardUID(mfrc522);
  if (newNumber!=oldNumber)
  {
    //Serial.println(newNumber);
    if (GetCardUIDSum(mfrc522) == 443) {
        bramka("Lila", 1);
      }

   else if (GetCardUIDSum(mfrc522) == 547) {
        bramka("Daniel", 1);
      }
    
   else if (GetCardUIDSum(mfrc522) == 505) {
        bramka("Alina", 2);
      }

   else {
        bramka("?", 3);
      };
      
      
    //Serial.println(GetCardUIDSum(mfrc522));

    oldNumber=newNumber;
    Timer1 = millis();
  }

  if (millis() - Timer1 > 2000) {oldNumber="";};

  
  // Dump debug info about the card; PICC_HaltA() is automatically called
  //mfrc522.PICC_DumpToSerial(&(mfrc522.uid));

}

void bramka (String kto, int wpusc )
{

  if (wpusc == 1) // rozpoznany dozwolony
  {
        Serial.println(kto + " - Witaj !!!");
        for (int i= 10; i < 90; i++)
        {
          servo1.write(i);
          delay (5);
        }
        
        delay(4000); //tyle czasu otwarte

        for (int i= 100; i > 10; i--)
        {
          servo1.write(i);
          delay(5);
        }
  }
  else if (wpusc == 2) // rozpoznany zabroniony
  {
        Serial.println(kto + " - Żegnaj :) !");
  }
  else if (wpusc == 3) // nierozpoznany zabroniony
  {
        Serial.println("Nie wiem kim jestes");
  }
  else
        Serial.println("????");
  
  
}

String GetCardUID(MFRC522 card){
  String temp="";
  for (byte i = 0; i < card.uid.size; i++) {
    if(card.uid.uidByte[i] < 0x10)
      temp = temp + (F(" 0"));
    else
      temp = temp + (F(" "));
      temp = temp + card.uid.uidByte[i];
  } 
  return temp;
}

int GetCardUIDSum(MFRC522 card){
  int temp=0;
  for (byte i = 0; i < card.uid.size; i++) {
      temp = temp + card.uid.uidByte[i];
  } 
  return temp;
}


