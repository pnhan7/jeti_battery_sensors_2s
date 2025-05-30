/*
   -----------------------------------------------------------
                Jeti Test Sensor v 1.0
   -----------------------------------------------------------

    Tero Salminen RC-Thoughts.com (c) 2017 www.rc-thoughts.com

  -----------------------------------------------------------

    Simple test sensor with two potentiometers and two 3-pos
    switches. 

    Bothe sensora are selectable to work in three ranges: 
    0 - 100, 0 - 1000 and 0 - 10000. 

  -----------------------------------------------------------
    Shared under MIT-license by Tero Salminen (c) 2017
  -----------------------------------------------------------
*/

#include <EEPROM.h>
#include <stdlib.h>
#include "SoftwareSerialJeti.h"
#include "JETI_EX_SENSOR.h"

#define prog_char char PROGMEM
#define GETCHAR_TIMEOUT_ms 20

#ifndef JETI_RX
#define JETI_RX 3
#endif

#ifndef JETI_TX
#define JETI_TX 4
#endif

#define ITEMNAME_1 F("1")
#define ITEMTYPE_1 F("V")
#define ITEMVAL_1 &voltage1

#define ITEMNAME_2 F("2")
#define ITEMTYPE_2 F("V")
#define ITEMVAL_2 &voltage2

#define ITEMNAME_3 F("3")
#define ITEMTYPE_3 F("V")
#define ITEMVAL_3 &voltage3

#define ITEMNAME_4 F("4")
#define ITEMTYPE_4 F("V")
#define ITEMVAL_4 &voltage4


#define ABOUT_1 F(" Battery")
#define ABOUT_2 F("Battery")

SoftwareSerial JetiSerial(JETI_RX, JETI_TX);

void JetiUartInit() {
  JetiSerial.begin(9700);
}

void JetiTransmitByte(unsigned char data, boolean setBit9) {
  JetiSerial.set9bit = setBit9;
  JetiSerial.write(data);
  JetiSerial.set9bit = 0;
}

unsigned char JetiGetChar(void) {
  unsigned long time = millis();
  while (JetiSerial.available() == 0) {
    if (millis() - time > GETCHAR_TIMEOUT_ms)
      return 0;
  }
  int read = -1;
  if (JetiSerial.available() > 0) {
    read = JetiSerial.read();
  }
  long wait = (millis() - time) - GETCHAR_TIMEOUT_ms;
  if (wait > 0)
    delay(wait);
  return read;
}

JETI_Box_class JB;

unsigned char SendFrame() {
  boolean bit9 = false;
  for (int i = 0; i < JB.frameSize; i++) {
    if (i == 0)
      bit9 = false;
    else if (i == JB.frameSize - 1)
      bit9 = false;
    else if (i == JB.middle_bit9)
      bit9 = false;
    else
      bit9 = true;
    JetiTransmitByte(JB.frame[i], bit9);
  }
}

unsigned char DisplayFrame() {
  for (int i = 0; i < JB.frameSize; i++) {
  }
}

//uint8_t frame[10];
//short value = 27;

//#include <EEPROM.h>
#define R1 6800.0
#define R2 10000.0
#define VREF 5.0
//const int EEPROM_ADDR = 0;
// Precomputed constants
const float conversionConstant = VREF / 1023.0;  // Convert ADC value to voltage in V
const float dividerRatio = (R1 + R2) / R2;       // Voltage divider ratio

float correctionFactor = 1.0;
float voltage1 = 0.0;
float voltage2 = 0.0;
float voltage3 = 0.0;
float voltage4 = 0.0;
float readInputVoltage(int analogPin) {
  int rawADC = analogRead(analogPin);
  if(rawADC<500){
    return 0.0;
  }
  return rawADC * conversionConstant * dividerRatio * correctionFactor;
}

#define MAX_SCREEN 2
#define MAX_CONFIG 1
#define COND_LES_EQUAL 1
#define COND_MORE_EQUAL 2

void setup() {
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);

  pinMode(JETI_RX, OUTPUT);
  JetiUartInit();

  JB.JetiBox(ABOUT_1, ABOUT_2);
  JB.Init(F("Battery"));

  JB.addData(ITEMNAME_1, ITEMTYPE_1);
  JB.setValue(1, ITEMVAL_1, 2);

  JB.addData(ITEMNAME_2, ITEMTYPE_2);
  JB.setValue(2, ITEMVAL_2, 2);

  JB.addData(ITEMNAME_3, ITEMTYPE_3);
  JB.setValue(3, ITEMVAL_3, 2);

  JB.addData(ITEMNAME_4, ITEMTYPE_4);
  JB.setValue(4, ITEMVAL_3, 4);

  do {
    JB.createFrame(1);
    SendFrame();
    delay(GETCHAR_TIMEOUT_ms);
  } while (sensorFrameName != 0);
  digitalWrite(13, LOW);
}

int header = 0;
int lastbtn = 240;
int current_screen = 0;
int current_config = 0;
char temp[LCDMaxPos / 2];
char msg_line1[LCDMaxPos / 2];
char msg_line2[LCDMaxPos / 2];

void process_screens() {
  switch (current_screen) {
    case 0:
      {
        JB.JetiBox(ABOUT_1, ABOUT_2);
        break;
      }
    case MAX_SCREEN:
      {
        JB.JetiBox(ABOUT_1, ABOUT_2);
        break;
      }
  }
}

void loop() {

  voltage1 = readInputVoltage(A0);
  voltage2 = readInputVoltage(A1);
  voltage3 = readInputVoltage(A2);
  voltage4 = readInputVoltage(A3);

  unsigned long time = millis();
  SendFrame();
  time = millis();
  int read = 0;
  pinMode(JETI_RX, INPUT);
  pinMode(JETI_TX, INPUT_PULLUP);

  JetiSerial.listen();
  JetiSerial.flush();

  while (JetiSerial.available() == 0) {

    if (millis() - time > 5)
      break;
  }

  if (JetiSerial.available() > 0) {
    read = JetiSerial.read();

    if (lastbtn != read) {
      lastbtn = read;
      switch (read) {
        case 224:  // RIGHT
          break;
        case 112:  // LEFT
          break;
        case 208:  // UP
          break;
        case 176:  // DOWN
          break;
        case 144:  // UP+DOWN
          break;
        case 96:  // LEFT+RIGHT
          break;
      }
    }
  }

  if (current_screen != MAX_SCREEN)
    current_config = 0;
  process_screens();
  header++;
  if (header >= 5) {
    JB.createFrame(1);
    header = 0;
  } else {
    JB.createFrame(0);
  }

  long wait = GETCHAR_TIMEOUT_ms;
  long milli = millis() - time;
  if (milli > wait)
    wait = 0;
  else
    wait = wait - milli;
  pinMode(JETI_TX, OUTPUT);
}
/*
   -----------------------------------------------------------
                Jeti Test Sensor v 1.0
   -----------------------------------------------------------

    Tero Salminen RC-Thoughts.com (c) 2017 www.rc-thoughts.com

  -----------------------------------------------------------

    Simple test sensor with two potentiometers and two 3-pos
    switches. 

    Bothe sensora are selectable to work in three ranges: 
    0 - 100, 0 - 1000 and 0 - 10000. 

  -----------------------------------------------------------
    Shared under MIT-license by Tero Salminen (c) 2017
  -----------------------------------------------------------
*/

#include <EEPROM.h>
#include <stdlib.h>
#include "SoftwareSerialJeti.h"
#include "JETI_EX_SENSOR.h"

#define prog_char char PROGMEM
#define GETCHAR_TIMEOUT_ms 20

#ifndef JETI_RX
#define JETI_RX 3
#endif

#ifndef JETI_TX
#define JETI_TX 4
#endif

#define ITEMNAME_1 F("1")
#define ITEMTYPE_1 F("V")
#define ITEMVAL_1 &voltage1

#define ITEMNAME_2 F("2")
#define ITEMTYPE_2 F("V")
#define ITEMVAL_2 &voltage2

#define ITEMNAME_3 F("3")
#define ITEMTYPE_3 F("V")
#define ITEMVAL_3 &voltage3

#define ITEMNAME_4 F("4")
#define ITEMTYPE_4 F("V")
#define ITEMVAL_4 &voltage4


#define ABOUT_1 F(" Battery")
#define ABOUT_2 F("Battery")

SoftwareSerial JetiSerial(JETI_RX, JETI_TX);

void JetiUartInit() {
  JetiSerial.begin(9700);
}

void JetiTransmitByte(unsigned char data, boolean setBit9) {
  JetiSerial.set9bit = setBit9;
  JetiSerial.write(data);
  JetiSerial.set9bit = 0;
}

unsigned char JetiGetChar(void) {
  unsigned long time = millis();
  while (JetiSerial.available() == 0) {
    if (millis() - time > GETCHAR_TIMEOUT_ms)
      return 0;
  }
  int read = -1;
  if (JetiSerial.available() > 0) {
    read = JetiSerial.read();
  }
  long wait = (millis() - time) - GETCHAR_TIMEOUT_ms;
  if (wait > 0)
    delay(wait);
  return read;
}

JETI_Box_class JB;

unsigned char SendFrame() {
  boolean bit9 = false;
  for (int i = 0; i < JB.frameSize; i++) {
    if (i == 0)
      bit9 = false;
    else if (i == JB.frameSize - 1)
      bit9 = false;
    else if (i == JB.middle_bit9)
      bit9 = false;
    else
      bit9 = true;
    JetiTransmitByte(JB.frame[i], bit9);
  }
}

unsigned char DisplayFrame() {
  for (int i = 0; i < JB.frameSize; i++) {
  }
}

//uint8_t frame[10];
//short value = 27;

//#include <EEPROM.h>
#define R1 6800.0
#define R2 10000.0
#define VREF 5.0
//const int EEPROM_ADDR = 0;
// Precomputed constants
const float conversionConstant = VREF / 1023.0;  // Convert ADC value to voltage in V
const float dividerRatio = (R1 + R2) / R2;       // Voltage divider ratio

float correctionFactor = 1.0;
float voltage1 = 0.0;
float voltage2 = 0.0;
float voltage3 = 0.0;
float voltage4 = 0.0;
float readInputVoltage(int analogPin) {
  int rawADC = analogRead(analogPin);
  if(rawADC<500){
    return 0.0;
  }
  return rawADC * conversionConstant * dividerRatio * correctionFactor;
}

#define MAX_SCREEN 2
#define MAX_CONFIG 1
#define COND_LES_EQUAL 1
#define COND_MORE_EQUAL 2

void setup() {
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);

  pinMode(JETI_RX, OUTPUT);
  JetiUartInit();

  JB.JetiBox(ABOUT_1, ABOUT_2);
  JB.Init(F("Battery"));

  JB.addData(ITEMNAME_1, ITEMTYPE_1);
  JB.setValue(1, ITEMVAL_1, 2);

  JB.addData(ITEMNAME_2, ITEMTYPE_2);
  JB.setValue(2, ITEMVAL_2, 2);

  JB.addData(ITEMNAME_3, ITEMTYPE_3);
  JB.setValue(3, ITEMVAL_3, 2);

  JB.addData(ITEMNAME_4, ITEMTYPE_4);
  JB.setValue(4, ITEMVAL_3, 4);

  do {
    JB.createFrame(1);
    SendFrame();
    delay(GETCHAR_TIMEOUT_ms);
  } while (sensorFrameName != 0);
  digitalWrite(13, LOW);
}

int header = 0;
int lastbtn = 240;
int current_screen = 0;
int current_config = 0;
char temp[LCDMaxPos / 2];
char msg_line1[LCDMaxPos / 2];
char msg_line2[LCDMaxPos / 2];

void process_screens() {
  switch (current_screen) {
    case 0:
      {
        JB.JetiBox(ABOUT_1, ABOUT_2);
        break;
      }
    case MAX_SCREEN:
      {
        JB.JetiBox(ABOUT_1, ABOUT_2);
        break;
      }
  }
}

void loop() {

  voltage1 = readInputVoltage(A0);
  voltage2 = readInputVoltage(A1);
  voltage3 = readInputVoltage(A2);
  voltage4 = readInputVoltage(A3);

  unsigned long time = millis();
  SendFrame();
  time = millis();
  int read = 0;
  pinMode(JETI_RX, INPUT);
  pinMode(JETI_TX, INPUT_PULLUP);

  JetiSerial.listen();
  JetiSerial.flush();

  while (JetiSerial.available() == 0) {

    if (millis() - time > 5)
      break;
  }

  if (JetiSerial.available() > 0) {
    read = JetiSerial.read();

    if (lastbtn != read) {
      lastbtn = read;
      switch (read) {
        case 224:  // RIGHT
          break;
        case 112:  // LEFT
          break;
        case 208:  // UP
          break;
        case 176:  // DOWN
          break;
        case 144:  // UP+DOWN
          break;
        case 96:  // LEFT+RIGHT
          break;
      }
    }
  }

  if (current_screen != MAX_SCREEN)
    current_config = 0;
  process_screens();
  header++;
  if (header >= 5) {
    JB.createFrame(1);
    header = 0;
  } else {
    JB.createFrame(0);
  }

  long wait = GETCHAR_TIMEOUT_ms;
  long milli = millis() - time;
  if (milli > wait)
    wait = 0;
  else
    wait = wait - milli;
  pinMode(JETI_TX, OUTPUT);
}
