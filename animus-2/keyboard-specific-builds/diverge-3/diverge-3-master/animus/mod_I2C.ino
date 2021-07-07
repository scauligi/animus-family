/*
Do not remove this comment, this comment is used by animus builder for the
build process
BUILDER_REQUIREMENT_START
I2C
BUILDER_REQUIREMENT_END
*/

#define mod_modname I2C
#define modMethod(str) conca(mod_modname, str)

#define LEDBRIGHTSETTER I2CSetLEDBrightness

#include "Wire.h"

int I2CKeyLayer = 0;
int I2CTempLayer = 0;

void I2CStartup()
{
  Wire.begin();
}

void I2CLoop()
{



  if (KeyLayer != I2CKeyLayer)
  {
    I2CSetKeyLayer(KeyLayer);
    I2CKeyLayer = KeyLayer;
  }

  if (TempLayer != I2CTempLayer)
  {
    I2CSetTempLayer(TempLayer);
    I2CTempLayer = TempLayer;
  }


  byte slaveArray[31];
  byte slaveCount = 0;
  boolean slaveExists = false;
  Wire.requestFrom(8, 31);    // request 31 bytes from slave device #8
  while (Wire.available())
  {
    // slave may send less than requested
    byte out = Wire.read(); // receive a byte
    slaveArray[slaveCount] = out;
    slaveExists = true;
    slaveCount++;
  }
  if (slaveExists)
  {
    byte reX, reY;
    slaveCount = slaveArray[0];
    for (int i = 1; i < slaveCount; i=i+3)
    {
      char cinput = slaveArray[i];
      byte tinput = slaveArray[i+1];
      byte type = slaveArray[i+2];
      if (type == 3)
      {
        reX = cinput;
        reY = tinput;
      }
      else if (type == 2)
      {
        ModPrePress(cinput, tinput);
      }
      else if (type == 5)
      {
        if (TempLayer != I2CTempLayer)
        {
          I2CSetTempLayer(TempLayer);
          I2CTempLayer = TempLayer;
          Wire.beginTransmission(8);
          Wire.write(7);    // request updated key press due to layer change
          Wire.write(reX);
          Wire.write(reY);
          Wire.endTransmission();
          Wire.requestFrom(8, 3);    // request 3 bytes from slave device #8
          Wire.read();    // discard count; we don't need it
          cinput = Wire.read();
          tinput = Wire.read();
        }
        PressKey(cinput, tinput);
      }
      else if (type == 1)
      {
        ReleaseKey(cinput, tinput);
      }
    }
  }

  if (CheckMillis())
  {

  }

}


void I2CKeyDown(char val, byte type)
{

  #ifdef LEDEXISTS
  if (type == 10)
  {

    I2CSetLEDBrightness(BledPWMBrightness);
  }
  #endif
}

void I2CKeyUp(char val, byte type)
{

}

void I2CSerial(String input)
{
  if (input.startsWith("uniqueksetsubkey"))
  {
    input = input.substring(input.indexOf('(')+1);
    byte x = input.substring(0, input.indexOf('(')).toInt();
    input = input.substring(input.indexOf('(')+1);
    byte y = input.substring(0, input.indexOf('(')).toInt();
    input = input.substring(input.indexOf('(')+1);
    byte z = input.substring(0, input.indexOf('(')).toInt();
    input = input.substring(input.indexOf('(')+1);
    byte val = input.substring(0, input.indexOf('(')).toInt();
    input = input.substring(input.indexOf('(')+1);
    byte type = input.toInt();
    I2CSetKey(x, y, z, val, type);
    Serial.print("sub key set: ");
    Serial.print(x);
    Serial.print(", ");
    Serial.print(y);
    Serial.print(", ");
    Serial.print(z);
    Serial.print(", ");
    Serial.print(val);
    Serial.print(", ");
    Serial.println(type);
  }
  else if (input.startsWith("uniqueksetsubeeprom"))
  {
    input = input.substring(input.indexOf('(')+1);
    int addr = input.substring(0, input.indexOf('(')).toInt();
    input = input.substring(input.indexOf('(')+1);
    byte val = input.toInt();
    I2CSetEEPROM(addr, val);
    Serial.print("Wrote to sub EEPROM: ");
    Serial.print(addr);
    Serial.print(", ");
    Serial.println(val);
  }
  else if (input.startsWith("uniqueksetsublay"))
  {
    input = input.substring(input.indexOf('(')+1);
    byte val = input.toInt();

    if (val <= (500  / (ROW * COL)))
    {
      I2CSetEEPROM(1023, val);
      Serial.print("Edited layer count to: ");
      Serial.println(val);
    }
    else
    {
      Serial.println("Layer count edit aborted, insufficient EEPROM");
    }

  }
  else if (input.startsWith("uniqueksetsubbright"))
  {
    input = input.substring(input.indexOf('(')+1);
    byte val = input.toInt();
    I2CSetLEDBrightness(val);
    Serial.println("Set sub brightness");
  }
  else if (input.startsWith("uniquekgetmods"))
  {
    Serial.print("I2C");
  }
}



/* references
1: set TempLayer
2: rebind
3: set number of layers
4: set EEPROM
5: set KeyLayer
6: set brightness
7: request key press
*/
void I2CSetKeyLayer(byte input)
{
  Wire.beginTransmission(8);
  Wire.write(5);
  Wire.write(input);
  Wire.endTransmission();
}


void I2CSetTempLayer(byte input)
{
  Wire.beginTransmission(8);
  Wire.write(1);
  Wire.write(input);
  Wire.endTransmission();
}

void I2CSetLEDBrightness(byte input)
{
  Wire.beginTransmission(8);
  Wire.write(6);
  Wire.write(input);
  Wire.endTransmission();
}

void I2CSetKey(byte x, byte y, byte z, char inputChar, byte inputType)
{
  byte type = 2;
  Wire.beginTransmission(8);
  Wire.write(type);
  Wire.write(x);
  Wire.write(y);
  Wire.write(z);
  Wire.write(inputChar);
  Wire.write(inputType);
  Wire.endTransmission();
}

void I2CSetLayer(byte input)
{
  Wire.beginTransmission(8);
  Wire.write(3);
  Wire.write(input);
  Wire.endTransmission();
}

void I2CSetEEPROM(int addr, byte val)
{

  Wire.beginTransmission(8);
  Wire.write(4);
  Wire.write(val);
  while (addr >255)
  {
    Wire.write(255);
    addr = addr - 255;
  }
  Wire.write(addr);
  Wire.endTransmission();
}



void modMethod(PressCoord)(byte x, byte y) { }
void modMethod(PrePress)(char val, byte type) { }
#undef mod_modname
#undef modMethod
// TODO
// bool I2CDetectDevice(byte addr)
