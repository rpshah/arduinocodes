// <a href="https://nurdspace.nl/ESP8266" rel="nofollow"> https://nurdspace.nl/ESP8266
//</a>//http://www.instructables.com/id/Using-the-ESP8266-module/
//https://www.zybuluo.com/kfihihc/note/31135
//http://tminusarduino.blogspot.nl/2014/09/experimenting-with-esp8266-5-wifi-module.html
//http://www.cse.dmu.ac.uk/~sexton/ESP8266/
//https://github.com/aabella/ESP8266-Arduino-library/blob/master/ESP8266abella/ESP8266aabella.h
//http://contractorwolf.com/esp8266-wifi-arduino-micro/
//**********************************************************
#include <SoftwareSerial.h>
#include "AddicoreRFID.h"
#include <SPI.h>

#define  uchar unsigned char
#define uint  unsigned int
#define DEBUG FALSE //comment out to remove debug msgs

//*-- Hardware Serial
#define _baudrate 115200

//*-- Software Serial
//
#define _rxpin      8
#define _txpin      9

//Maximum length of the array
#define MAX_LEN 16


//*-- IoT Information
#define SSID "My Asus"
#define PASS "qwertyuiop"
#define IP "192.168.43.3"

#define DEVICEID "2"
#define DEVICETKEN "qwerty"

//#define SSID "GTPL"
//#define PASS "swami321"
//#define IP "184.106.153.149" // ThingSpeak IP Address: 184.106.153.149
//#define IP "192.168.1.14"

SoftwareSerial wifi( _rxpin, _txpin ); // RX, TX
uchar serNumA[5];

uchar fifobytes;
uchar fifoValue;

AddicoreRFID myRFID; // create AddicoreRFID object to control the RFID module

/////////////////////////////////////////////////////////////////////
//set the pins
/////////////////////////////////////////////////////////////////////
const int chipSelectPin = 10;
const int NRSTPD = 5;

const int redLed = A1;
const int greenLed = A2;
const int blueLed = A3;

//const int NRSTPD = 2;



// GET /update?key=[THINGSPEAK_KEY]&field1=[data 1]&field2=[data 2]...;
//String GET = "GET /update?key=[ThingSpeak_(Write)API_KEY]";
String GET = "GET /usage/entry/"+DEVICEID;

void setup() {

  pinMode(redLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(blueLed, OUTPUT);



  //Serial.begin( _baudrate );
  wifi.begin( _baudrate );

  SPI.begin();

  pinMode(chipSelectPin, OUTPUT);             // Set digital pin 10 as OUTPUT to connect it to the RFID /ENABLE pin
  digitalWrite(chipSelectPin, LOW);         // Activate the RFID reader
  pinMode(NRSTPD, OUTPUT);                    // Set digital pin 10 , Not Reset and Power-down
  digitalWrite(NRSTPD, HIGH);

  myRFID.AddicoreRFID_Init();

  sendWifi("AT");
  delay(5000);
  //if (wifi.find("OK"))
  //{
  //Serial.println("RECEIVED: OK\nData ready to sent!");
  //Serial.println("Wiring of wifi is OK.");
  connectWiFi();
  //}

}

void loop() {

  uchar i, tmp, checksum1;
  uchar status;
  uchar str[MAX_LEN];
  uchar RC_size;
  uchar blockAddr;  //Selection operation block address 0 to 63
  String mynum = "";

  str[1] = 0x4400;
  //Find tags, return tag type
  status = myRFID.AddicoreRFID_Request(PICC_REQIDL, str);
  if (status == MI_OK)
  {
    Serial.println("RFID tag detected");
    Serial.print(str[0], BIN);
    Serial.print(" , ");
    Serial.print(str[1], BIN);
    Serial.println(" ");
  }

  //Anti-collision, return tag serial number 4 bytes
  status = myRFID.AddicoreRFID_Anticoll(str);
  if (status == MI_OK)
  {
    checksum1 = str[0] ^ str[1] ^ str[2] ^ str[3];
    Serial.println("The tag's number is  : ");
    Serial.print(2);
    Serial.print(str[0]);
    Serial.print(" , ");
    Serial.print(str[1]);
    Serial.print(" , ");
    Serial.print(str[2]);
    Serial.print(" , ");
    Serial.print(str[3]);
    Serial.print(" , ");
    Serial.print(str[4]);
    Serial.print(" , ");
    Serial.println(checksum1, BIN);
    Serial.println();
    blinkLed(blueLed , 500);
    updateRFID(str[0], str[1], str[2], str[3], str[4], checksum1);

    delay(1000);
  }
 updateRFID(str[0], str[1], str[2], str[3], str[4], checksum1);

  myRFID.AddicoreRFID_Halt();       //Command tag into hibernation

  //updateTS(temp, light, humid);
  delay(5000); //
}


//----- update the  Thingspeak string with 3 values
void updateRFID( uchar D0, uchar D1, uchar D2, uchar D3, uchar D4, uchar checksum)
{
  // ESP8266 Client
  String cmd = "AT+CIPSTART=\"TCP\",\"";// Setup TCP connection
  cmd += IP;
  cmd += "\",80";
  sendWifi(cmd);
  delay(2000);
  if ( !wifi.find( "OK" ) )
  {
    Serial.print( "RECEIVED: Error\nExit1" );
    //return;
  }

  cmd = GET + "?&latitude0=" + 10 + "&longitude=" + 20 + "&usage=" + 20 + "&quality=" + 7.8 + "&token=" + D4 + "&rfid_card_number=" + "2DB1AFD5" + "\r\n";
  wifi.print( "AT+CIPSEND=" );
  wifi.println( cmd.length() );
  if (wifi.find( ">" ) )
  {
    Serial.print(">");
    Serial.print(cmd);
    wifi.print(cmd);

    if ( wifi.find("OK") )
    {
      Serial.println( "RECEIVED: OK" );
    }
    else
    {
      Serial.println( "RECEIVED: Error\nExit2" );
    }

  }
  //else
  //{
  sendWifi( "AT+CIPCLOSE" );//close TCP connection
  //}
  delay(1000);
  if ( wifi.find("OK") )
  {
    Serial.println( "RECEIVED: OK" );
  }
  else
  {
    Serial.println( "RECEIVED: Error\nExit2" );
  }
  blinkLed(greenLed , 1000);
  Serial.println("Wait for 10 seconds");
  //delay(10000);
}

void sendWifi(String cmd)
{
  Serial.print("SEND: ");
  Serial.println(cmd);
  wifi.println(cmd);
}

boolean connectWiFi()
{
  wifi.println("AT+CWMODE=1");//WiFi STA mode - if '3' it is both client and AP
  delay(2000);
  //Connect to Router with AT+CWJAP="SSID","Password";
  // Check if connected with AT+CWJAP?
  String cmd = "AT+CWJAP=\""; // Join accespoint
  cmd += SSID;
  cmd += "\",\"";
  cmd += PASS;
  cmd += "\"";
  sendWifi(cmd);
  delay(5000);
  if (!wifi.find("OK"))
  {
    Serial.println("RECEIVED: Error");
    //return true;
  }
  else
  {
    Serial.println("returned value is untrackable");
    //return false;
  }

  cmd = "AT+CIPMUX=0";// Set Single connection
  sendWifi( cmd );
  if ( wifi.find( "Error") )
  {
    Serial.print( "RECEIVED: Error" );
    //return false;
  }
}


void blinkLed(int ledPin , int delayTime)
{
  analogWrite(redLed, 0);
  analogWrite(greenLed, 0);
  analogWrite(blueLed, 0);

  analogWrite(ledPin, 255);
  delay(delayTime);
  analogWrite(ledPin, 0);

}
