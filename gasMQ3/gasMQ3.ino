int gasIn = A0;

void setup() {
  // put your setup code here, to run once:
  pinMode(A0,INPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  int val;
  val = analogRead(gasIn);
  Serial.println(val,DEC);
  delay(200);
}
