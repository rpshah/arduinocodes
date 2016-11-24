#include<SoftwareSerial.h>

SoftwareSerial b1(10,11);   //RX, TX

void setup() {
  // put your setup code here, to run once:
  b1.begin(9600);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  while(b1.available()){
      Serial.print(b1.read());
    }
   Serial.println(" ");

   b1.print("This is Arduibo 1.");
}
