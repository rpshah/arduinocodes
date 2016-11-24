const int TrigPin = 2;

const int EchoPin = 3;

float dist;
int RELAY1 = 12;
int LED1 = 13;

void setup() {

  Serial.begin(9600);

  pinMode(TrigPin, OUTPUT_FAST);
  pinMode(EchoPin, INPUT_FAST);
  
  pinMode(RELAY1, OUTPUT);
  pinMode(LED1, OUTPUT);

}

void loop()
{
  fastDigitalWrite(TrigPin, HIGH);

  delayMicroseconds(10);

  fastDigitalWrite(TrigPin, LOW);

  dist = pulseIn(EchoPin, HIGH) / 58.0;

 

  Serial.print(dist);

  Serial.println(" cm");

  delay(100);
  
  if (dist < 15) {
    digitalWrite(RELAY1, HIGH);
    digitalWrite(LED1, HIGH);
    delay(2000);
    digitalWrite(RELAY1, LOW);
    digitalWrite(LED1, LOW);


  }

  delay(100);
}

