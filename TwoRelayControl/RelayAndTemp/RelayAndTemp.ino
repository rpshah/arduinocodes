#include "DHT.h"
#define DHTPIN 8     // what pin we're connected to

// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11 
// #define DHTTYPE DHT22   // DHT 22  (AM2302)
//#define DHTTYPE DHT21   // DHT 21 (AM2301)
DHT dht(DHTPIN, DHTTYPE);

int RELAY1 = 9;          //AC
int RELAY2 = 10;          //Heater
int delayValue = 2500;
int R1=0;
int R2=0;

void setup() {
  // put your setup code here, to run once:
  pinMode(RELAY1, OUTPUT);
  pinMode(RELAY2, OUTPUT);

  digitalWrite(RELAY1, LOW);
  R1=0;
  digitalWrite(RELAY2, LOW);
  R2=0;
  Serial.println("Initially Ac and Heater are Off...");
  Serial.begin(9600);
  dht.begin();
}

void loop() {

  int h = dht.readHumidity();
  int t = dht.readTemperature();
  Serial.print("Temp = ");
  Serial.println(t);
  Serial.print("Humidity = ");
  Serial.println(h);
  // put your main code here, to run repeatedly:
  if(t>35&&R1!=1)
  {
    digitalWrite(RELAY1, HIGH);
    R1=1;
    digitalWrite(RELAY2, LOW);
    R2=0;
    Serial.println("AC ON,Heater Off");
  }
  else if(t<15&&R2!=1)
  {
    digitalWrite(RELAY2,HIGH);
    R2=1;
    digitalWrite(RELAY1,LOW);
    Serial.println("AC Off,Heater On");  
  }
  else if(t<35&&t>15)
  {
    if(R2==1)
    {
      digitalWrite(RELAY2,LOW);
      R2=0;
      Serial.println("Heater Off");
    }
    if(R1==1)
    {
      digitalWrite(RELAY1,LOW);
      R1=0;
      Serial.println("AC Off");
    }
  }
  delay(delayValue);
}
