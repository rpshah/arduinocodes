/* SFE_BMP180 library example sketch

This sketch shows how to use the SFE_BMP180 library to read the
Bosch BMP180 barometric pressure sensor.
https://www.sparkfun.com/products/11824

Like most pressure sensors, the BMP180 measures absolute pressure.
This is the actual ambient pressure seen by the device, which will
vary with both altitude and weather.

Before taking a pressure reading you must take a temparture reading.
This is done with startTemperature() and getTemperature().
The result is in degrees C.

Once you have a temperature reading, you can take a pressure reading.
This is done with startPressure() and getPressure().
The result is in millibar (mb) aka hectopascals (hPa).

If you'll be monitoring weather patterns, you will probably want to
remove the effects of altitude. This will produce readings that can
be compared to the published pressure readings from other locations.
To do this, use the sealevel() function. You will need to provide
the known altitude at which the pressure was measured.

If you want to measure altitude, you will need to know the pressure
at a baseline altitude. This can be average sealevel pressure, or
a previous pressure reading at your altitude, in which case
subsequent altitude readings will be + or - the initial baseline.
This is done with the altitude() function.

Hardware connections:

- (GND) to GND
+ (VDD) to 3.3V

(WARNING: do not connect + to 5V or the sensor will be damaged!)

You will also need to connect the I2C pins (SCL and SDA) to your
Arduino. The pins are different on different Arduinos:

Any Arduino pins labeled:  SDA  SCL
Uno, Redboard, Pro:        A4   A5
Mega2560, Due:             20   21
Leonardo:                   2    3

Leave the IO (VDDIO) pin unconnected. This pin is for connecting
the BMP180 to systems with lower logic levels such as 1.8V

Have fun! -Your friends at SparkFun.

The SFE_BMP180 library uses floating-point equations developed by the
Weather Station Data Logger project: http://wmrx00.sourceforge.net/

Our example code uses the "beerware" license. You can do anything
you like with this code. No really, anything. If you find it useful,
buy me a beer someday.

V10 Mike Grusin, SparkFun Electronics 10/24/2013
V1.1.2 Updates for Arduino 1.6.4 5/2015
*/

// Your sketch must #include this library, and the Wire library.
// (Wire is a standard library included with Arduino.):

#include <SFE_BMP180.h>
#include <Wire.h>
#include <LiquidCrystal.h>        //Load Liquid Crystal Library
#include <SoftwareSerial.h>       //wifi is communicating througth serialPorts

#define  uchar unsigned char      //To reduce programming time only
#define uint  unsigned int
#define DEBUG FALSE               //comment out to remove debug msgs

//*-- Hardware Serial
#define _baudrate 115200           //mey differ for your wifi Module____mine is esp8266

//*-- Software Serial
#define _rxpin      50
#define _txpin      51

//Maximum length of the array
#define MAX_LEN 16


//*-- IoT Information
#define SSID "cube"              //SSID of your router/network
#define PASS "xyz54321"             //PASSWORD to connectwith it.
#define DEST_HOST "www.storywith.in"            //IP address or URL of website/server You wants to connect
#define DEST_IP "50.31.138.216"

SoftwareSerial wifi( _rxpin, _txpin ); // RX, TX
uchar serNumA[5];

uchar fifobytes;
uchar fifoValue;


// You will need to create an SFE_BMP180 object, here called "pressure":

SFE_BMP180 pressure;

#define ALTITUDE 1655.0 // Altitude of SparkFun's HQ in Boulder, CO. in meters

int MQ3 = A9;
int MQ8 = A8;
int MQ135 = A7;
int MQ3Value = 0;
int MQ8Value = 0;
int MQ135Value = 0;

//For RGB Leds
const int redLed = A1;
const int greenLed = A2;
const int blueLed = A3;


String GET = "GET /industry/industryInsert.php";     //FilePath to which you wants to send data through GET REQUEST

//LCD + Ultra
LiquidCrystal LCD(8, 9, 4, 5, 6, 7); //Create Liquid Crystal Object called LCD



void setup()
{
  pinMode(redLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(blueLed, OUTPUT);



  Serial.println("REBOOT");

  // Initialize the sensor (it is important to get calibration values stored on the device).

  if (pressure.begin())
    Serial.println("BMP180 init success");
  else
  {
    // Oops, something went wrong, this is usually a connection problem,
    // see the comments at the top of this sketch for the proper connections.

    Serial.println("BMP180 init fail\n\n");
    while (1); // Pause forever.
  }

  LCD.begin(16, 2); //Tell Arduino to start your 16 column 2 row LCD
  LCD.setCursor(0, 0); //Set LCD cursor to upper left corner, column 0, row 0
  LCD.print("Wait Some Time...");  //Print Message on First Row

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
    //goto AT;
  }
  LCD.setCursor(0, 0);
  LCD.print("                ");
  LCD.setCursor(0,0);
  LCD.print("Industrial Monitoring ON");
}

void loop()
{
  char status;
  double T, P, p0, a;
  LCD.setCursor(0, 0);
  LCD.print("                ");
  LCD.setCursor(0,0);
  LCD.print("Industrial Monitoring ON");
  LCD.setCursor(0, 1);
  LCD.print("                ");

  // Loop here getting pressure readings every 10 seconds.

  // If you want sea-level-compensated pressure, as used in weather reports,
  // you will need to know the altitude at which your measurements are taken.
  // We're using a constant called ALTITUDE in this sketch:

  Serial.println();
  Serial.print("provided altitude: ");
  Serial.print(ALTITUDE, 0);
  Serial.print(" meters, ");
  Serial.print(ALTITUDE * 3.28084, 0);
  Serial.println(" feet");

  // If you want to measure altitude, and not pressure, you will instead need
  // to provide a known baseline pressure. This is shown at the end of the sketch.

  // You must first get a temperature measurement to perform a pressure reading.

  // Start a temperature measurement:
  // If request is successful, the number of ms to wait is returned.
  // If request is unsuccessful, 0 is returned.

  status = pressure.startTemperature();
  if (status != 0)
  {
    // Wait for the measurement to complete:
    delay(status);

    // Retrieve the completed temperature measurement:
    // Note that the measurement is stored in the variable T.
    // Function returns 1 if successful, 0 if failure.

    status = pressure.getTemperature(T);
    if (status != 0)
    {
      // Print out the measurement:
      Serial.print("temperature: ");
      Serial.print(T, 2);
      Serial.print(" deg C, ");
      Serial.print((9.0 / 5.0)*T + 32.0, 2);
      Serial.println(" deg F");

      // Start a pressure measurement:
      // The parameter is the oversampling setting, from 0 to 3 (highest res, longest wait).
      // If request is successful, the number of ms to wait is returned.
      // If request is unsuccessful, 0 is returned.

      status = pressure.startPressure(3);
      if (status != 0)
      {
        // Wait for the measurement to complete:
        delay(status);

        // Retrieve the completed pressure measurement:
        // Note that the measurement is stored in the variable P.
        // Note also that the function requires the previous temperature measurement (T).
        // (If temperature is stable, you can do one temperature measurement for a number of pressure measurements.)
        // Function returns 1 if successful, 0 if failure.

        status = pressure.getPressure(P, T);
        if (status != 0)
        {
          // Print out the measurement:
          Serial.print("absolute pressure: ");
          Serial.print(P, 2);
          Serial.print(" mb, ");
          Serial.print(P * 0.0295333727, 2);
          Serial.println(" inHg");
          /**LCD.setCursor(0,1);
          LCD.print("                ");
          LCD.setCursor(0,1);
          LCD.print(P, 2);
          LCD.print(" mb, ");
          LCD.print(P * 0.0295333727, 2);
          LCD.println(" inHg");
          */
          if (P > 345) {
            LCD.setCursor(0, 0);
            LCD.print("                ");
            LCD.setCursor(0,0);
            LCD.print("PRESSURE is very HIGH");
            LCD.setCursor(0, 1);
            LCD.print("                ");
            LCD.setCursor(0,1);
            LCD.print(P, 2);
            LCD.print(" mb, ");
            LCD.print(P * 0.0295333727, 2);
            LCD.print("  inHg");
            delay(500);
          }
          else if (P < 45) {
            LCD.setCursor(0, 0);
            LCD.print("                ");
            LCD.setCursor(0,0);
            LCD.print("PRESSURE is very LOW");
            LCD.setCursor(0, 1);
            LCD.print("                ");
            LCD.setCursor(0,1);
            LCD.print(P, 2);
            LCD.print(" mb, ");
            LCD.print(P * 0.0295333727, 2);
            LCD.print("  inHg");
            delay(500);
          }

          // The pressure sensor returns abolute pressure, which varies with altitude.
          // To remove the effects of altitude, use the sealevel function and your current altitude.
          // This number is commonly used in weather reports.
          // Parameters: P = absolute pressure in mb, ALTITUDE = current altitude in m.
          // Result: p0 = sea-level compensated pressure in mb

          p0 = pressure.sealevel(P, ALTITUDE); // we're at 1655 meters (Boulder, CO)
          Serial.print("relative (sea-level) pressure: ");
          Serial.print(p0, 2);
          Serial.print(" mb, ");
          Serial.print(p0 * 0.0295333727, 2);
          Serial.println(" inHg");

          // On the other hand, if you want to determine your altitude from the pressure reading,
          // use the altitude function along with a baseline pressure (sea-level or other).
          // Parameters: P = absolute pressure in mb, p0 = baseline pressure in mb.
          // Result: a = altitude in m.

          a = pressure.altitude(P, p0);
          Serial.print("computed altitude: ");
          Serial.print(a, 0);
          Serial.print(" meters, ");
          Serial.print(a * 3.28084, 0);
          Serial.println(" feet");
        }
        else Serial.println("error retrieving pressure measurement\n");
      }
      else Serial.println("error starting pressure measurement\n");
    }
    else Serial.println("error retrieving temperature measurement\n");
  }
  else Serial.println("error starting temperature measurement\n");

  delay(500);  // Pause for 0.5 seconds.
  MQ3Value = analogRead(MQ3);
  Serial.println(MQ3Value);
  MQ8Value = analogRead(MQ8);
  Serial.println(MQ8Value);
  MQ135Value = analogRead(MQ135);
  Serial.println(MQ135Value);
  /**LCD.setCursor(0, 0);
  LCD.print("                ");
  LCD.setCursor(0, 0);
  LCD.print(T, 2);
  LCD.print(" ");
  LCD.print(MQ3Value);
  LCD.print(" ");
  LCD.print(MQ8Value);
  LCD.print(" ");
  LCD.print(MQ135Value);
  */
  if (T > 40) {
    LCD.setCursor(0, 0);
    LCD.print("                ");
    LCD.setCursor(0,0);
    LCD.print("TEMP. is very HIGH");
    LCD.setCursor(0, 1);
    LCD.print("                ");
    LCD.setCursor(0,1);
    LCD.print(T, 2);
    LCD.print(" degree Temp ");
    delay(500);
  }
  else if (T < 20) {
    LCD.setCursor(0, 0);
    LCD.print("                ");
    LCD.setCursor(0,0);
    LCD.print("TEMP. is very LOW");
    LCD.setCursor(0, 1);
    LCD.print("                ");
    LCD.setCursor(0,1);
    LCD.print(T, 2);
    LCD.print(" degree Temp ");
    delay(500);
  }

  if (MQ3Value > 200) {
    LCD.setCursor(0, 0);
    LCD.print("                ");
    LCD.setCursor(0,0);
    LCD.print("MQ3 level is HIGH");
    LCD.setCursor(0, 1);
    LCD.print("                ");
    LCD.setCursor(0,1);
    LCD.print(MQ3Value);
    LCD.print(" analog Read ");
    delay(500);
  }
  else if (MQ3Value < 40) {
    LCD.setCursor(0, 0);
    LCD.print("                ");
    LCD.setCursor(0,0);
    LCD.print("MQ3 level is LOW");
    LCD.setCursor(0, 1);
    LCD.print("                ");
    LCD.setCursor(0,1);
    LCD.print(MQ3Value);
    LCD.print(" analog Read ");
    delay(500);
  }

  if (MQ8Value > 200) {
    LCD.setCursor(0, 0);
    LCD.print("                ");
    LCD.setCursor(0,0);
    LCD.print("MQ8 level is HIGH");
    LCD.setCursor(0, 1);
    LCD.print("                ");
    LCD.setCursor(0,1);
    LCD.print(MQ8Value);
    LCD.print(" analog Read ");
    delay(500);
  }
  else if (MQ8Value < 40) {
    LCD.setCursor(0, 0);
    LCD.print("                ");
    LCD.setCursor(0,0);
    LCD.print("MQ8 level is LOW");
    LCD.setCursor(0, 1);
    LCD.print("                ");
    LCD.setCursor(0,1);
    LCD.print(MQ8Value);
    LCD.print(" analog Read ");
    delay(500);
  }

  if (MQ135Value > 200) {
    LCD.setCursor(0, 0);
    LCD.print("                ");
    LCD.setCursor(0,0);
    LCD.print("MQ135 level is HIGH");
    LCD.setCursor(0, 1);
    LCD.print("                ");
    LCD.setCursor(0,1);
    LCD.print(MQ135Value);
    LCD.print(" analog Read ");
    delay(500);
  }
  else if (MQ135Value < 40) {
    LCD.setCursor(0, 0);
    LCD.print("                ");
    LCD.setCursor(0,0);
    LCD.print("MQ135 level is LOW");
    LCD.setCursor(0, 1);
    LCD.print("                ");
    LCD.setCursor(0,1);
    LCD.print(MQ135Value);
    LCD.print(" analog Read ");
    delay(500);
  }
  

  
  updateDatabase(MQ3Value, MQ8Value, MQ135Value, P, P * 0.0295333727, T);


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
    //goto CWJAP;                                                //Found Error
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
    //goto CIPMUX;
    //return false;
  }
  //**********else if( wifi.find("OK"))
  blinkLed(greenLed, 500);                            //1.5 Sec. delay(total).
  blinkLed(greenLed, 500);
  blinkLed(greenLed, 500);                            //Indicating ready for reading RFID cards
}

// SEND GET REQUEST(QUERY) to Perticulor URL
void updateDatabase( int MQ3Value, int MQ8Value, int MQ135Value, double p1 , double p2 , double temp )
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
  cmd = GET + "?mq3=" + MQ3Value + "&mq8=" + MQ8Value + "&mq135=" + MQ135Value + "&p1=" + p1 + "&p2=" + p2 + "&temp=" + temp + "&location=Ahmedabad" + " HTTP/1.1\r\nHOST: " + DEST_HOST + ":80\r\n\r\n";
  wifi.print( "AT+CIPSEND=" );
  wifi.println( cmd.length() );
  Serial.print( "AT+CIPSEND=" );
  Serial.println( cmd.length() );
  //Serial.print(wifi.read());
  //if (wifi.find( ">" ) )
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
  //else
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

