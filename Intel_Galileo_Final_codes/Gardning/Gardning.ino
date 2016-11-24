#include<LiquidCrystal.h>

#include "DHT.h"

#define DHTIN 2     // what pin we're connected to
#define DHTOUT 3

// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11 
//#define DHTTYPE DHT22   // DHT 22  (AM2302)
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Connect pin 1 (on the left) of the sensor to +5V
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

DHT dht(DHTIN,DHTOUT, DHTTYPE);

int moiPin = A5;    
int moiValue = 0;  // variable to store the value coming from the sensor

int waterSupplyPin = 13;

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(8,9,4,5,6,7);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  dht.begin();
  // set up the LCD's number of columns and rows:
  lcd.init(1, 8, 255, 9, 4, 5, 6, 7, 0, 0, 0, 0);
  // different from arduino code!
  lcd.begin(16, 2);
  lcd.print("Wait a minute!");

  pinMode(waterSupplyPin ,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  
  // Wait a few seconds between measurements.
  delay(1000);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit
  float f = dht.readTemperature(true);
  
  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Compute heat index
  // Must send in temp in Fahrenheit!
  float hi = dht.computeHeatIndex(f, h);

  Serial.print("Humidity: "); 
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: "); 
  Serial.print(t);
  Serial.print(" *C ");
  Serial.print(f);
  Serial.print(" *F\t");
  Serial.print("Heat index: ");
  Serial.print(hi);
  Serial.println(" *F");

  moiValue = analogRead(moiPin);
  moiValue = 1024 -moiValue;
  Serial.println("Moisture : "+moiValue);

  lcd.setCursor(0, 0);
  // different from arduino code!
  lcd.print("                ");
  lcd.setCursor(0,0);
  lcd.print("Temp :");
  lcd.print(t);
  lcd.print(" Cel");
  // different from arduino code!
  // set the cursor to column 0, line 1 // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 1);
  lcd.print("                ");
  lcd.setCursor(0,1);
  lcd.print("Moisture : ");
  lcd.print(moiValue);

  if(moiValue < 900){
    digitalWrite(waterSupplyPin ,HIGH);
    }
   else{
    digitalWrite(waterSupplyPin ,LOW);
    }
}
