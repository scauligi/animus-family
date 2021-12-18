/*
Do not remove this comment, this comment is used by animus builder for the
build process
BUILDER_REQUIREMENT_START
I2C
BUILDER_REQUIREMENT_END
*/


#include "Wire.h"

class I2CSub_ : public ModBase {
  public:

// sub data
byte subCount = 1;
static const byte MAX_SUB_COUNT = 31;
byte subArray[MAX_SUB_COUNT];

void Startup()
{
  if (!IS_PRIME) {
    Wire.begin(8);
    Wire.onRequest(wire_onrequest);
    Wire.onReceive(wire_onreceive);
  }
}


void requestEvent()
{
  subArray[0] = subCount;
  Wire.write(subArray, subCount);
  subCount = 1;
}

void receiveEvent(int numBytes)
{

  byte type = Wire.read();
  if (type == 1)
  {
    byte input = Wire.read();
    TempLayer = input;
  }
  else if (type == 2)
  {
    byte x = Wire.read();
    byte y = Wire.read();
    byte z = Wire.read();
    char inputChar = Wire.read();
    byte inputType = Wire.read();
    SetEEPROM(x, y, z, inputChar, inputType);
  }
  else if (type == 3)
  {
    byte input = Wire.read();
    EEPROM.update(1023, input);
  }
  else if (type == 4)
  {
    byte val = Wire.read();
    int addr = 0;
    while (0 < Wire.available())
    {
      addr = addr + Wire.read();
    }
    EEPROM.update(addr, val);
  }
  else if (type == 5)
  {
    byte input = Wire.read();
    KeyLayer = input;
  }
  else if (type == 6)
  {
    byte input = Wire.read();
  }
  else if (type == 7)
  {
    byte x = Wire.read();
    byte y = Wire.read();
    LayerState[x][y] = TempLayer;
    if (subCount < MAX_SUB_COUNT)
    {
      subArray[subCount] = GetValEEPROM(x, y, TempLayer);
      subCount++;
      subArray[subCount] = GetTypeEEPROM(x, y, TempLayer);
      subCount++;
    }
  }
}

/* references
1: set TempLayer
2: rebind
3: set number of layers
4: set EEPROM
5: set KeyLayer
*/

void PressCoord(byte x, byte y)
{
  if (!IS_PRIME) {
    if (subCount < MAX_SUB_COUNT)
    {
      subArray[subCount] = x;
      subCount++;
      subArray[subCount] = y;
      subCount++;
      subArray[subCount] = 3;
      subCount++;
    }
  }
}

void PrePress(char val, byte type)
{
  if (!IS_PRIME) {
    if (subCount < MAX_SUB_COUNT)
    {
      subArray[subCount] = val;
      subCount++;
      subArray[subCount] = type;
      subCount++;
      subArray[subCount] = 2;
      subCount++;
    }
  }
}

void KeyDown(char val, byte type)
{
  if (!IS_PRIME) {
    if (subCount < MAX_SUB_COUNT)
    {
      subArray[subCount] = val;
      subCount++;
      subArray[subCount] = type;
      subCount++;
      subArray[subCount] = 5;
      subCount++;
    }
  }
}

void KeyUp(char val, byte type)
{
  if (!IS_PRIME) {
    if (subCount < MAX_SUB_COUNT)
    {
      subArray[subCount] = val;
      subCount++;
      subArray[subCount] = type;
      subCount++;
      subArray[subCount] = 1;
      subCount++;
    }
  }
}

void SerialCommand(String input)
{
  if (input.startsWith("uniquekgetmods"))
  {
    Serial.print("I2CSub");
  }
}

};
I2CSub_ I2CSub;

void wire_onrequest() {
  I2CSub.requestEvent();
}

void wire_onreceive(int numBytes) {
  I2CSub.receiveEvent(numBytes);
}
