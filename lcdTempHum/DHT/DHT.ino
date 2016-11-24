// DHT code from:
// Example testing sketch for various DHT humidity/temperature sensors
// Written by ladyada, public domain
//#include "LiquidCrystal.h"
#include "DHT.h"

#define DHTPIN 8     // what pin we're connected to

// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11 
// #define DHTTYPE DHT22   // DHT 22  (AM2302)
//#define DHTTYPE DHT21   // DHT 21 (AM2301)
DHT dht(DHTPIN, DHTTYPE);

// LCD
// LCD RS=3, EN=4, DS4=5, DS5=6, DS6=7, DS7=8

//LiquidCrystal lcd(12, 11, 5, 4, 3, 2);



void setup() {
  Serial.begin(9600);
  // set up the LCD's number of columns and rows: 
  //analogWrite(9,7000);  //brightness
  //analogWrite(6,20);  //contrast
  //lcd.begin(16,2);
  dht.begin();
}

void loop() {
  
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  int h = dht.readHumidity();
  int t = dht.readTemperature();

    // set the cursor to (0,0):
  //lcd.setCursor(0, 0);
  // print from 0 to 9:
  /*
   lcd.print("Temp: ");
   lcd.print(t);
   lcd.print("C");
  // set the cursor to (16,1):
  lcd.setCursor(0,1);
  lcd.print("Humidity: ");
  lcd.print(h);
  lcd.print("%");
    */
    delay(200);
  Serial.print("Temp: ");
   Serial.print(t);
   Serial.println("C");
  // set the cursor to (16,1):
  //lcd.setCursor(0,1);
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.println("%");
    delay(2000);
}

