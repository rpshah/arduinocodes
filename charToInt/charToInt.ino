void setup() {
  // put your setup code here, to run once:
  char key = '2';
  char key2 = '3';

  int v1 = key-48;
  int v2 = key2-48;
  int value = (v1*10)+v2;

  Serial.begin(9600);
  Serial.println(value);
}

void loop() {
  // put your main code here, to run repeatedly:

}
