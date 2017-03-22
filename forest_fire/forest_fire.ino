#include "dht.h"

#define DHTPIN 2     // what digital pin we're connected to

#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321

DHT dht(DHTPIN, DHTTYPE);

int CO2_PIN = 5;
int alarm = 13;

int maxCO2 = 5000;
float maxTemp = 50;
float minHum = 12;

float hum = 0;
float temp = 0;

int value_CO2 = 0;
int count_CO2 = 0;

const boolean DEBUG = true;

void setup()
{
    pinMode(CO2_PIN,INPUT);
    pinMode(alarm,OUTPUT);
    Serial.begin(9600);
    
    dht.begin();
    
    if(DEBUG)
    {
      Serial.println("Program started");
      Serial.println("Type,\tstatus,\tHumidity (%),\tTemperature (C)\tTime (us)");  
    }
}

void loop()
{
    int co2 = CO2_sensor();
    DHT_sensor();
    if(( co2 >= maxCO2 ) && ( temp >= maxTemp || hum <=minHum ) ) 
    {
      if(DEBUG)
      {
        Serial.println("Fire! Fire! Fire!");  
      }
      digitalWrite(alarm,HIGH);  
    }
    else{
      digitalWrite(alarm,LOW);  
    }
    
    
    delay(2000);
}


int CO2_sensor()
{
  // Serial.println("get Co2");
  unsigned long Th = pulseIn(CO2_PIN, HIGH, 1004000);

  Th = Th / 1000;

  if (Th > 2)
  {
    unsigned long value = ((2000 * (Th - 2)) / (Th + (1004 - Th) - 4));
    value_CO2 += value;
    count_CO2 += 1;
  }

  if (DEBUG)
  {
    Serial.print("CO2 : ");
    Serial.print(value_CO2 / count_CO2);
    Serial.println("ppm     ");
  }

  return value_CO2 / count_CO2;
}

void DHT_sensor()
{
  // Serial.println("get Temperature,Humidity");
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if ( ( isnan(h) || isnan(t) || isnan(f) ) && DEBUG ) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  else{
    hum = h;
    temp = t;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);
    if(DEBUG)
    {
      // DISPLAY DATA

      Serial.print("Humidity: ");
      Serial.print(h);
      Serial.print(" %\t");
      Serial.print("Temperature: ");
      Serial.print(t);
      Serial.print(" *C ");
      Serial.print(f);
      Serial.print(" *F\t");
      Serial.print("Heat index: ");
      Serial.print(hic);
      Serial.print(" *C ");
      Serial.print(hif);
      Serial.println(" *F");

    }
}


