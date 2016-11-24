// <a href="https://nurdspace.nl/ESP8266" rel="nofollow"> https://nurdspace.nl/ESP8266
//</a>//http://www.instructables.com/id/Using-the-ESP8266-module/
//https://www.zybuluo.com/kfihihc/note/31135
//http://tminusarduino.blogspot.nl/2014/09/experimenting-with-esp8266-5-wifi-module.html
//http://www.cse.dmu.ac.uk/~sexton/ESP8266/
//https://github.com/aabella/ESP8266-Arduino-library/blob/master/ESP8266abella/ESP8266aabella.h
//http://contractorwolf.com/esp8266-wifi-arduino-micro/
//**********************************************************
#include <SoftwareSerial.h>
int sensor_temp = A0;
int value_temp;
int sensor_light = A1;
int value_light;
int sensor_humid = A2;
int value_humid;


#define DEBUG FALSE //comment out to remove debug msgs

//*-- Hardware Serial
#define _baudrate 115200

//*-- Software Serial
//
#define _rxpin      10
#define _txpin      11
SoftwareSerial debug( _rxpin, _txpin ); // RX, TX

//*-- IoT Information
#define SSID "tshah56"
#define PASS "N!$$123t"
#define IP "184.106.153.149" // ThingSpeak IP Address: 184.106.153.149

// GET /update?key=[THINGSPEAK_KEY]&field1=[data 1]&field2=[data 2]...;
String GET = "GET /update?key=[ThingSpeak_(Write)API_KEY]";

void setup() {
   Serial.begin( _baudrate );
  debug.begin( _baudrate );

  sendDebug("AT");
  delay(5000);
  if(debug.find("OK"))
  {
    Serial.println("RECEIVED: OK\nData ready to sent!");
    connectWiFi();
  }

}

void loop() {
  value_temp = analogRead(sensor_temp);
  value_light = analogRead(sensor_light);
  value_humid = analogRead(sensor_humid);
  String temp =String(value_temp);// turn integer to string
  String light= String(value_light);// turn integer to string
  String humid=String(value_humid);// turn integer to string
  updateTS(temp,light, humid);
  delay(3000); //
}
//----- update the  Thingspeak string with 3 values
void updateTS( String T, String L , String H)
{
  // ESP8266 Client
  String cmd = "AT+CIPSTART=\"TCP\",\"";// Setup TCP connection
  cmd += IP;
  cmd += "\",80";
  sendDebug(cmd);
  delay(2000);
  if( debug.find( "Error" ) )
  {
    Serial.print( "RECEIVED: Error\nExit1" );
    return;
  }

  cmd = GET + "&field1=" + T +"&field2="+ L + "&field3=" + H +"\r\n";
  debug.print( "AT+CIPSEND=" );
  debug.println( cmd.length() );
  if(debug.find( ">" ) )
  {
    Serial.print(">");
    Serial.print(cmd);
    debug.print(cmd);
  }
  else
  {
    sendDebug( "AT+CIPCLOSE" );//close TCP connection
  }
  if( debug.find("OK") )
  {
    Serial.println( "RECEIVED: OK" );
  }
  else
  {
    Serial.println( "RECEIVED: Error\nExit2" );
  }
}

void sendDebug(String cmd)
{
  Serial.print("SEND: ");
  Serial.println(cmd);
  debug.println(cmd);
}

boolean connectWiFi()
{
  debug.println("AT+CWMODE=1");//WiFi STA mode - if '3' it is both client and AP
  delay(2000);
  //Connect to Router with AT+CWJAP="SSID","Password";
  // Check if connected with AT+CWJAP?
  String cmd="AT+CWJAP=\""; // Join accespoint
  cmd+=SSID;
  cmd+="\",\"";
  cmd+=PASS;
  cmd+="\"";
  sendDebug(cmd);
  delay(5000);
  if(debug.find("OK"))
  {
    Serial.println("RECEIVED: OK");
    return true;
  }
  else
  {
    Serial.println("RECEIVED: Error");
    return false;
  }

  cmd = "AT+CIPMUX=0";// Set Single connection
  sendDebug( cmd );
  if( debug.find( "Error") )
  {
    Serial.print( "RECEIVED: Error" );
    return false;
  }
}

