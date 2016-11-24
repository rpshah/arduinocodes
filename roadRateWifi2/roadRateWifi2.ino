//AttendanceSystem with RFID_READER,WIFI_MODULE_esp8266,RGB_LED,ARDUINO
/**
 *@author RAJ_SHAH
 *@author imec studios aka lab55
*/

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
#define SSID "Dhruv"              //SSID of your router/network
#define PASS "xyz54321"             //PASSWORD to connectwith it.
#define DEST_HOST "iot.rpshah96.me"            //IP address or URL of website/server You wants to connect
#define DEST_IP "128.199.80.33"

SoftwareSerial wifi( _rxpin, _txpin ); // RX, TX
uchar serNumA[5];

uchar fifobytes;
uchar fifoValue;

//For RGB Leds
const int redLed = A1;
const int greenLed = A2;
const int blueLed = 13;

//gasSensor MQ3's Analodinput Pin 
int gasIn = A0;

String GET = "GET /roadRate/testing/writeFile.php?xaxis=234&yaxis=-87&zaxis=23&file=ceptRoad.txt";     //FilePath to which you wants to send data through GET REQUEST

void setup() {
  // put your setup code here, to run once:
  pinMode(redLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(blueLed, OUTPUT);

   pinMode(A0,INPUT);

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
  int val;
  val = analogRead(gasIn);
  Serial.println(val,DEC);
  updateDatabase(val);
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
void updateDatabase( int gasValue )
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
  //cmd = GET + "?&data0=" + gasValue + " HTTP/1.1\r\nHOST: " + DEST_HOST + ":80\r\n\r\n";
  
  cmd = GET + " HTTP/1.1\r\nHOST: " + DEST_HOST + ":80\r\n\r\n";
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

