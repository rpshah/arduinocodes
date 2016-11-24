char dataBuf[1000];

/////  GYROSCOPE   ////
#include <Wire.h>


#include <SoftwareSerial.h>
////   ULTRASONIC   /////

// defines pins numbers
const int trigPin = 8;
const int echoPin = 9;
// defines variables
long duration;
int distance;

/////  LDR ///////
int LDR = A0;     //analog pin to which LDR is connected, here we set it to 0 so it means A0
int LDRValue = 0;      //that’s a variable to store LDR values


///// GAS SENSOR ////
const int GASInPin = A1;
int GASValue = 0;


///////////      BLUETOOTH   //////////
// arduino>>bluetooth
// D11   >>>  Rx
// D10   >>>  Tx
SoftwareSerial Genotronex(12, 13); // RX, TX
int BluetoothData; // the data given from Computer


///////////      MOTOR DRIVER   /////
int ENA = 10; //Connect on Arduino, Pin 2
int IN1 = 3; //Connect on Arduino, Pin 3
int IN2 = 4; //Connect on Arduino, Pin 4
int ENB = 5; //Connect on Arduino, Pin 5
int IN3 = 6; //Connect on Arduino, Pin 6
int IN4 = 7; //Connect on Arduino, Pin 7

void driveForward() {
  digitalWrite(IN1, LOW); //Forward
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

}

void driveStop() {
  digitalWrite(IN1, HIGH); //Forward
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, HIGH);

}

void setup() {

  //Serial.begin(9600); // Starts the serial communication
  Wire.begin();

  
  // ULTRASONIC
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input



    
  //////    BLUETOOTH    /////
  Genotronex.begin(9600);


  /////    MOTOR DRIVER  ///
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  digitalWrite(ENA, HIGH); // Activer moteur A
  digitalWrite(ENB, HIGH); // Activer moteur B
  driveForward();
  delay(1000);
  driveStop();

}

void loop() {
  // put your main code here, to run repeatedly:
  //if (Genotronex.available()) {
    //BluetoothData = Genotronex.read();
    //if (BluetoothData == '1') { // if number 1 pressed ....
      driveForward();
      delay(200);
      driveStop();
    //}
    //else{
     // return;
    //}
  //}
  //else{
    //return;
    //}
    ////  ULTRASONIC      ///////////////////////////
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

    // Prints the distance on the Serial Monitor
    Serial.print("Distance: ");
    Serial.println(distance);
    sprintf(dataBuf , "%d," , distance);
    Genotronex.print(dataBuf);


    /////  LDR   ////
    LDRValue = analogRead(LDR);      //reads the ldr’s value through LDR
    Serial.println(LDRValue);       //prints the LDR values to serial monitor
    delay(200);        //This is the speed by which LDR sends value to arduino
    sprintf(dataBuf , "%d," , LDRValue);
    Genotronex.print(dataBuf);


    ///// GAS SENSOR  //////
    GASValue = analogRead(GASInPin);
    Serial.println("Gas value:   ");
    Serial.println(GASValue);
    sprintf(dataBuf , "%d," , GASValue);
    Genotronex.print(dataBuf);


    /////   GYROSCOPE    ////
   
    Genotronex.println("");
  }
