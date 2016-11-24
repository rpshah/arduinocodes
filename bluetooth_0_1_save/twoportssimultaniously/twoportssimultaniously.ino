#include<SoftwareSerial.h>

SoftwareSerial portOne(10,11);
SoftwareSerial portTwo(8,9);

void setup(){
  Serial.begin(9600);
  portOne.begin(4800);
  portTwo.begin(4800);
}

void loop(){
  portOne.listen();

  if(portOne.isListening()){
    Serial.println("PORT ONE IS LISTENING");    
  }
  else{
    Serial.println("PORT ONE NOT LISTENING");
  }
  if(portTwo.isListening()){
    Serial.println("PORT TWO IS LISTENING");    
  }
  else{
    Serial.println("PORT TWO NOT LISTENING");
  }
}

