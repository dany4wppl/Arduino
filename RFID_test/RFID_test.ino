#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9
MFRC522 rfid(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;

void setup() {
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();
}

void loop() {
  if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial())
  {
    Serial.print("UID: {");
    Serial.print(rfid.uid.uidByte[0] < 0x10 ? "0x0" : "0x");
    Serial.print(rfid.uid.uidByte[0], HEX);
    Serial.print(rfid.uid.uidByte[1] < 0x10 ? ", 0x0" : ", 0x");
    Serial.print(rfid.uid.uidByte[1], HEX);
    Serial.print(rfid.uid.uidByte[2] < 0x10 ? ", 0x0" : ", 0x");
    Serial.print(rfid.uid.uidByte[2], HEX);
    Serial.print(rfid.uid.uidByte[3] < 0x10 ? ", 0x0" : ", 0x");
    Serial.print(rfid.uid.uidByte[3], HEX);
    Serial.println("}");
    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();
  }
}

