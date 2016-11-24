
int MQ3 = A0; 
int MQ8 = A1; 
int ledPin = 13;      // select the pin for the LED
int MQ3Value = 0;
int MQ8Value = 0;

void setup() {
  // declare the ledPin as an OUTPUT:
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // read the value from the sensor:
  MQ3Value = analogRead(MQ3);
  Serial.println(MQ3Value);
  MQ8Value = analogRead(MQ8);
  Serial.println(MQ8Value);
  
  delay(1000);
  // turn the ledPin on
  
}
