#include<SoftwareSerial.h>
#define DEBUG true

SoftwareSerial wifi(2,3);

void setup(){
  Serial.begin(9600);
  wifi.begin(9600);

  sendData(" AT+RST\r\n",2000,DEBUG);
  sendData(" AT+CWMODE= 2\r\n",1000,DEBUG);
  sendData(" AT+CIFSR\r\n",1000,DEBUG);
  sendData(" AT+CIPMUX= 1\r\n",1000,DEBUG);
  sendData(" AT+CIPSERVER= 1,80\r\n",1000,DEBUG);
}

void loop(){
  if(wifi.available()){
    while(wifi.available()){
      char c=wifi.read();
      Serial.write(c);
    }
    if(wifi.find(" +IDP, ")){
      delay(1000);
      int connectionId=wifi.read()-48;

      String webPage=" <h1>Hello</h1><h2>world!</h2><button>LED</buton> ";
      String cipSend=" AT+CIPSEND= ";
      cipSend+=connectionId;
      cipSend+=" ,";
      cipSend+=webPage.length();
      cipSend+=" \r\n";

      sendData(cipSend,1000,DEBUG);
      sendData(webPage,1000,DEBUG);
      
      webPage=" <h1>NeXt:</h1>PaGe:<h2></h2><button>LED</buton> ";
      cipSend=" AT+CIPSEND= ";
      cipSend+=connectionId;
      cipSend+=" ,";
      cipSend+=webPage.length();
      cipSend+=" \r\n";

      sendData(cipSend,1000,DEBUG);
      sendData(webPage,1000,DEBUG);
      
      String closeCommand=" AT+CIPCLOSE= ";
      closeCommand +=connectionId;
      closeCommand +=" \r\n ";
      
      closeCommand(closeCommand,3000,DEBUG);
    }
  }
}

String SendData(String command,const int timeOut, boolean debug ){
  String response="";
  wifi.print(command);
  
  long int time=millis();
  
  while((time+timeOut)>millis()){
    while(wifi.available()){
      char c=wifi.read();
      response+=c;
    }
  }
  
  if(debug){
    Serial.println(response);
  }
  
  return response;
}

