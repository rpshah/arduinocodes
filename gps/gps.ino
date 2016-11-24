#include <SoftwareSerial.h>
#include <TinyGPS.h>
 
float flat;
float flon;// create variable for latitude and longitude object
 
SoftwareSerial gpsSerial(2, 3); // create gps sensor connection
TinyGPS gps; // create gps object
 
void setup(){
  Serial.begin(115200); // connect serial
  gpsSerial.begin(4800); // connect gps sensor
}
 
void loop(){
  while(gpsSerial.available()){ // check for gps data
   if(gps.encode(gpsSerial.read())){ // encode gps data
    gps.f_get_position(&flat,&flon); // get latitude and longitude
    // display position
    Serial.print("Position: ");
    Serial.print("lat: ");
    Serial.print(flat);
    Serial.print(" ");// print latitude
    Serial.print("lon: ");
    Serial.println(flon); // print longitude
   }
  }
}
