// completed 5/27/2016 blahlicus
// this library enables a special key called a tap dance key, allowing one physical button to function as a number of keys depending on the number of taps
// example serial command: uniqueksettapdance(0(3(25(4(0(5(0(6(0

/*
Do not remove this comment, this comment is used by animus builder for the
build process
BUILDER_REQUIREMENT_START
EEPROM(842,982)
Keytype(19)
BUILDER_REQUIREMENT_END
*/




class TapDance_ : public ModBase {
public:


static const byte TIMER_MULTIPLIER = 10;
static const int MAX_ADDR = 842;
static const int MIN_ADDR = 982;
static const byte MAX_INDEX = 20;
static const byte MAX_KEYS = 3;
static const byte PADDING = 2;

int16_t PressTimer[MAX_INDEX];
byte PressCount[MAX_INDEX];
boolean KeyIsDown[MAX_INDEX];



void Startup()
{
  // things to run during hardware startup
  for (byte i = 0; i < MAX_INDEX; i++)
  {
    PressCount[i] = 0;
    PressTimer[i] = 0;
    KeyIsDown[i] = false;
  }
}

void Loop()
{
  if (IS_PRIME)
  {
    for (byte i = 0; i < MAX_INDEX; i++)
    {


      if (PressCount[i] > 0)
      {
        if (PressTimer[i] > 0)
        {
          PressTimer[i]--;
        }
        else if (PressTimer[i] == 0)
        {
          FireTapDance(i);
          if (!KeyIsDown[i])
          {
            KeyUp(i, 19);
          }
        }
      }
    }
  }


}

void KeyDown(char val, byte type)
{
  // ran when a key is pressed down
  // ran if this device's IS_PRIME flag is true
  if (IS_PRIME)
  {
    if (type == 19)
    {
      if (PressCount[val] < MAX_KEYS)
      {
        byte count = ++PressCount[val]; // count gets the *new* count value
        PressTimer[val] = GetTimeout(val) * TIMER_MULTIPLIER;
        KeyIsDown[val] = true;

        if (count == MAX_KEYS)
        {
          // no more tap dance keys; fire immediately
          FireTapDance(val);
        }
        else
        {
          // check the key that would come after this one
          char tdVal = GetKeyVal(val, count);
          byte tdType = GetKeyType(val, count);

          if (tdVal == 0 && tdType == 0)
          {
            // no more tap dance keys; fire immediately
            FireTapDance(val);
          }
        }
      }
    }
  }
}

void PrePress(char val, byte type)
{
  if (IS_PRIME)
  {
    for (byte i = 0; i < MAX_INDEX; i++)
    {
      if ((type != 19 || val != i) && PressTimer[i] > 0)
      {
        FireTapDance(i);
        if (!KeyIsDown[i])
        {
          KeyUp(i, 19);
        }
      }
    }
  }
}

void KeyUp(char val, byte type)
{
  // ran when a key is pressed up


  // ran if this device's IS_PRIME flag is true
  if (IS_PRIME)
  {
    if (type == 19)
    {
      // if key has been fired
      if (PressTimer[val] < 0)
      {
        byte count = PressCount[val] - 1;
        char tdVal = GetKeyVal(val, count);
        byte tdType = GetKeyType(val, count);

        ReleaseKey(tdVal, tdType);
        PressCount[val] = 0;
        PressTimer[val] = 0;
      }
      KeyIsDown[val] = false;
    }
  }
}

void SerialCommand(String input)
{
  // ran when serial command of input is sent to device from host

  // do not delete this as this is for the identifier
  if (input.startsWith("uniquekgetmods"))
  {
    Serial.print("tapDance");
  }
  else if (input.startsWith("uniqueksettapdance"))
  {
    input = input.substring(input.indexOf('(')+1);
    byte id = input.substring(0, input.indexOf('(')).toInt();
    input = input.substring(input.indexOf('(')+1);
    byte length = input.substring(0, input.indexOf('(')).toInt();
    input = input.substring(input.indexOf('(')+1);
    byte delay = input.substring(0, input.indexOf('(')).toInt();
    input = input.substring(input.indexOf('(')+1);
    byte key0val = input.substring(0, input.indexOf('(')).toInt();
    input = input.substring(input.indexOf('(')+1);
    byte key0type = input.substring(0, input.indexOf('(')).toInt();
    input = input.substring(input.indexOf('(')+1);
    byte key1val = input.substring(0, input.indexOf('(')).toInt();
    input = input.substring(input.indexOf('(')+1);
    byte key1type = input.substring(0, input.indexOf('(')).toInt();
    input = input.substring(input.indexOf('(')+1);
    byte key2val = input.substring(0, input.indexOf('(')).toInt();
    input = input.substring(input.indexOf('(')+1);
    byte key2type = input.toInt();

    SetLength(id, length);
    SetTimeout(id, delay);
    SetKeyVal(id, 0, key0val);
    SetKeyVal(id, 1, key1val);
    SetKeyVal(id, 2, key2val);
    SetKeyType(id, 0, key0type);
    SetKeyType(id, 1, key1type);
    SetKeyType(id, 2, key2type);



    Serial.print("set tapdance(");
    Serial.print(id);
    Serial.print("(");
    Serial.print(length);
    Serial.print("(");
    Serial.print(delay);
    Serial.print("(");
    Serial.print(key0val);
    Serial.print("(");
    Serial.print(key0type);
    Serial.print("(");
    Serial.print(key1val);
    Serial.print("(");
    Serial.print(key1type);
    Serial.print("(");
    Serial.print(key2val);
    Serial.print("(");
    Serial.println(key2type);
  }

}

void FireTapDance(byte id)
{
  byte count = PressCount[id] - 1;
  char val = GetKeyVal(id, count);
  byte type = GetKeyType(id, count);
  PressTimer[id] = -1; // signal that the key has fired
  PressKey(val, type);
}

byte GetLength(byte id)
{

  byte output = 0;
  if (id < MAX_INDEX)
  {
    int addr = GetStartAddr(id);
    byte main = EEPROM.read(addr);
    output = main & 3;
  }
  return output;
}

void SetLength(byte id, byte val)
{

  if (id < MAX_INDEX)
  {
    int addr = GetStartAddr(id);
    EEPROM.update(addr, val + (GetTimeout(id) << 2) );
  }
}

byte GetTimeout(byte id)
{

  byte output = 0;
  if (id < MAX_INDEX)
  {
    int addr = GetStartAddr(id);
    byte main = EEPROM.read(addr);
    output = (main >> 2) & 63;
  }
  return output;
}

void SetTimeout(byte id, byte val)
{

  if (id < MAX_INDEX)
  {
    int addr = GetStartAddr(id);
    EEPROM.update(addr, GetLength(id) + (val << 2) );
  }
}


// key LL functions start
char GetKeyVal(byte id, byte key)
{
  byte output = 0;
  if (id < MAX_INDEX && key < MAX_KEYS)
  {
    int addr = GetStartAddr(id);
    output = EEPROM.read(addr + PADDING + key * 2);
  }
  return output;
}

void SetKeyVal(byte id, byte key, byte val)
{
  if (id < MAX_INDEX && key < MAX_KEYS)
  {
    int addr = GetStartAddr(id);
    EEPROM.update(addr + PADDING + key * 2, val);
  }
}

byte GetKeyType(byte id, byte key)
{
  byte output = 0;
  if (id < MAX_INDEX && key < MAX_KEYS)
  {
    int addr = GetStartAddr(id);
    output = EEPROM.read(addr + PADDING + key * 2 + 1);
  }
  return output;
}

void SetKeyType(byte id, byte key, byte val)
{
  if (id < MAX_INDEX && key < MAX_KEYS)
  {
    int addr = GetStartAddr(id);
    EEPROM.update(addr + PADDING + key * 2 + 1, val);
  }
}

/* not used for this set
byte GetLength(int id)
{
byte output = 0;
if (id < MAX_INDEX)
{
int addr = GetStartAddr(id);
output = EEPROM.read(addr + 1);
}
return output;
}

void SetLength(int id, byte newLength)
{
if (id < MAX_INDEX && newLength <= MAX_KEYS)
{
int addr = GetStartAddr(id);
EEPROM.update(addr + 1, newLength);
}
}
*/

int GetStartAddr(byte id)
{
  int addr = MAX_ADDR + (id * (MAX_KEYS * 2 + PADDING));
  return addr;
}

// key LL functions stop

};
TapDance_ TapDance;
