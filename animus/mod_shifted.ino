/*
Do not remove this comment, this comment is used by animus builder for the
build process
BUILDER_REQUIREMENT_START
Keytype(11)
BUILDER_REQUIREMENT_END
*/


class Shifted_ : public ModBase {
public:

char Shifted = 0;
char LastVal = 0;

void KeyDown(char val, byte type)
{
  // ran when a key is pressed down


  // ran if this device's IS_PRIME flag is true
  if (IS_PRIME)
  {
    // shifted
    if (type == 11)
    {
      PressKey(225, 0);
      PressKey(val, 0);
      Shifted = val;
    }
  }
}

void PrePress(char val, byte type)
{
  if (IS_PRIME)
  {
    if (type == 0)
    {
      LastVal = val;
      if (Shifted)
      {
        ReleaseKey(Shifted, 0);
        ReleaseKey(225, 0);
        Shifted = 0;
      }
    }
    else if (type == 11)
    {
      if (val == LastVal)
      {
        ReleaseKey(LastVal, 0);
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
    // shifted
    if (type == 11)
    {
      ReleaseKey(val, 0);
      ReleaseKey(225, 0);
      Shifted = 0;
    }
  }
}

void SerialCommand(String input)
{
  // ran when serial command of input is sent to device from host

  // do not delete this as this is for the identifier
  if (input.startsWith("uniquekgetmods"))
  {
    Serial.print("Shifted");
  }

}

};
Shifted_ Shifted;
