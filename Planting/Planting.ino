int ledPin=13;
int analogPin=A1;

void setup()
{
  Serial.begin(9600);
  pinMode(ledPin,OUTPUT);
  //  pinMode(ledPin,INPUT);
  pinMode(analogPin,INPUT);
}

void loop()
{
  static int count;
  int readedValue=analogRead(analogPin);
  float voltage=float(readedValue*(float(5.0/1023.0)));        //To Count Voltage
  if(readedValue>750)
  {
    count++;
    if(count<6)
    {
      Serial.print("Thank You!...");
      Serial.println(readedValue);
      Serial.print("\tVoltage....");
      Serial.println(voltage);
    }
    else if(count==6)
    {
      Serial.println("Plant has been Watered...");
    }
    //if(digitalRead(ledPin)==HIGH)
    //{
    digitalWrite(ledPin,HIGH);
    delay(readedValue*10);
    digitalWrite(ledPin,LOW);
    delay(readedValue);
    //}
  }
  else
  {
    count=0;
    Serial.println("********************");
    Serial.print("    FEED ME MORE ...   ");
    Serial.println(readedValue);
    Serial.print("\tVoltage....");
    Serial.println(voltage);
    //    if(digitalRead(ledPin)==LOW)
    //{
    //  delay(readedValue*10);
    if(readedValue<200)
    {
      readedValue=200;
    }
    digitalWrite(ledPin,LOW);
    delay(readedValue);
    digitalWrite(ledPin,HIGH);
    delay(readedValue);
    //digitalWrite(ledPin,LOW);
    //delay(readedValue*10);
    //}
  }

}





