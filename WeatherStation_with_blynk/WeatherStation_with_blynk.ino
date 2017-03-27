//#define BLYNK_USE_DIRECT_CONNECT

#define BLYNK_PRINT Serial

#include <BlynkSimpleCurieBLE.h>
#include <CurieBLE.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "cf44daeac5d640a8bc7e88f2f528a9cf";

BLEPeripheral  blePeripheral;

// defines pins numbers
const int methenePin = A1;    
const int coPin = A2;   
const int benzenePin = A3;  

const boolean DEBUG = true;

void setup() {
  
  pinMode(methenePin, INPUT);
  pinMode(coPin, INPUT);
  pinMode(benzenePin, INPUT);

  Serial.begin(9600);
  delay(1000);
  blePeripheral.setLocalName("Blynk");
  blePeripheral.setDeviceName("Blynk");
  blePeripheral.setAppearance(384);
  Serial.println("ble will start now start");
  Blynk.begin(auth, blePeripheral);
  blePeripheral.begin();

  Serial.println("Waiting for connections...");

}

void loop() {
  Blynk.run();
  blePeripheral.poll();

  int m = findMethene();
  showMethene(m);
  int c = findCO();
  showCO(c);
  int b = findBenzene();
  showBenzene(b);
  delay(1000);
}

int findMethene(){
  return analogRead(methenePin);
}

int findCO(){
  return analogRead(coPin);
}

int findBenzene(){
  return analogRead(benzenePin);
}

void showMethene(int value){
  if(DEBUG){
    Serial.print("Methene(Analog Reading) :\t");
    Serial.println(value);
  }
  Blynk.virtualWrite(V1, value);
}


void showCO(int value){
  if(DEBUG){
    Serial.print("Carbon Monoxide(Analog Reading) :\t");
    Serial.println(value);
  }
  Blynk.virtualWrite(V2, value);
}


void showBenzene(int value){
  if(DEBUG){
    Serial.print("Benzene(Analog Reading) :\t");
    Serial.println(value);
  }
  Blynk.virtualWrite(V3, value);
}
