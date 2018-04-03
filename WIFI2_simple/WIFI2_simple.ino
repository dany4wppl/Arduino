#include <SoftwareSerial.h>
//SoftwareSerial ESPserial(2, 3); // RX | TX

void setup() 
{
   Serial.begin(115200);     // communication with the host computer
   while (!Serial)   { ; }
   // Start the software serial for communication with the ESP8266
   Serial1.begin(115200);  
   Serial.println("");
   Serial.println("Remember to to set Both NL & CR in the serial monitor.");
   Serial.println("Ready");
   Serial.println("");    
}
void loop() 
{
   // listen for communication from the ESP8266 and then write it to the serial monitor
   if ( Serial1.available() )
  {  
   //Serial.println( "Serial1 WIFI.available()" ); 
   Serial.write( Serial1.read() ); 
  }
  // listen for user input and send it to the ESP8266
   if ( Serial.available() )      
  {  
   Serial1.write( Serial.read() ); 
   //Serial.println( "Poszlo do WIFI" ); 
  }
}
