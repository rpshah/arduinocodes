int ledPin=13;

void setup()
{
  //Serial.begin(9600);
  pinMode(ledPin,OUTPUT);
}

void loop()
{
  digitalWrite(ledPin,HIGH);
  //Serial.println("HIGH");
  delay(1000);
  digitalWrite(ledPin,LOW);
  //Serial.println("LOW");
  delay(1000);
}


