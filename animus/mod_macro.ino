// completed 5/27/2016 blahlicus
// this library helps
// example serial command: uniqueksetmacro(0(4(97(0(0(98(0(0(99(0(0(100(0(0(97(0(1(98(0(1(99(0(1(100(0(1(0

/*
Do not remove this comment, this comment is used by animus builder for the
build process
BUILDER_REQUIREMENT_START
EEPROM(600,800)
Keytype(7)
BUILDER_REQUIREMENT_END
*/


class Macro_ : public ModBase {
public:

static const int macroMinAddr = 600;
static const int macroMaxAddr = 800;
static const byte macroMAX_MACROS = 12;
static const byte macroMAX_KEYS = 8;
static const byte macroSERIES_PADDING = 2;
static const byte macroKEY_DOWN = 0;
static const byte macroKEY_UP = 1;

static const byte macroDelay = 5;



void KeyDown(char val, byte type)
{
  // ran when a key is pressed down


  // ran if this device's IS_PRIME flag is true
  if (IS_PRIME)
  {
    if (type == 7)
    {
      // sends the macro sequence with id = val
      Send(val);
    }
  }
}

void SerialCommand(String input)
{
  // ran when serial command of input is sent to device from host

  // do not delete this as this is for the identifier
  if (input.startsWith("uniquekgetmods"))
  {
    Serial.print(tokenToString("macro"));
  }
  else if (input.startsWith("uniqueksetmacro"))
  {
    byte key, type, state;

    input = input.substring(input.indexOf('(')+1);
    byte id = input.substring(0, input.indexOf('(')).toInt();
    input = input.substring(input.indexOf('(')+1);
    byte length = input.substring(0, input.indexOf('(')).toInt();

    SetMacroLength(id, length);

    for (int i = 0; i < macroMAX_KEYS; i++)
    {
      input = input.substring(input.indexOf('(')+1);
      key = input.substring(0, input.indexOf('(')).toInt();
      SetKeyVal(id, i, key);
      input = input.substring(input.indexOf('(')+1);
      type = input.substring(0, input.indexOf('(')).toInt();
      SetKeyType(id, i, type);
      input = input.substring(input.indexOf('(')+1);
      state = input.substring(0, input.indexOf('(')).toInt();
      SetKeyDownUp(id, i, state);
    }

    input = input.substring(input.indexOf('(')+1);
    byte overflow = input.toInt();

    Serial.print("set macro(");
    Serial.print(id);
    Serial.print("(");
    Serial.println(length);
  }

}

void Send(byte id)
{
  if (id < macroMAX_MACROS)
  {
    byte length = GetMacroLength(id);
    for (byte i = 0; i < length; i++)
    {
      if (GetKeyDownUp(id, i) == macroKEY_DOWN)
      {
        PressKey( GetKeyVal(id, i), GetKeyType(id, i) );
      }
      else if (GetKeyDownUp(id, i) == macroKEY_UP)
      {
        ReleaseKey( GetKeyVal(id, i), GetKeyType(id, i) );
      }
      delay(macroDelay);
    }
    ReleaseAllKey();
  }
}

char GetKeyVal(byte id, byte key)
{
  char output = 0;
  if (id < macroMAX_MACROS)
  {
    int addr = GetMacroStartAddr(id);
    output = EEPROM.read(addr + macroSERIES_PADDING + key * 2);
  }
  return output;
}

void SetKeyVal(byte id, byte key, byte val)
{
  if (id < macroMAX_MACROS && key < macroMAX_KEYS)
  {
    int addr = GetMacroStartAddr(id);
    EEPROM.update(addr + macroSERIES_PADDING + key * 2, val);
  }
}

byte GetKeyType(byte id, byte key)
{
  char output = 0;
  if (id < macroMAX_MACROS && key < macroMAX_KEYS)
  {
    int addr = GetMacroStartAddr(id);
    output = EEPROM.read(addr + macroSERIES_PADDING + key * 2 + 1);
  }
  return output;
}

void SetKeyType(byte id, byte key, byte val)
{
  if (id < macroMAX_MACROS && key < macroMAX_KEYS)
  {
    int addr = GetMacroStartAddr(id);
    EEPROM.update(addr + macroSERIES_PADDING + key * 2 + 1, val);
  }
}


byte GetKeyDownUp(byte id, byte key)
{
  byte output = 2;
  if (id < macroMAX_MACROS)
  {
    int addr = GetMacroStartAddr(id);
    output = bitRead(EEPROM.read(addr), key);
  }
  return output;
}

void SetKeyDownUp(byte id, byte key, byte bitState)
{
  if (id < macroMAX_MACROS && key < macroMAX_KEYS)
  {
    int addr = GetMacroStartAddr(id);
    byte keyStates = EEPROM.read(addr);
    bitWrite(keyStates, key, bitState);
    EEPROM.update(addr, keyStates);
  }
}

byte GetMacroLength(byte id)
{
  byte output = 0;
  if (id < macroMAX_MACROS)
  {
    int addr = GetMacroStartAddr(id);
    output = EEPROM.read(addr + 1);
  }
  return output;
}

void SetMacroLength(byte id, byte newLength)
{
  if (id < macroMAX_MACROS && newLength <= macroMAX_KEYS)
  {
    int addr = GetMacroStartAddr(id);
    EEPROM.update(addr + 1, newLength);
  }
}

int GetMacroStartAddr(byte id)
{
    int addr = macroMinAddr + (id * (macroMAX_KEYS * 2 + macroSERIES_PADDING));
    return addr;
}


};
Macro_ Macro;
