/**
 * @example HTTPGET.ino
 * @brief The HTTPGET demo of library WeeESP8266.
 * @author Wu Pengfei<pengfei.wu@itead.cc>
 * @date 2015.03
 *
 * @par Copyright:
 * Copyright (c) 2015 ITEAD Intelligent Systems Co., Ltd. \n\n
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version. \n\n
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#include <SoftwareSerial.h>
#include "ESP8266.h"

#define SSID        "tshah56"
#define PASSWORD    "N!$$123t"
#define HOST_NAME   "http://192.168.1.2"
#define HOST_PORT   (80)

SoftwareSerial mySerial(10, 11); /* RX:D3, TX:D2 */
ESP8266 wifi(mySerial);

void setup()
{
  Serial.begin(115200);
  Serial.print("setup begin\r\n");

  /*if (wifi.restart()) {
    Serial.println("Restarted");
  }
  else {
    Serial.println("Cold not restart.");
  }*/
  /* Serial.print("FW Version:");
    Serial.println(wifi.getVersion());
   //Serial.println(wifi.getVersion().c_str());

   /*if (wifi.setOprToStationSoftAP()) {
       Serial.print("to station + softap ok\r\n");
   } else {
       Serial.print("to station + softap err\r\n");
   }
  */
  /*if (wifi.joinAP(SSID, PASSWORD)) {
      Serial.print("Join AP success\r\n");

      Serial.print("IP:");
      Serial.println(wifi.getLocalIP().c_str());
  } else {
      Serial.print("Join AP failure\r\n");
  }
  */
  if (wifi.disableMUX()) {
    Serial.print("single ok\r\n");
  } else {
    Serial.print("single err\r\n");
  }
  //Serial.print("setup end\r\n");
}

void loop()
{
  uint8_t buffer[1024] = {0};

  if (wifi.createTCP(HOST_NAME, HOST_PORT)) {
    Serial.print("create tcp ok\r\n");
  } else {
    Serial.print("create tcp err\r\n");
  }

  char *hello = "GET /testingget.php?&temp=23.4\r\n\r\n";
  wifi.send((const uint8_t*)hello, strlen(hello));
  uint32_t len = wifi.recv(buffer, sizeof(buffer), 10000);
  if (len > 0) {
    Serial.print("Received:[");
    for (uint32_t i = 0; i < len; i++) {
      Serial.print((char)buffer[i]);
    }
    Serial.print("]\r\n");
  }

  if (wifi.releaseTCP()) {
    Serial.print("release tcp ok\r\n");
  } else {
    Serial.print("release tcp err\r\n");
  }
  Serial.println("Program Sopped.");
  while (1);

}


