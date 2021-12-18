


/*
Do not remove this comment, this comment is used by animus builder for the
build process
BUILDER_REQUIREMENT_START
keyType(31)
keyType(32)
keyType(33)
keyType(34)
BUILDER_REQUIREMENT_END
*/

/* keytype index
24: x movement (128 = 0)
25: y movement
26: wheel movement
27: click type (bitwise)
*/


class MouseKeys_ : public ModBase {
public:


byte XVal = 128;
byte YVal = 128;
byte WheelVal = 128;
boolean IsRunning = false;

void Loop()
{
  if (XVal == 128 && YVal == 128 && WheelVal == 128)
  {
    // do nothing
    if (IsRunning)
    {
      IsRunning = false;
      //Mouse.end();
    }

  }
  else
  {
    //Mouse.begin();
    IsRunning = true;
    Mouse.move(XVal-128,YVal-128,WheelVal-128);
  }

}

void KeyDown(char val, byte type)
{
  // ran when a key is pressed down


  // ran if this device's IS_PRIME flag is true
  if (IS_PRIME)
  {
    if (type == 31)
    {
      XVal += val - 128;


    }
    else if (type == 32)
    {
      YVal += val - 128;


    }
    else if (type == 33)
    {
      WheelVal += val - 128;

    }
    else if (type == 34)
    {
      if (val == 1)
      {
        Mouse.press(MOUSE_LEFT);
      }
      else if (val == 2)
      {
        Mouse.press(MOUSE_RIGHT);
      }
      else if (val == 3)
      {
        Mouse.press(MOUSE_MIDDLE);
      }
      else if (val == 4)
      {
        Mouse.press(MOUSE_PREV);
      }
      else if (val == 5)
      {
        Mouse.press(MOUSE_NEXT);
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
    if (type == 31)
    {
      XVal -= val - 128;

    }
    else if (type == 32)
    {
      YVal -= val -128;

    }
    else if (type == 33)
    {
      WheelVal -= val -128;

    }
    else if (type == 34)
    {
      if (val == 1)
      {
        Mouse.release(MOUSE_LEFT);
      }
      else if (val == 2)
      {
        Mouse.release(MOUSE_RIGHT);
      }
      else if (val == 3)
      {
        Mouse.release(MOUSE_MIDDLE);
      }
      else if (val == 4)
      {
        Mouse.release(MOUSE_PREV);
      }
      else if (val == 5)
      {
        Mouse.release(MOUSE_NEXT);
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
    Serial.print("MouseKeys");
  }

}

};
MouseKeys_ MouseKeys;
