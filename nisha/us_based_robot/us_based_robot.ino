
void setup() {
   pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
   
  
  // put your setup code here, to run once:

}

void loop() {
  
  digitalWrite(3, HIGH);
  digitalWrite(4, LOW);
  analogWrite(2,60);
  digitalWrite(5,HIGH);
  digitalWrite(6, LOW);
  analogWrite(7,60);
  delay(1000);
  digitalWrite(4, HIGH);
  digitalWrite(3, LOW);
  analogWrite(2,60);
  digitalWrite(6,HIGH);
  digitalWrite(5, LOW);
  analogWrite(7,60);
  
  
 }
  // put your main code here, to run repeatedly:

