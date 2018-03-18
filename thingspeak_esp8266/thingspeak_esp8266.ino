// Program: Sending data to thingspeak channel using AT commands on esp8266
// Date:    March 2018
// Author:  Raj Shah "rpshah_96@yahoo.com"

#include <SoftwareSerial.h>
#include <LiquidCrystal.h> // include the library code for lcd
#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 6

//*-- Hardware Serial
#define _baudrate 115200

//*-- Software Serial
//
#define _rxpin      8
#define _txpin      9

//Maximum length of the array
#define MAX_LEN 16


//*-- Network Connectivity Information
#define SSID "Network SSID"
#define PASS "Network PASSWORD"
#define IP "184.106.153.149"

//*-- Sensor Connectivity Info
int echopin = 9; // echo pin
int trigpin = 8; // Trigger pin
int maximumRange = 50;
long duration, distance;
float temperature;

const int switchPin = 6;
int switchState = 0;
int condVal;

SoftwareSerial wifi( _rxpin, _txpin ); // RX, TX
LiquidCrystal lcd(12, 11, 5, 4, 3, 2); //
// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

// GET /update?key=[THINGSPEAK_KEY]&field1=[data 1]&field2=[data 2]...;
String GET = "GET /update?key=[ThingSpeak_(Write)API_KEY]";
bool debugging = true; //to show debug msgs or not

void setup() {

  pinMode (trigpin, OUTPUT);
  pinMode (echopin, INPUT );
  pinMode (4, OUTPUT);
  pinMode (13, OUTPUT);
  pinMode(switchPin, INPUT); //sets analog 6 as an input pin

  Serial.begin( 9600 );
  wifi.begin( _baudrate );
  lcd.begin(16, 2);
  sensors.begin();

  sendWifi("AT");
  delay(5000);  //wait for 5 sec to bootup esp8266
  connectWiFi();
}

void loop() {
  readTemp();
  readDistance();
  if (distance <= 3 ) {
    digitalWrite (13, LOW); // connect to relay(motor)
    digitalWrite (7, LOW);
    lcd.setCursor(0, 1);
    lcd.print("Tank is Full");
  }
  else if (distance >= 9) {
    digitalWrite (7, HIGH); // connect to relay(motor)
    digitalWrite (13, HIGH);
    lcd.setCursor(0, 1);
    lcd.print("Motor Started");
  }
  else {
    lcd.setCursor(0, 1);
    lcd.print("Motor is running");
  }
  updateRemoteChannel(distance, temperature);
  delay(1000); // repeat after 1 more sec
}


//----- update the  Thingspeak string
void updateRemoteChannel(long water_level, float temperature)
{
  // ESP8266 Client
  String cmd = "AT+CIPSTART=\"TCP\",\"";// Setup TCP connection
  cmd += IP;
  cmd += "\",80";
  sendWifi(cmd);
  delay(2000);
  if ( !wifi.find( "OK" ) )
  {
    debugPrint( "RECEIVED: Error\nExit1", false);
  }
  cmd = GET + "?&field1=" + water_level + "&field2=" + temperature + "\r\n";
  wifi.print( "AT+CIPSEND=" );
  wifi.println( cmd.length() );
  if (wifi.find( ">" ) )
  {
    debugPrint(">", false);
    debugPrint(cmd, true);
    wifi.print(cmd);

    if ( wifi.find("OK") )
    {
      debugPrint( "RECEIVED: OK", true);
    }
    else
    {
      debugPrint( "RECEIVED: Error\nExit2", true);
    }

  }
  sendWifi( "AT+CIPCLOSE" );//close TCP connection
  delay(1000);
  if ( wifi.find("OK") )
  {
    debugPrint( "RECEIVED: OK", true);
  }
  else
  {
    debugPrint( "RECEIVED: Error\nExit2", true);
  }
}

void sendWifi(String cmd)
{
  debugPrint("SEND: ", false);
  debugPrint(cmd, true);
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
    debugPrint("RECEIVED: Error", true);
  }
  else
  {
    debugPrint("returned value is untrackable", true);
  }

  cmd = "AT+CIPMUX=0";// Set Single connection
  sendWifi( cmd );
  if ( wifi.find( "Error") )
  {
    debugPrint( "RECEIVED: Error", true);
  }
}


void debugPrint(String str, bool nl)
{
  if (!debugging) return;
  if (nl) Serial.println(str);
  else Serial.print(str);
}

void readDistance()
{
  digitalWrite(trigpin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigpin, HIGH);
  delayMicroseconds(10);

  duration = pulseIn (echopin, HIGH);
  distance = duration / 58.2;
  delay (50);
  debugPrint(String(distance), true);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("water level :");
  lcd.print(distance);
  delay(0);
}

void readTemp()
{
  sensors.requestTemperatures();
  temperature = sensors.getTempCByIndex(0); // Why "byIndex"? You can have more than one IC on the same bus. 0 refers to the first IC on the wire
  debugPrint("Temperature  ", false);
  debugPrint(String(temperature), true);

  condVal = analogRead(A0);
  float voltage = condVal * (5.0 / 1023.0); //calculation of relative conductivity
  debugPrint("EC", false);
  debugPrint(String(voltage), false);
  debugPrint("  ", true);

}
