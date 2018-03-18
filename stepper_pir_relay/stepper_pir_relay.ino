#include <Stepper.h>

const int stepsPerRevolution = 360;  // number of steps per revolution
const int stepsPerHalfRevolution = 105;  // number of steps per revolution
const int rpmSpeed = 2; //use as rpm of stepper


// initialize the stepper library on pins as below:
Stepper myStepper1(stepsPerRevolution, 2, 3, 4, 5);
Stepper myStepper2(stepsPerRevolution, 6, 7, 8, 9);
Stepper myStepper3(stepsPerRevolution, 22, 23, 24, 25);
Stepper myStepper4(stepsPerRevolution, 28, 29, 30, 31);
Stepper myStepper5(stepsPerRevolution, 34, 35, 36, 37);
Stepper myStepper6(stepsPerRevolution, 40, 42, 41, 43);
Stepper myStepper7(stepsPerRevolution, 46, 48, 47, 49);
Stepper myStepper8(stepsPerRevolution, 50, 52, 51, 53);


const int pirPin1 = 10;               // choose the input pin (for PIR sensor)
const int pirPin2 = 11;
//const int pirPin3 = 12;
int pirState = LOW;             // we start, assuming no motion detected

const int relayPin = 13;
int relayState = HIGH;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Wait for 10sec to get device ready...");
  delay(10000);

  //Set RPM for steppers
  myStepper1.setSpeed(rpmSpeed);
  myStepper2.setSpeed(rpmSpeed);
  myStepper3.setSpeed(rpmSpeed);
  myStepper4.setSpeed(rpmSpeed);
  myStepper5.setSpeed(rpmSpeed);
  myStepper6.setSpeed(rpmSpeed);
  myStepper7.setSpeed(rpmSpeed);
  myStepper8.setSpeed(rpmSpeed);

  pinMode(pirPin1, INPUT);
  pinMode(pirPin2, INPUT);
  //pinMode(pirPin3, INPUT);
  pinMode(relayPin, OUTPUT);

  digitalWrite(relayPin,HIGH);

  Serial.println("We are ready now...");
}

void loop() {
  if(motionDetection() == true){
      Serial.println("Turning Relay ON");
      digitalWrite(relayPin,LOW); //Turn ON relay switch
      rotateMotors();
      Serial.println("Turning Relay OFF");
      digitalWrite(relayPin,HIGH); //Turn OFF relay switch

      //Serial.println("Now waiting for 10 seconds");
      //delay(10000);

  }
}

void rotateMotors(){

  Serial.println("Clockwise...");
  for(int i=0; i<stepsPerHalfRevolution; i++){
    myStepper1.step(1);
    myStepper2.step(1);
    myStepper3.step(1);
    myStepper4.step(1);
    myStepper5.step(1);
    myStepper6.step(1);
    myStepper7.step(1);
    myStepper8.step(1);
  }
  delay(5000);   //rotate till 15sec and do nothing for next 5sec
  
  Serial.println("AntiClockwise...");
  for(int i=0; i<stepsPerHalfRevolution; i++){
    myStepper1.step(-1);
    myStepper2.step(-1);
    myStepper3.step(-1);
    myStepper4.step(-1);
    myStepper5.step(-1);
    myStepper6.step(-1);
    myStepper7.step(-1);
    myStepper8.step(-1);
  }
  delay(5000);   //rotate in reverse for 15sec and do nothing for next 5sec
}

boolean motionDetection(){

  int val1 = digitalRead(pirPin1);  // read input value
  int val2 = digitalRead(pirPin2);
  //int val3 = digitalRead(pirPin3);
  if (val1 == HIGH || val2 == HIGH) {            // check if the input is HIGH
        
    if (pirState == LOW) {
      Serial.println("Motion detected!");
      pirState = HIGH;
      
      digitalWrite(pirPin1,LOW);
      digitalWrite(pirPin2,LOW);
      //digitalWrite(pirPin3,LOW);
      return true;
    }
    
    //Serial.println("trapped");
  } else {
      if (pirState == HIGH){
      Serial.println("Motion ended!");
      pirState = LOW;
    }
  }
  delay(100);
  return false;
  
}
