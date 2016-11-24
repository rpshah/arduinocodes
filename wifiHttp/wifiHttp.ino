/*
  Software serial multple serial test

 Receives from the hardware serial, sends to software serial.
 Receives from software serial, sends to hardware serial.

 The circuit:
 * RX is digital pin 10 (connect to TX of other device)
 * TX is digital pin 11 (connect to RX of other device)

 Note:
 Not all pins on the Mega and Mega 2560 support change interrupts,
 so only the following can be used for RX:
 10, 11, 12, 13, 50, 51, 52, 53, 62, 63, 64, 65, 66, 67, 68, 69

 Not all pins on the Leonardo support change interrupts,
 so only the following can be used for RX:
 8, 9, 10, 11, 14 (MISO), 15 (SCK), 16 (MOSI).

 created back in the mists of time
 modified 25 May 2012
 by Tom Igoe
 based on Mikal Hart's example

 This example code is in the public domain.

 */
#include <SoftwareSerial.h>
#include<Bridge.h>
#include<HttpClient.h>
#include<SPI.h>

#define Ssid "TP-LINK_36A6"
#define Pass "nathikhabar"

SoftwareSerial mySerial(10, 11); // RX, TX

void setup()
{
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
//  Bridge.begin();

  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }


  Serial.println("Goodnight moon!");

  // set the data rate for the SoftwareSerial port
  mySerial.begin(115200);
  while (!mySerial);

  // delay(5000);
  //  mySerial.println("Hello, world?");
  while (mySerial.available() > 0)
    mySerial.read();

  delay(1000);
  //test if the module is ready
  mySerial.print("AT+RST");
      //try "AT+CWJAP?"  ==response sould be "ok" or "OK"
  //delay(5000);
  //delay(1000);
  Serial.println("Resetting module");
  Serial.flush();
  while(mySerial.available())
  Serial.print(mySerial.read());
  //if(Serial2.find("ready"))
  if (mySerial.find("Ready") || mySerial.find("ready"))
  {
    //dbgSerial.println("Module is ready");
    Serial.println("Module is ready");
  }
  else
  {
    //dbgSerial.println("Module have no response.");
    Serial.println("Module have no response.");
    while (1);
  }
  delay(1000);
  //connect to the wifi
  boolean connected = false;
  for (int i = 0; i < 5; i++)
  {
    if (connectWiFi())
    {
      connected = true;
      break;
    }
  }
  if (!connected) {
    while (1);
  }
  delay(5000);
  //print the ip addr
  /*
  Serial2.println("AT+CIFSR");
  Serial.println("ip address:");
  while (Serial2.available())
  Serial.write(Serial2.read());

  */
  //set the single connection mode
  mySerial.println("AT+CIPMUX=0");

}

void loop() // run over and over
{
  /*if (mySerial.available())
    Serial.write(mySerial.read());
  if (Serial.available())
    mySerial.write(Serial.read());
  */int err = 0;

  //EthernetClient c;
  HttpClient http;

  err = http.get("http://arduino.cc/asciilogo.txt");
  Serial.println(err);
  if (err == 0)
  {
    Serial.println("startedRequest ok");

    //err = http.responseStatusCode();
    if (err >= 0)
    {
      Serial.print("Got status code: ");
      Serial.println(err);

      // Usually you'd check that the response code is 200 or a
      // similar "success" code (200-299) before carrying on,
      // but we'll print out whatever response we get

      //      err = http.skipResponseHeaders();
    }
  }
}


boolean connectWiFi()
{
  mySerial.println("AT+CWMODE=1");
  String cmd = "AT+CWJAP=\"";
  cmd += Ssid;
  cmd += "\",\"";
  cmd += Pass;
  cmd += "\"";
  //dbgSerial.println(cmd);
  mySerial.println(cmd);
  Serial.println(cmd);
  delay(2000);
  if (mySerial.find("OK"))
  {
    //dbgSerial.println("OK, Connected to WiFi.");
    Serial.println("OK, Connected to WiFi.");
    return true;
  } else
  {
    //dbgSerial.println("Can not connect to the WiFi.");
    Serial.println("Can not connect to the WiFi.");
    return false;
  }
}
