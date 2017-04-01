#include <BlynkSimpleCurieBLE.h>
#include <CurieBLE.h>

#include "CurieIMU.h"
#include "dht.h"

#define DHTPIN 2     // what digital pin we're connected to

#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321

#define BLYNK_PRINT Serial

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "622c8a2e7276483da1076df6d32f209f";

BLEPeripheral  blePeripheral;

DHT dht(DHTPIN, DHTTYPE);

int CO2_PIN = 5;
const int benzenePin = A3;  
int alarm = 13;

int LDR = A0;     //analog pin to which LDR is connected, here we set it to 0 so it means A0

int maxCO2 = 5000;
float maxTemp = 50;
float minHum = 12;

float hum = 0;
float temp = 0;

int value_CO2 = 0;
int count_CO2 = 0;

const boolean DEBUG = true;

int ax, ay, az;         // accelerometer values
int gx, gy, gz;         // gyrometer values

int calibrateOffsets = 1; // int to determine whether calibration takes place or not

WidgetLED ledX1(V6);
WidgetLED ledX2(V7);
WidgetLED ledY1(V8);
WidgetLED ledY2(V9);
WidgetLED ledZ1(V10);
WidgetLED ledZ2(V11);
WidgetLED ledFall(V12);
  
void setup() {
  pinMode(CO2_PIN,INPUT);
  pinMode(alarm,OUTPUT);

  // put your setup code here, to run once:
  Serial.begin(9600); // initialize Serial communication
  blePeripheral.setLocalName("Blynk");
  blePeripheral.setDeviceName("Blynk");
  blePeripheral.setAppearance(384);
  Serial.println("ble will start now start");
  Blynk.begin(auth, blePeripheral);
  blePeripheral.begin();

  dht.begin();
  
  // initialize device
  Serial.println("Initializing IMU device...");

  // verify connection
  Serial.println("Testing device connections...");
  if (CurieIMU.begin()) {
    Serial.println("CurieIMU connection successful");
    //Attach Event Callback

  } else {
    Serial.println("CurieIMU connection failed");
  }
  CurieIMU.attachInterrupt(eventCallback);
  // use the code below to calibrate accel/gyro offset values
  if (calibrateOffsets == 1) {
    Serial.println("Internal sensor offsets BEFORE calibration...");
    Serial.print(CurieIMU.getAccelerometerOffset(X_AXIS));
    Serial.print("\t"); // -76
    Serial.print(CurieIMU.getAccelerometerOffset(Y_AXIS));
    Serial.print("\t"); // -235
    Serial.print(CurieIMU.getAccelerometerOffset(Z_AXIS));
    Serial.print("\t"); // 168
    Serial.print(CurieIMU.getGyroOffset(X_AXIS));
    Serial.print("\t"); // 0
    Serial.print(CurieIMU.getGyroOffset(Y_AXIS));
    Serial.print("\t"); // 0
    Serial.println(CurieIMU.getGyroOffset(Z_AXIS));

    // To manually configure offset compensation values,
    // use the following methods instead of the autoCalibrate...() methods below
    //CurieIMU.setAccelerometerOffset(X_AXIS,495.3);
    //CurieIMU.setAccelerometerOffset(Y_AXIS,-15.6);
    //CurieIMU.setAccelerometerOffset(Z_AXIS,491.4);
    //CurieIMU.setGyroOffset(X_AXIS,7.869);
    //CurieIMU.setGyroOffset(Y_AXIS,-0.061);
    //CurieIMU.setGyroOffset(Z_AXIS,15.494);

    Serial.println("About to calibrate. Make sure your board is stable and upright");
    delay(5000);

    // The board must be resting in a horizontal position for
    // the following calibration procedure to work correctly!
    Serial.print("Starting Gyroscope calibration and enabling offset compensation...");
    CurieIMU.autoCalibrateGyroOffset();
    Serial.println(" Done");

    Serial.print("Starting Acceleration calibration and enabling offset compensation...");
    CurieIMU.autoCalibrateAccelerometerOffset(X_AXIS, 0);
    CurieIMU.autoCalibrateAccelerometerOffset(Y_AXIS, 0);
    CurieIMU.autoCalibrateAccelerometerOffset(Z_AXIS, 1);
    Serial.println(" Done");

    Serial.println("Internal sensor offsets AFTER calibration...");
    Serial.print(CurieIMU.getAccelerometerOffset(X_AXIS));
    Serial.print("\t"); // -76
    Serial.print(CurieIMU.getAccelerometerOffset(Y_AXIS));
    Serial.print("\t"); // -2359
    Serial.print(CurieIMU.getAccelerometerOffset(Z_AXIS));
    Serial.print("\t"); // 1688
    Serial.print(CurieIMU.getGyroOffset(X_AXIS));
    Serial.print("\t"); // 0
    Serial.print(CurieIMU.getGyroOffset(Y_AXIS));
    Serial.print("\t"); // 0
    Serial.println(CurieIMU.getGyroOffset(Z_AXIS));
  }
  /* Enable Zero Motion Detection */
  CurieIMU.setDetectionThreshold(CURIE_IMU_ZERO_MOTION, 50);  // 50mg
  CurieIMU.setDetectionDuration(CURIE_IMU_ZERO_MOTION, 2);    // 2s
  CurieIMU.interrupts(CURIE_IMU_ZERO_MOTION);

  /* Enable Motion Detection */
  CurieIMU.setDetectionThreshold(CURIE_IMU_MOTION, 20);      // 20mg
  CurieIMU.setDetectionDuration(CURIE_IMU_MOTION, 10);       // trigger times of consecutive slope data points
  CurieIMU.interrupts(CURIE_IMU_MOTION); 

  /* Enable Shock Detection */
  CurieIMU.setDetectionThreshold(CURIE_IMU_SHOCK, 1500); // 1.5g = 1500 mg
  CurieIMU.setDetectionDuration(CURIE_IMU_SHOCK, 50);   // 50ms
  CurieIMU.interrupts(CURIE_IMU_SHOCK);

  /* Enable Free Fall Detection */
  CurieIMU.setDetectionThreshold(CURIE_IMU_FREEFALL, 1000); // 1g=1000mg
  CurieIMU.setDetectionDuration(CURIE_IMU_FREEFALL, 50);  // 50ms
  CurieIMU.interrupts(CURIE_IMU_FREEFALL);

}

void loop() {
  // put your main code here, to run repeatedly:
  Blynk.run();
  blePeripheral.poll();
  delay(1000);  
}

void readMotionData(){
  // read raw accel/gyro measurements from device
  CurieIMU.readMotionSensor(ax, ay, az, gx, gy, gz);

  // these methods (and a few others) are also available

  //CurieIMU.readAcceleration(ax, ay, az);
  //CurieIMU.readRotation(gx, gy, gz);

  //ax = CurieIMU.readAccelerometer(X_AXIS);
  //ay = CurieIMU.readAccelerometer(Y_AXIS);
  //az = CurieIMU.readAccelerometer(Z_AXIS);
  //gx = CurieIMU.readGyro(X_AXIS);
  //gy = CurieIMU.readGyro(Y_AXIS);
  //gz = CurieIMU.readGyro(Z_AXIS);

  // display tab-separated accel/gyro x/y/z values
  Serial.print("a/g:\t");
  Serial.print(ax);
  Serial.print("\t");
  Serial.print(ay);
  Serial.print("\t");
  Serial.print(az);
  Serial.print("\t");
  Serial.print(gx);
  Serial.print("\t");
  Serial.print(gy);
  Serial.print("\t");
  Serial.println(gz);
  Serial.println(" ");  
}

static void eventCallback(void){
  if (CurieIMU.getInterruptStatus(CURIE_IMU_ZERO_MOTION)) {
   Serial.println("zero motion detected...");
   //ledX1.off();
   //ledX2.off();
   //ledY1.off();
   //ledY2.off();
   //ledZ1.off();
   //ledZ2.off();
   //ledFall.off();
  }  
  if (CurieIMU.getInterruptStatus(CURIE_IMU_MOTION)) {
    if (CurieIMU.motionDetected(X_AXIS, POSITIVE)){
      Serial.println("Negative motion detected on X-axis");
      //ledX2.on();
    }
    if (CurieIMU.motionDetected(X_AXIS, NEGATIVE)){
      Serial.println("Positive motion detected on X-axis");
      //ledX2.on();
    }
    if (CurieIMU.motionDetected(Y_AXIS, POSITIVE)){
      Serial.println("Negative motion detected on Y-axis");
      //ledY2.on();
    }
    if (CurieIMU.motionDetected(Y_AXIS, NEGATIVE)){
      Serial.println("Positive motion detected on Y-axis");
      //ledY2.on();
    }
    if (CurieIMU.motionDetected(Z_AXIS, POSITIVE)){
      Serial.println("Negative motion detected on Z-axis");
      //ledZ2.on();
    }
    if (CurieIMU.motionDetected(Z_AXIS, NEGATIVE)){
      Serial.println("Positive motion detected on Z-axis");
      //ledZ2.on();
    }
    readAllData();
  }  

  if (CurieIMU.getInterruptStatus(CURIE_IMU_SHOCK)) {
    if (CurieIMU.shockDetected(X_AXIS, POSITIVE)){
      Serial.println("Negative shock detected on X-axis");
      //ledX1.on();
    }
    if (CurieIMU.shockDetected(X_AXIS, NEGATIVE)){
      Serial.println("Positive shock detected on X-axis");
      //ledX1.on();
    }
    if (CurieIMU.shockDetected(Y_AXIS, POSITIVE)){
      Serial.println("Negative shock detected on Y-axis");
      //ledY1.on();
    }
    if (CurieIMU.shockDetected(Y_AXIS, NEGATIVE)){
      Serial.println("Positive shock detected on Y-axis");
      //ledY1.on();
    }
    if (CurieIMU.shockDetected(Z_AXIS, POSITIVE)){
      Serial.println("Negative shock detected on Z-axis");
      //ledZ1.on();
    }
    if (CurieIMU.shockDetected(Z_AXIS, NEGATIVE)){
      Serial.println("Positive shock detected on Z-axis");
      //ledZ2.on();
    }
    Serial.println("+.- .....SHOCK WAS DETECTED");
    readAllData();
  
  }

  if (CurieIMU.getInterruptStatus(CURIE_IMU_FREEFALL)) {
    Serial.println("free fall detected! ");
    Serial.println("--------------------");
    Serial.println("Very bad road");
    readAllData();
  }
}

void readAllData(){
    ledFall.on();
    readMotionData();
    readTemp();
    int b = findBenzene();
    showBenzene(b);
    int l = findLDR();
    showLDR(l);  
}

void readTemp(){
    int co2 = CO2_sensor();
    DHT_sensor();
    Blynk.virtualWrite(V1, co2);
    if(( co2 >= maxCO2 ) && ( temp >= maxTemp || hum <=minHum ) ) 
    {
      if(DEBUG)
      {
        Serial.println("Fire! Fire! Fire!");  
      }
      digitalWrite(alarm,HIGH);  
    }
    else{
      digitalWrite(alarm,LOW);  
    }  
}

int CO2_sensor()
{
  // Serial.println("get Co2");
  unsigned long Th = pulseIn(CO2_PIN, HIGH, 1004000);

  Th = Th / 1000;

  if (Th > 2)
  {
    unsigned long value = ((2000 * (Th - 2)) / (Th + (1004 - Th) - 4));
    value_CO2 += value;
    count_CO2 += 1;
  }

  if (DEBUG)
  {
    Serial.print("CO2 : ");
    Serial.print(value_CO2 / count_CO2);
    Serial.println("ppm     ");
  }

  return value_CO2 / count_CO2;
}

void DHT_sensor()
{
  // Serial.println("get Temperature,Humidity");
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if ( ( isnan(h) || isnan(t) || isnan(f) ) && DEBUG ) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  else{
    hum = h;
    temp = t;
    Blynk.virtualWrite(V5, h);
    Blynk.virtualWrite(V4, t);
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);
    if(DEBUG)
    {
      // DISPLAY DATA

      Serial.print("Humidity: ");
      Serial.print(h);
      Serial.print(" %\t");
      Serial.print("Temperature: ");
      Serial.print(t);
      Serial.print(" *C ");
      Serial.print(f);
      Serial.print(" *F\t");
      Serial.print("Heat index: ");
      Serial.print(hic);
      Serial.print(" *C ");
      Serial.print(hif);
      Serial.println(" *F");

    }
}

int findBenzene(){
  return analogRead(benzenePin);
}


void showBenzene(int value){
  if(DEBUG){
    Serial.print("Benzene(Analog Reading) :\t");
    Serial.println(value);
  }
  Blynk.virtualWrite(V2, value);
}

int findLDR(){
  return analogRead(LDR);
}


void showLDR(int value){
  if(DEBUG){
    Serial.print("Light Intensity :\t");
    Serial.println(value);
  }
  Blynk.virtualWrite(V3, value);
}

