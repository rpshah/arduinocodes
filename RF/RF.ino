#include <VirtualWire.h>
char *controllerTr;
void setup() {
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  vw_set_ptt_inverted(true); //
  vw_set_tx_pin(12);
  vw_set_rx_pin(2);
  vw_setup(4000);// speed of data transfer Kbps
   vw_rx_start();       // Start the receiver PLL running
}

void loop() {
  controllerTr = "1"  ;
  vw_send((uint8_t *)controllerTr, strlen(controllerTr));
  vw_wait_tx(); // Wait until the whole message is gone
  Serial.println("Transmitter : 1");
  digitalWrite(13, 1);
  delay(2000);
  uint8_t buf[VW_MAX_MESSAGE_LEN];
    uint8_t buflen = VW_MAX_MESSAGE_LEN;

    if (vw_get_message(buf, &buflen)) // Non-blocking
    {
      Serial.println("Recivied");
      if(buf[0]=='1'){
      Serial.println("Reciever Got 1");
  
   digitalWrite(13,0);
      }  
   if(buf[0]=='0'){
    Serial.println("Reciever Got 0");
  digitalWrite(13,1);
    }

}
  controllerTr = "0"  ;
  vw_send((uint8_t *)controllerTr, strlen(controllerTr));
  vw_wait_tx(); // Wait until the whole message is gone
  Serial.println("Transmitter : 0");
  digitalWrite(13, 0);
  delay(2000);
  if (vw_get_message(buf, &buflen)) // Non-blocking
    {
      if(buf[0]=='1'){
      Serial.println("Reciever Got 1");
  
   digitalWrite(13,0);
      }  
   if(buf[0]=='0'){
    Serial.println("Reciever Got 0");
  digitalWrite(13,1);
    }

}

}
