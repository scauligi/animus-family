
class ZCauligi_ : public ModBase {
  public:

byte WinDown = 0;
int16_t MouseTimer[4];
byte MouseCount[4];
byte SafeEnterLayer = -1;
boolean SafeToEnter = true;
boolean Shiftiness = false;
boolean LshiftDown = false;
boolean RshiftDown = false;

void Startup()
{
  // things to run during hardware startup
  for (byte i = 0; i < 4; i++)
  {
    MouseTimer[i] = -1;
    MouseCount[i] = 0;
  }
}

void Loop()
{
  for (byte i = 0; i < 4; i++)
  {
    if (MouseTimer[i] > 0)
    {
      MouseTimer[i]--;
    }
    else if (MouseTimer[i] == 0)
    {
      MouseTimer[i] = -1;
    }
  }
}

void RestoreShiftiness() {
  Shiftiness = 0;
  if (LshiftDown)
    AnimusKeyboard.press(225);
  if (RshiftDown)
    AnimusKeyboard.press(229);
}

void PrePress(char val, byte type) {
  if (IS_PRIME) {
    if (Shiftiness)
      RestoreShiftiness();
  }
}


void KeyDown(char val, byte type)
{
  // ran when a key is pressed down


  // ran if this device's IS_PRIME flag is true
  if (IS_PRIME)
  {
    if (type != 1 && type != 5) // not switching layers
    {
      if (TempLayer == SafeEnterLayer)
      {
        if (!((type == 36 && val == 1) // not the safe enter key
              || (type == 0 && val == 43))) // not the tab key
        {
          SafeToEnter = false;
        }
      }
    }

    if (type == 0) {
      if (val == 225) {
        LshiftDown = true;
      } else if (val == 229) {
        RshiftDown = true;
      }
    }

    else if (type == 1 || type == 5) { // switching layers
      SafeToEnter = true;
    }

    // double tap mouse keys
    else if (type == 31 || type == 32)
    {
      int8_t m = KeyToMouse(val, type);
      byte i = m & B11;

      if (MouseTimer[i] < 0)
        MouseCount[i] = 0;
      MouseCount[i]++;
      byte mouseSpeed = 1 << (MouseCount[i] - 1);
      AdjustMouse(i, mouseSpeed - 1);
    }

    // custom keys
    else if (type == 36)
    {
      // safe enter
      if (val == 1) {
        if (SafeToEnter)
        {
          if (SafeEnterLayer != TempLayer)
            SafeEnterLayer = TempLayer;
          AnimusKeyboard.press(40); // enter
        }
        else
        {
          AnimusKeyboard.press(44); // space
        }
      }

      // slasher
      else if (val == 2) {
        if (LshiftDown || RshiftDown) {
          AnimusKeyboard.release(225);
          AnimusKeyboard.release(229);
          Shiftiness = true;
          PressKey(49, 0); // backslash
        } else {
          PressKey(56, 0); // forward slash
        }
      }

      // dotcomm
      else if (val == 3) {
        if (LshiftDown || RshiftDown) {
          AnimusKeyboard.release(225);
          AnimusKeyboard.release(229);
          Shiftiness = true;
          PressKey(54, 0); // comma
        } else {
          PressKey(55, 0); // period
        }
      }

      // nullwin
      else if (val == 4)
      {
        PressKey(227, 0); // Lwin
        WinDown = TempLayer;
        TempLayer = KeyLayer;
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
    if (type == 0) {
      if (val == 225) {
        LshiftDown = false;
      } else if (val == 229) {
        RshiftDown = false;
      }
    }

    // when you release that layer key
    else if (type == 1 && val == WinDown)
    {
      ReleaseKey(227, 0); // Lwin
      WinDown = 0;
    }

    else if (type == 31 || type == 32)
    {
      int8_t m = KeyToMouse(val, type);
      byte i = m & B11;

      byte mouseSpeed = 1 << (MouseCount[i] - 1);
      AdjustMouse(i, -(mouseSpeed - 1));

      MouseTimer[i] = 5 * TapDance.TIMER_MULTIPLIER;
    }

    else if (type == 36) {
      if (val == 1) {
        AnimusKeyboard.release(40);
        AnimusKeyboard.release(44);
      }

      // slasher
      else if (val == 2) {
        ReleaseKey(49, 0); // backslash
        ReleaseKey(56, 0); // forward slash
        RestoreShiftiness();
      }

      // dotcomm
      else if (val == 3) {
        ReleaseKey(54, 0); // comma
        ReleaseKey(55, 0); // period
        RestoreShiftiness();
      }
    }
  }
}

void SerialCommand(String input)
{
  // ran when serial command of input is sent to device from host

  // do not delete this as this is for the identifier
  if (input.startsWith("uniquekgetmods"))
  {
    Serial.print("ZCauligi");
  }

}

void AdjustMouse(byte i, int8_t amt)
{
  switch (i)
  {
    case 0: // up
      MouseKeys.YVal -= amt;
      break;
    case 1: // down
      MouseKeys.YVal += amt;
      break;
    case 2: // left
      MouseKeys.XVal -= amt;
      break;
    case 3: // right
      MouseKeys.XVal += amt;
      break;
  }
}

int8_t KeyToMouse(char val, byte type)
{
  byte i;
  int8_t amt = val - 128;
  if (type == 32)
  {
    if (amt < 0)
      i = 0;
    else if (amt > 0)
      i = 1;
  }
  else if (type == 31)
  {
    if (amt < 0)
      i = 2;
    else if (amt > 0)
      i = 3;
  }
  return (amt << 2) | i;
}

};
extern ZCauligi_ ZCauligi;
