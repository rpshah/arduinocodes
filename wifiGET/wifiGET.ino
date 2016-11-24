#include <SoftwareSerial.h>
//#include<HttpClient.h>

//my web site, replace with yours
#define DST_IP "127.0.0.1"
SoftwareSerial mySerial(10, 11); // RX, TX

/* The circuit:
 * RX is digital pin 10 (connect to TX of other device)
 * TX is digital pin 11 (connect to RX of other device)
*/
//Define the pin for the on board LED
int led = 13;

//Initialize Device
void setup()
{
  //Enable the Onboard LED to act as a Status indicator light
  pinMode(led, OUTPUT);
  //Set the speed for communicating with the ESP8266 module
  Serial.begin(115200);
  while (!Serial) {
    ; //wait sometime
  }
  Serial.println("Program Started.");
  //Send a series of flashes to show us we are in the bootup phase.
  mySerial.begin(115200);
  blinkcode ();
  //Reset the ESP8266
  delay(5000);
  mySerial.println("AT+RST");
  //Wait for the WiFi module to bootup
  delay(1000);
  while (mySerial.available()) {
    Serial.write(mySerial.read());
  }
  //Switch the chip to "client" mode
 /* mySerial.println("AT+CWMODE=1");
  //Connect to our Wireless network NOTE: change SSID to your wireless
  //network and PASSWORD to the key used to join the network.
  mySerial.println("AT+CWJAP=\"Cuboid\",\"swami123\"");
  //Once again blink some lights because it is fun!
  while (mySerial.available()) {
    Serial.write(mySerial.read());
  }
  delay(3000);
  */blinkcode ();
}

void loop()
{
  //Open a connection to the web server
  String cmd = "AT+CIPSTART=\"TCP\",\""; //make this command: AT+CPISTART="TCP","192.168.88.35",80
  cmd += DST_IP;
  cmd += "\",80";
  Serial.println(cmd);
  mySerial.println(cmd);
  while (mySerial.available()) {
    Serial.write(mySerial.read());
  }

  //wait a little while for 'Linked'
  delay(300);

  //This is our HTTP GET Request change to the page and server you want to load.
  cmd = "GET /testingget.php?&temp=23.21 HTTP/1.0\r\n";
  cmd += "Host: 127.0.0.1\r\n\r\n";

  //The ESP8266 needs to know the size of the GET request
  Serial.print("AT+CIPSEND=");
  Serial.println(cmd.length());
  mySerial.print("AT+CIPSEND=");
  mySerial.println(cmd.length());
  while (mySerial.available()) {
    Serial.write(mySerial.read());
  }
  //Look for the > prompt from the esp8266
  if (mySerial.find(">"))
  {
    //Send our http GET request
    Serial.println(cmd);
    mySerial.println(cmd);
    //mySerial.println();

  }
  else
  {
    //Something didn't work...
    Serial.println("AT+CIPCLOSE");
    mySerial.println("AT+CIPCLOSE");
  }

  //Check the returned header & web page. Looking for a keyword. I used YES12321

  /*if (Serial.find("YES12321"))
  {
    //If the string was found we know the page is up and we turn on the LED status
    //light to show the server is ONLINE
    digitalWrite(led, HIGH);
  }
  else
  {
    //If the string was not found then we can assume the server is offline therefore
    //we should turn of the light.
    digitalWrite(led, LOW);
  }*/
  //Wait a second for everything to settle.
  delay(1000);
  //Be great people and close our connection.
  mySerial.println("AT+CIPCLOSE");
  Serial.println("AT+CIPCLOSE");

  //Introduce a delay timer before we finish and go back to the begining.
  delay (5000);
}

void blinkcode ()
{
  //Simply loop a few times flashing the status light (this is used during boot up)
  int i;
  for (i = 1; i <= 10; i++) {
    delay (100);
    digitalWrite(led, HIGH);
    delay (100);
    digitalWrite(led, LOW);
  }
}
