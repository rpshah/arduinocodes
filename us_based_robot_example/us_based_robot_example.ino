int ENA=2; //Connect on Arduino, Pin 2
int IN1=3; //Connect on Arduino, Pin 3
int IN2=4; //Connect on Arduino, Pin 4
int ENB=5; //Connect on Arduino, Pin 5
int IN3=6; //Connect on Arduino, Pin 6
int IN4=7; //Connect on Arduino, Pin 7
 
//Set up the ultrasonic
#define echoPin 12 // Echo Pin
#define trigPin 11 // Trigger Pin
 
long duration, distance; // Duration used to calculate distance
 
void setup() {
 
 //Serial.begin (9600); //used serial monitor to see if ping was working
 pinMode(ENA,OUTPUT);
 pinMode(ENB,OUTPUT);
 pinMode(IN1,OUTPUT);
 pinMode(IN2,OUTPUT);
 pinMode(IN3,OUTPUT);
 pinMode(IN4,OUTPUT);
 
 digitalWrite(ENA,HIGH);// Activer moteur A
 digitalWrite(ENB,HIGH);// Activer moteur B
 
 pinMode(trigPin, OUTPUT);
 pinMode(echoPin, INPUT);
 
}
void loop(){
digitalWrite(trigPin, LOW);
 delayMicroseconds(2);
digitalWrite(trigPin, HIGH);
 delayMicroseconds(10);
 
 digitalWrite(trigPin, LOW);
 duration = pulseIn(echoPin, HIGH);
 
 //Calculate the distance (in cm) based on the speed of sound.
 distance = duration/58.2;
 
 if (distance <= 30){
   
 
 digitalWrite(IN1,LOW); //Forward
 digitalWrite(IN2,HIGH);
 digitalWrite(IN3,HIGH);
 digitalWrite(IN4,LOW);
 
 }
 
}
