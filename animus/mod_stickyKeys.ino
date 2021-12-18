/*
Do not remove this comment, this comment is used by animus builder for the
build process
BUILDER_REQUIREMENT_START
keyType(30)
BUILDER_REQUIREMENT_END
*/

class StickyKeys_ : public ModBase {
public:


static const int STICKY_KEY_MAX = 20;

boolean State[STICKY_KEY_MAX] = {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false};

boolean PressedKey = false;

void KeyDown(char val, byte type)
{
  // ran when a key is pressed down


  // ran if this device's IS_PRIME flag is true
  if (IS_PRIME)
  {
    // ctrlDualRole
    if (type == 30)
    {


      State[val] = !State[val];
      if (State[val])
      {


        if (val == 0)
        {
          AnimusKeyboard.press(224); // ctrl
        }
        else if (val == 1)
        {
          AnimusKeyboard.press(225); // shift
        }
        else if (val == 2)
        {
          AnimusKeyboard.press(226); //alt
        }
        else if (val == 3)
        {
          AnimusKeyboard.press(230); //altgr
        }
        else if (val > 3 && val < STICKY_KEY_MAX)
        {
          TempLayer = val-3;
        }
        PressedKey = false;
      }
      else
      {
        ReleaseAll();
      }


    }
    else
    {
      ReleaseAll();
    }
  }
}

void ReleaseAll()
{

    PressedKey = true;
    for (byte i = 0; i < STICKY_KEY_MAX; i++)
    {
      if (State[i])
      {
        if (i == 0)
        {
          AnimusKeyboard.release(224); // ctrl
        }
        else if (i == 1)
        {
          AnimusKeyboard.release(225); // shift
        }
        else if (i == 2)
        {
          AnimusKeyboard.release(226); //alt
        }
        else if (i == 3)
        {
          AnimusKeyboard.release(230); //altgr
        }
        else if (i > 3 && i < STICKY_KEY_MAX)
        {
          TempLayer = KeyLayer;
        }
        State[i] = false;
      }
    }
}

void SerialCommand(String input)
{
  // ran when serial command of input is sent to device from host

  // do not delete this as this is for the identifier
  if (input.startsWith("uniquekgetmods"))
  {
    Serial.print("StickyKeys");
  }

}

};
StickyKeys_ StickyKeys;
