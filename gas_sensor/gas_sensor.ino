#include <LiquidCrystal.h>
LiquidCrystal lcd(8,9,4,5,6,7);
const int analogInPin = A1;
int sensorValue = 0;
void setup() {
  lcd.begin(16, 2);
  lcd.setCursor(0,1);
//  lcd.write("testing 1 2 3 ");
}
void loop() {
sensorValue = analogRead(analogInPin);        
 lcd.setCursor(0,0);
 lcd.write("Gas value:   ");
 lcd.print(sensorValue);
}
