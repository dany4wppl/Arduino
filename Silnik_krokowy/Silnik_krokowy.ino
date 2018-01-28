  //original source is http://www.geeetech.com/wiki/index.php/Stepper_Motor_5V_4-Phase_5-Wire_%26_ULN2003_Driver_Board_for_Arduino
// This code is used with a video tutorial for RoboJax.com
// Published on March 27, 2017 from Aajx, ON, Canada.
// Updated on October 04, 2017 in Ajax, Ontrio, Canada for Robojax.com by Nejrabi

//If you want to run the motor up to certain time and then stop it, then this code will do that.
// Just change the "runtime" value to any value in meli second. For 3 second enter 3000, for 5.5 second enter 5500
///

int Pin1 = 10; 
int Pin2 = 11; 
int Pin3 = 12; 
int Pin4 = 13; 
int _step = 0; 
boolean dir = true;// false=clockwise, true=counter clockwise


int count=0;// counter for running time
int runTime = 3000;// the time motor will run


void setup() 
{ 
 Serial.begin(9600);
 pinMode(Pin1, OUTPUT);  
 pinMode(Pin1, OUTPUT);  
 pinMode(Pin2, OUTPUT);  
 pinMode(Pin4, OUTPUT);  
} 
 void loop() 
{ 

 // Serial.println(count);
  
 switch(_step){ 
   case 0: 
  if(count <runTime)
  {
     digitalWrite(Pin1, LOW);  
     digitalWrite(Pin2, LOW); 
     digitalWrite(Pin3, LOW); 
     digitalWrite(Pin4, HIGH); 
  }else{
    stop_motor();
  }
   break;  
   case 1: 
  if(count <runTime)
  {   
     digitalWrite(Pin1, LOW);  
     digitalWrite(Pin2, LOW); 
     digitalWrite(Pin3, HIGH); 
     digitalWrite(Pin4, HIGH); 
  }else{
    stop_motor();
  }  
   break;  
   case 2: 
  if(count <runTime)
  {   
     digitalWrite(Pin1, LOW);  
     digitalWrite(Pin2, LOW); 
     digitalWrite(Pin3, HIGH); 
     digitalWrite(Pin4, LOW); 
  }else{
    stop_motor();
  }  
   break;  
   case 3: 
  if(count <runTime)
  {   
     digitalWrite(Pin1, LOW);  
     digitalWrite(Pin2, HIGH); 
     digitalWrite(Pin3, HIGH); 
     digitalWrite(Pin4, LOW); 
  }else{
    stop_motor();
  }  
   break;  
   case 4: 
  if(count <runTime)
  {   
     digitalWrite(Pin1, LOW);  
     digitalWrite(Pin2, HIGH); 
     digitalWrite(Pin3, LOW); 
     digitalWrite(Pin4, LOW); 
  }else{
    stop_motor();
  }  
   break;  
   case 5: 
  if(count <runTime)
  {   
     digitalWrite(Pin1, HIGH);  
     digitalWrite(Pin2, HIGH); 
     digitalWrite(Pin3, LOW); 
     digitalWrite(Pin4, LOW); 
  }else{
    stop_motor();
  }  
   break;  
     case 6: 
  if(count <runTime)
  {  
     digitalWrite(Pin1, HIGH);  
     digitalWrite(Pin2, LOW); 
     digitalWrite(Pin3, LOW); 
     digitalWrite(Pin4, LOW); 
  }else{
    stop_motor();
  }  
   break;  
   case 7: 
  if(count <runTime)
  {   
     digitalWrite(Pin1, HIGH);  
     digitalWrite(Pin2, LOW); 
     digitalWrite(Pin3, LOW); 
     digitalWrite(Pin4, HIGH); 
  }else{
    stop_motor();
  }  
   break;  
   default: 
    stop_motor();// stop the motor
   break;  
 } 
 if(dir){ 
   _step++; 
 }else{ 
   _step--; 
 } 
 if(_step>7){ 
   _step=0; 
 } 
 if(_step<0){ 
   _step=7; 
 } 
 delay(1); 
 
// check if the counter has reached run time
if(count < runTime){
   count++;//increment count
}else{
   count =0;// set to zero
}

 
}// loop end


// this subroutine stops the motor
void stop_motor(){
     digitalWrite(Pin1, LOW);  
     digitalWrite(Pin2, LOW); 
     digitalWrite(Pin3, LOW); 
     digitalWrite(Pin4, LOW); 
}
