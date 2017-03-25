
// defines pins numbers
const int trigPin = 3;
const int echoPin = 4;

//LEDS
int LED1 = 13;
int LED2 = 12;
int LED3 = 11;
int LED4 = 10;
int LED5 = 9;

// defines variables
long duration;
int distance;

const boolean DEBUG = true;

void setup() {
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(LED5, OUTPUT);
  Serial.begin(9600); // Starts the serial communication
}

void loop() {
  int d = findDistance();
  showStatus(d);
  delay(100);
}

int findDistance(){
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);

  // Calculating the distance
  distance = duration * 0.034 / 2;

  if(DEBUG){
    // Prints the distance on the Serial Monitor
    Serial.print("Distance: ");
    Serial.println(distance);
  }
  
  return distance;
}

void showStatus(int dist){
  if(dist>50 && dist<2000){
    digitalWrite(LED5,LOW);
    digitalWrite(LED4,LOW);
    digitalWrite(LED3,LOW);
    digitalWrite(LED2,LOW);
    digitalWrite(LED1,LOW);
  }
  else if(dist>40 && dist<=50){
    digitalWrite(LED5,HIGH);
    digitalWrite(LED4,LOW);
    digitalWrite(LED3,LOW);
    digitalWrite(LED2,LOW);
    digitalWrite(LED1,LOW);  
  }  
  else if(dist>30 && dist<=40){
    digitalWrite(LED5,HIGH);
    digitalWrite(LED4,HIGH);
    digitalWrite(LED3,LOW);
    digitalWrite(LED2,LOW);
    digitalWrite(LED1,LOW);  
  }  
  else if(dist>20 && dist<=30){
    digitalWrite(LED5,HIGH);
    digitalWrite(LED4,HIGH);
    digitalWrite(LED3,HIGH);
    digitalWrite(LED2,LOW);
    digitalWrite(LED1,LOW);  
  }
  else if(dist>10 && dist<=20){
    digitalWrite(LED5,HIGH);
    digitalWrite(LED4,HIGH);
    digitalWrite(LED3,HIGH);
    digitalWrite(LED2,HIGH);
    digitalWrite(LED1,LOW);  
  }
  
  else if(dist>0 && dist<=10){
    digitalWrite(LED5,HIGH);
    digitalWrite(LED4,HIGH);
    digitalWrite(LED3,HIGH);
    digitalWrite(LED2,HIGH);
    digitalWrite(LED1,HIGH);
    if(DEBUG){
      Serial.println("Dustbin is full,no more space.");
    }  
  }
  else{
    if(DEBUG){
      Serial.println("Too High or Zero Value,Check for any errors");
    }  
  }
}
