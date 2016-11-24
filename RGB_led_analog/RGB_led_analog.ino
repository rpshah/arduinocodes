int redLed = A1;
int greenLed = A2;
int blueLed = A3;


void setup() {
  // put your setup code here, to run once:
  pinMode(redLed,OUTPUT);
  pinMode(greenLed,OUTPUT);
  pinMode(blueLed,OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  analogWrite(blueLed,0);
  analogWrite(redLed,255);
  delay(1000);
  
  analogWrite(redLed,0);
  analogWrite(greenLed,255);
  delay(1000);
  
  analogWrite(greenLed,0);
  analogWrite(blueLed,255);
  delay(1000);
}
