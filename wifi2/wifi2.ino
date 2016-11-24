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
#include<HttpClient.h>
#include<Bridge.h>
SoftwareSerial mySerial(10, 11); // RX, TX
int count = 0;
void setup()
{
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
    Bridge.begin();

  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }


  Serial.println("Goodnight moon!");

  // set the data rate for the SoftwareSerial port
  mySerial.begin(115200);
}

void loop() // run over and over
{
/*  if (mySerial.available())
    Serial.write(mySerial.read());
  if (Serial.available())
    mySerial.write(Serial.read());
*/
  if (count == 0) {
    count++;
    HttpClient client;
    client.get("http://arduino.cc/asciilogo.txt");
    delay(2000);
    while (client.available()) {
      char c = char(client.read());
      Serial.print(c);
      delay(1000);
    }
    Serial.flush();

    delay(5000);
  }
}

