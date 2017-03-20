
#include <dht.h>

dht DHT;

#define DHT22_PIN 5

int gasIn = A0;
int alarm = 13;

int normalGas = 550;
int normalTemp = 35;
int normalHum = 20;

void setup()
{
    pinMode(gasIn,INPUT);
    pinMode(alarm,OUTPUT);
    Serial.begin(9600);
    Serial.println("dht22_test.ino");
    Serial.print("LIBRARY VERSION: ");
    Serial.println(DHT_LIB_VERSION);
    Serial.println();
    Serial.println("Type,\tstatus,\tHumidity (%),\tTemperature (C)\tTime (us)");
}

void loop()
{
    int gasVal;
    gasVal = analogRead(gasIn);
    Serial.print("CO2 : \t");
    Serial.print(gasVal,DEC);
    Serial.println("");
    // READ DATA
    Serial.print("DHT22, \t");

    uint32_t start = micros();
    int chk = DHT.read22(DHT22_PIN);
    uint32_t stop = micros();

    // DISPLAY DATA
    Serial.print(DHT.humidity, 1);
    Serial.print(",\t");
    Serial.print(DHT.temperature, 1);
    Serial.print(",\t");
    Serial.print(stop - start);
    Serial.println();


    if( (DHT.humidity <= normalHum && DHT.humidity != 0.0 ) || ( DHT.temperature >= normalTemp && DHT.temperature != 0.0) || gasVal >= normalGas){
      Serial.println("Fire! Fire! Fire!");
      digitalWrite(alarm,HIGH);  
    }
    else{
      digitalWrite(alarm,LOW);  
    }
    
    
    delay(2000);
}
//
// END OF FILE
//
