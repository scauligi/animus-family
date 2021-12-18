/*
Do not remove this comment, this comment is used by animus builder for the
build process
BUILDER_REQUIREMENT_START
Keytype(20)
Keytype(21)
Keytype(22)
Keytype(23)
Keytype(24)
Keytype(25)
Keytype(26)
Keytype(27)
Keytype(28)
Keytype(29)
Keytype(35)
BUILDER_REQUIREMENT_END
*/


class DualRoles_ : public ModBase {
public:


void KeyDown(char val, byte type)
{
  // ran when a key is pressed down


  // ran if this device's IS_PRIME flag is true
  if (IS_PRIME)
  {
    // ctrlDualRole
    if (type == 20)
    {
      PressKey(224, 0);
      PressedKey = false;
    }
    else if (type == 21) // shiftDualRolee
    {
      PressKey(225, 0);
      PressedKey = false;
    }
    else if (type == 22) // alt
    {
      PressKey(226, 0);
      PressedKey = false;
    }
    else if (type == 23) // altgr
    {
      PressKey(230, 0);
      PressedKey = false;
    }
    else if (type > 23 && type < 30) // fn
    {
      PressedKey = false;
      TempLayer = type - 23;
    }
    else if (type == 35) // win
    {
      PressKey(227, 0);
      PressedKey = false;
    }
  }
}

void KeyUp(char val, byte type)
{
  // ran when a key is pressed up


  // ran if this device's IS_PRIME flag is true
  if (IS_PRIME)
  {
    if (type >= 20 && (type < 30 || type == 35))
    {

      // ctrlDualRole
      if (type == 20)
      {
        ReleaseKey(224, 0);
      }
      else if (type == 21) // shiftDualRolee
      {
        ReleaseKey(225, 0);
      }
      else if (type == 22) // alt
      {
        ReleaseKey(226, 0);
      }
      else if (type == 23) // altgr
      {
        ReleaseKey(230, 0);
      }
      else if (type > 23 && type < 30) // fn
      {
        TempLayer = KeyLayer;
      }
      else if (type == 35) // win
      {
        ReleaseKey(227, 0);
      }


      if (PressedKey == false)
      {
        AnimusKeyboard.write(val);
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
    Serial.print("dualRoles");
  }

}

};
DualRoles_ DualRoles;
