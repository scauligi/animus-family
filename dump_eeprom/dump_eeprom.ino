#include "EEPROM.h"
#include "Wire.h"
#include <avr/wdt.h>

// baud: dont change this it messes with the GUI interface
int BaudRate = 19200;

void setup()
{
  Serial.begin(BaudRate);

  // serial delay
  delay(300);
  Wire.begin();
}

void loop()
{
  // checks serials
  TestSerial();
  WireTest();
}

// serial data
String inputData = "";

// serial functions start

void TestSerial()
{
  if (Serial.available()>0)
  {
    char inputByte = (char)Serial.read();

    if (inputByte != ' ')
    {
      inputData += inputByte;
    }
    else
    {
      SerialCommand(inputData);
      inputData = "";
    }

    if (inputData.length() > 100)
    {
      inputData = "";
    }
  }
}

void SetTempLayer(byte input)
{
  Wire.beginTransmission(8);
  Wire.write(1);
  Wire.write(input);
  Wire.endTransmission();
}

const byte ROWS = 5;
const byte COLS = 8;

void SerialCommand(String input)
{
  byte b;
  if (input == "dumpme")
  {
    for (int i = 0; i < 1024; i++) {
      b = EEPROM.read(i);
      Serial.write(b);
    }
  }
  else if (input == "dumpsubkeys") {
    byte numLayers = EEPROM.read(1023);
    for (byte z = 0; z < numLayers; z++) {
      SetTempLayer(z);
      for (byte y = 0; y < ROWS; y++) {
        for (byte x = 0; x < COLS; x++) {
          Wire.beginTransmission(8);
          Wire.write(7);    // request updated key press due to layer change
          Wire.write(x);
          Wire.write(y);
          Wire.endTransmission();
          Wire.requestFrom(8, 3);    // request 3 bytes from sub device #8
          Wire.read();    // discard count; we don't need it
          byte cinput = Wire.read();
          byte tinput = Wire.read();
          Serial.write(cinput);
          Serial.write(tinput);
        }
      }
    }
  }
  else {
    Serial.println("what?");
  }
}

void WireTest() {
  byte subArray[31];
  byte subCount = 0;
  boolean subExists = false;
  Wire.requestFrom(8, 31);    // request 31 bytes from sub device #8
  while (Wire.available())
  {
    // sub may send less than requested
    byte out = Wire.read(); // receive a byte
    subArray[subCount] = out;
    subExists = true;
    subCount++;
  }
  if (subExists)
  {
    byte reX, reY;
    subCount = subArray[0];
    for (int i = 1; i < subCount; i=i+3)
    {
      char cinput = subArray[i];
      byte tinput = subArray[i+1];
      byte type = subArray[i+2];
      if (type == 3)
      {
        reX = cinput;
        reY = tinput;
        if (reX == 6 && reY == 0) {
          //((void(*)(void))(0))();
          uint16_t bootKey = 0x7777;
          volatile uint16_t * const bootKeyPtr = (volatile uint16_t *)0x0800;

          // Stash the magic key
          *bootKeyPtr = bootKey;

          // Set a watchdog timer
          wdt_enable(WDTO_120MS);

          while(1) {}
        }
      }
    }
  }
}
