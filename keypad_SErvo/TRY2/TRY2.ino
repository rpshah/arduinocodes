#include <Keypad.h>
#include <Servo.h>
#include<Wire.h>

Servo myservoA;
Servo myservoB;
Servo myservoC;

int posA = 0;
int posC = 0;
int posB = 0;



const byte ROWS = 4; //four rows
const byte COLS = 4; //three columns
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {5, 4, 3, 2}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {8, 7, 6, 9}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void setup()
{
  Serial.begin(9600);
  myservoA.attach(12);
  myservoB.attach(12);
  myservoC.attach(12);

}

void loop() {
KEY:
  char key = keypad.getKey();
  if (key == 0)
    goto KEY;
  delay(500);
KEY2:
  char key2 = keypad.getKey();
  if (key2 == 0)
    goto KEY2;

  int v1 = key - 48;
  int v2 = key2 - 48;
  int value = (v1 * 10) + v2;

  Serial.println(value);

VKEY:
  char vkey = keypad.getKey();
  if (vkey == 0)
    goto VKEY;
  delay(500);
VKEY2:
  char vkey2 = keypad.getKey();
  if (vkey2 == 0)
    goto VKEY2;

  int vv1 = vkey - 48;
  int vv2 = vkey2 - 48;
  int vvalue = (vv1 * 10) + vv2;

  Serial.println(vvalue);

VVKEY:
  char vvkey = keypad.getKey();
  if (vvkey == 0)
    goto VVKEY;
  delay(500);
VVKEY2:
  char vvkey2 = keypad.getKey();
  if (vvkey2 == 0)
    goto VVKEY2;

  int vvv1 = vvkey - 48;
  int vvv2 = vvkey2 - 48;
  int vvvalue = (vvv1 * 10) + vvv2;

  Serial.println(vvvalue);


RKEY:
  char rkey = keypad.getKey();
  if (rkey == 0)
    goto RKEY;

  Serial.println(rkey);


  if (true)
  {
    //   Serial.println("WALL");

    for (posA = 0; posA <= 90; posA += 1)
    {
      myservoA.write(posA);
      delay(15);

    }

    delay(value*100);


    for (posA = 90; posA >= 0; posA -= 1)
    {
      myservoA.write(posA);
      delay(15);
    }


    delay(500);


    for (posB = 0; posB <= 90; posB += 1)
    {
      myservoB.write(posB);
      delay(15);

    }

    delay(vvalue*100);

    for (posB = 90; posB >= 0; posB -= 1)
    {
      myservoB.write(posB);
      delay(15);
    }

    delay(500);



    for (posC = 0; posC <= 90; posC += 1)
    {
      myservoC.write(posC);
      delay(15);

    }

    delay(vvvalue*100);



    for (posC = 90; posC >= 0; posC -= 1)
    {
      myservoC.write(posC);
      delay(15);
    }
    delay(500);
    //}
  }
}

