#define trigPin 9
#define echoPin 10

void setup() {
  Serial.begin (9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  // put your setup code here, to run once:

}

void loop() {
   long duration, distance;
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1;
 

  if (50<=distance <= 400) {
  digitalWrite(3, HIGH);
  digitalWrite(4, LOW);
   analogWrite(2, 20);
   digitalWrite(5, HIGH);
  digitalWrite(6, LOW);
   analogWrite(7, 20);
  }
 else{
  digitalWrite(3, HIGH);
  digitalWrite(4, LOW);
   analogWrite(2, 20);
   digitalWrite(5, LOW);
  digitalWrite(6, LOW);
   analogWrite(7, 20);
    
 }
   
   
  // put your main code here, to run repeatedly:

}
