
/**
 *@author RAJ_SHAH
 *@author imec studios aka lab55
*/

/**
 * ultra ---  arduino Uno
 * vcc        5v
 * gnd        gnd
 * echo       2
 * trig       3
 * 
 *esp8266 --- Arduino Uno
 * vcc        3.3V
 * gnd        gnd
 * tx         12
 * rx         13
 * rst        3.3V
 */


#include <LiquidCrystal.h>        //Load Liquid Crystal Library
#include <SoftwareSerial.h>       //wifi is communicating througth serialPorts

#define  uchar unsigned char      //To reduce programming time only
#define uint  unsigned int
#define DEBUG FALSE               //comment out to remove debug msgs

//*-- Hardware Serial
#define _baudrate 115200           //mey differ for your wifi Module____mine is esp8266

//*-- Software Serial
#define _rxpin      12
#define _txpin      13

//Maximum length of the array
#define MAX_LEN 16


//*-- IoT Information
#define SSID "LAB55"              //SSID of your router/network
#define PASS "abc54321"             //PASSWORD to connectwith it.
#define DEST_HOST "www.storywith.in"            //IP address or URL of website/server You wants to connect
#define DEST_IP "50.31.138.216"

SoftwareSerial wifi( _rxpin, _txpin ); // RX, TX
uchar serNumA[5];

uchar fifobytes;
uchar fifoValue;

//For RGB Leds
const int redLed = A1;
const int greenLed = A2;
const int blueLed = A3;

const int buttonPin = A5;     // the number of the pushbutton pin
int buttonState = 0;         // variable for reading the pushbutton status

String GET = "GET /ultraInsert.php";     //FilePath to which you wants to send data through GET REQUEST

//LCD + Ultra
LiquidCrystal LCD(8, 9, 4, 5, 6, 7); //Create Liquid Crystal Object called LCD

int trigPin = 2; //Sensor Trip pin connected to Arduino pin 13
int echoPin = 3; //Sensor Echo pin connected to Arduino pin 11
int myCounter = 0; //declare your variable myCounter and set to 0
int servoControlPin = 6; //Servo control line is connected to pin 6
float pingTime;  //time for ping to travel from sensor to target and return
float targetDistance; //Distance to Target in inches
float speedOfSound = 776.5; //Speed of sound in miles per hour when temp is 77 degrees.


void setup() {
  // put your setup code here, to run once:
  pinMode(redLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(blueLed, OUTPUT);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

// initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);
  
  LCD.begin(16, 2); //Tell Arduino to start your 16 column 2 row LCD
  LCD.setCursor(0, 0); //Set LCD cursor to upper left corner, column 0, row 0
  LCD.print("Target Distance:");  //Print Message on First Row

  Serial.begin( _baudrate );       // comment means debugging through SerialMonitor is off.
  wifi.begin( _baudrate );            //Start wifi


  //Start Connection with the wifi
AT:
  analogWrite(blueLed, 255);             //MAKE Blue led glow to indicate power suply.
  sendWifi("AT\r\n");
  delay(5000);                                //5 Sec. Delay
  if (wifi.find("OK"))
  {
    Serial.println("RECEIVED: OK\nData ready to sent!");
    Serial.println("Wiring of wifi is OK.");

    connectWiFi();

  }
  else
  {
    blinkLed(redLed, 2000);               //2 Sec. Delay
    goto AT;
  }
}

void loop() {

  // put your main code here, to run repeatedly:
  //Lcd + Ultra
  digitalWrite(trigPin, LOW); //Set trigger pin low
  delayMicroseconds(2000); //Let signal settle
  digitalWrite(trigPin, HIGH); //Set trigPin high
  delayMicroseconds(15); //Delay in high state
  digitalWrite(trigPin, LOW); //ping has now been sent
  delayMicroseconds(10); //Delay in high state
  
  pingTime = pulseIn(echoPin, HIGH);  //pingTime is presented in microceconds
  pingTime=pingTime/1000000; //convert pingTime to seconds by dividing by 1000000 (microseconds in a second)
  pingTime=pingTime/3600; //convert pingtime to hourse by dividing by 3600 (seconds in an hour)
  targetDistance= speedOfSound * pingTime;  //This will be in miles, since speed of sound was miles per hour
  targetDistance=targetDistance/2; //Remember ping travels to target and back from target, so you must divide by 2 for actual target distance.
  targetDistance= targetDistance*63360;    //Convert miles to inches by multipling by 63360 (inches per mile)
  
  LCD.setCursor(0,1);  //Set cursor to first column of second row
  LCD.print("                "); //Print blanks to clear the row
  LCD.setCursor(0,1);   //Set Cursor again to first column of second row
  LCD.print(targetDistance); //Print measured distance
  LCD.print(" inches");  //Print your units.
  delay(250); //pause to let things settle
  
  buttonState = digitalRead(buttonPin);

  // check if the pushbutton is pressed.
  // if it is, the buttonState is HIGH:
  //if (buttonState == LOW) {
    
    updateDatabase(targetDistance);  
  
  //}
   
  delay(1000);
}


//Function to BLINK the LED
void blinkLed(int ledPin , int delayTime)
{
  analogWrite(redLed, 0);
  analogWrite(greenLed, 0);
  analogWrite(blueLed, 0);

  analogWrite(ledPin, 255);
  delay(delayTime);
  analogWrite(ledPin, 0);

}

//Function to Send AT commands to wifi
void sendWifi(String cmd)
{
  Serial.print("SEND: ");
  Serial.println(cmd);
  wifi.println(cmd);
}


//Function to connect to the network --> One TIME Progarmm
boolean connectWiFi()
{
  wifi.println("AT+CWMODE=1");          //WiFi STA mode - if '3' it is both client and AP
  delay(2000);                                            //2 Seconds Waiting
  //Connect to Router with AT+CWJAP="SSID","Password";
  // Check if connected with AT+CWJAP?
  String cmd = "AT+CWJAP=\"";           // Join accespoint
  cmd += SSID;
  cmd += "\",\"";
  cmd += PASS;
  cmd += "\"";
CWJAP:
  sendWifi(cmd);
  delay(5000);                                             //5 Seconds Waiting

  if (wifi.find("OK"))
  {
    Serial.println("RECIEVED: OK");
    goto CIPMUX;                                        //connection Established Go MAke SINGLE CONNECTION
    //return true;
  }
  else if (wifi.find("Error"))
  {
    Serial.println("RECEIVED: Error");
    blinkLed(redLed, 500);                            //1.5 Sec. delay(total).
    blinkLed(redLed, 500);
    blinkLed(redLed, 500);
    goto CWJAP;                                                //Found Error
    //return true;
  }
  else
  {
    Serial.println("returned value is untrackable");
    //goto CWJAP;
    blinkLed(blueLed, 500);                         //COULD NOT TRACK wifi's Replay
    //Ignoring replay and assuming connection Established   //1 Sec. Delay
    //return false;
  }
CIPMUX:
  cmd = "AT+CIPMUX=0";// Set Single connection
  sendWifi( cmd );
  if ( wifi.find( "Error") )
  {
    Serial.print( "RECEIVED: Error" );                //single connection failed.
    blinkLed(redLed, 500);                            //1 Sec. delay(total).
    blinkLed(redLed, 500);
    goto CIPMUX;
    //return false;
  }
  //**********else if( wifi.find("OK"))
  blinkLed(greenLed, 500);                            //1.5 Sec. delay(total).
  blinkLed(greenLed, 500);
  blinkLed(greenLed, 500);                            //Indicating ready for reading RFID cards
}

// SEND GET REQUEST(QUERY) to Perticulor URL
void updateDatabase( float distanceValue )
{
  // ESP8266 Client
  //AT+CIPSTART="TCP","server",80;            //Single TCP connection
  String cmd = "AT+CIPSTART=\"TCP\",\"";      // Setup TCP connection
  cmd += DEST_IP;
  cmd += "\",80";
  sendWifi(cmd);
  delay(2000);                                //2 Sec. Delay
  if ( wifi.find( "OK" ) )
  {
    Serial.print( "RECEIVED: OK" );
    blinkLed(greenLed, 250);               //0.25 Sec. Delay
    //connected to the SERVER
    //return;
  }
  else if ( wifi.find( "Error" ) )
  {
    Serial.print( "RECEIVED: Error\nExit1" );
    blinkLed(redLed, 250);                  //0.25 Sec. Delay

    //Please place the card again.
    return;
  }
  else
  {
    Serial.print( "Data Could Not be Retrieved" );
    blinkLed(blueLed, 250);               //0.25 Sec. Delay
    //return;
  }

  //  AT+CIPSEND=length
  //  >GET /databaseInsert.php?&field1=value1
  cmd = GET + "?&data0=" + distanceValue + " HTTP/1.1\r\nHOST: " + DEST_HOST + ":80\r\n\r\n";
  wifi.print( "AT+CIPSEND=" );
  wifi.println( cmd.length() );
  Serial.print( "AT+CIPSEND=" );
  Serial.println( cmd.length() );
  //Serial.print(wifi.read());
  if (wifi.find( ">" ) )
  {
    Serial.print(">");
    Serial.print(cmd);
    wifi.print(cmd);

    if ( wifi.find("OK") )
    {
      Serial.println( "RECEIVED: OK" );
    }
    else if ( wifi.find("Error") )
    {
      Serial.println( "RECEIVED: Error\nExit2" );
    }
    else
    {
      Serial.print( "Data Could Not be Retrieved" );
    }
    blinkLed(greenLed, 500);               //0.5 Sec. Delay
    //Attandance Taken
  }
  else
  {
    sendWifi( "AT+CIPCLOSE" );              //close TCP connection
    blinkLed(redLed, 500);                  //0.5 Sec. Delay
    //Attandance NOT Taken

    delay(1000);
    if ( wifi.find("OK") )
    {
      Serial.println( "RECEIVED: OK" );
    }
    else
    {
      Serial.println( "RECEIVED: Error\nExit2" );
    }
  }
  delay(1000);
}

