#include <Servo.h>

Servo myservo;  // create servo object to control a servo
int servoPin = 2;

//const int rainsensorMin = 0;     // sensor minimum
//const int rainsensorMax = 1024;  // sensor maximum
const int rainsensorPin = A0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  myservo.attach(servoPin);
}

void loop() {
  // put your main code here, to run repeatedly:
  int rainsensorReading = analogRead(rainsensorPin);
  Serial.println(rainsensorReading);

  if (rainsensorReading < 900 ) {
    myservo.write(179);
    delay(2000);

  }
  else {
    myservo.write(0);
    delay(2000);

  }
}
