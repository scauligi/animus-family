/*
Do not remove this comment, this comment is used by animus builder for the
build process
BUILDER_REQUIREMENT_START
BUILDER_REQUIREMENT_END
Remeber to change the mod_modname to your mod name
*/


#define mod_modname zcauligi

#define modMethod(str) conca(mod_modname, str)

byte modMethod(WinDown) = 0;
int16_t modMethod(MouseTimer)[4];
byte modMethod(MouseCount)[4];

void modMethod(Startup)()
{
  // things to run during hardware startup
  for (byte i = 0; i < 4; i++)
  {
    modMethod(MouseTimer)[i] = -1;
    modMethod(MouseCount)[i] = 0;
  }
}

void modMethod(Loop)()
{
  // full speed master loop


  // this is an async clock that runs once every ms, put non-critically responsive code in here to reduce processor stress
  if (CheckMillis())
  {
    for (byte i = 0; i < 4; i++)
    {
      if (modMethod(MouseTimer)[i] > 0)
      {
        modMethod(MouseTimer)[i]--;
      }
      else if (modMethod(MouseTimer)[i] == 0)
      {
        modMethod(MouseTimer)[i] = -1;
      }
    }
  }
}

void modMethod(KeyDown)(char val, byte type)
{
  // ran when a key is pressed down


  // ran if this device's IS_MASTER flag is true
  if (IS_MASTER)
  {
    // if you press a null key while on any layer
    if (type == 255 && val == 0)
    {
      if (TempLayer == 1 || TempLayer == 3)
      {
        PressKey(TempLayer == 1 ? 227 : 231, 0); // win
        modMethod(WinDown) = TempLayer;
        TempLayer = KeyLayer;
      }
    }

    // double tap mouse keys
    else if (type == 31 || type == 32)
    {
      int8_t m = modMethod(KeyToMouse)(val, type);
      byte i = m & B11;

      if (modMethod(MouseTimer)[i] < 0)
        modMethod(MouseCount)[i] = 0;
      modMethod(MouseCount)[i]++;
      modMethod(AdjustMouse)(i, modMethod(MouseCount)[i] - 1);
    }
  }
}

void modMethod(KeyUp)(char val, byte type)
{
  // ran when a key is pressed up


  // ran if this device's IS_MASTER flag is true
  if (IS_MASTER)
  {
    // when you release that layer key
    if (type == 1 && val == modMethod(WinDown))
    {
      ReleaseKey(modMethod(WinDown) == 1 ? 227 : 231, 0); // win
      modMethod(WinDown) = 0;
    }

    else if (type == 31 || type == 32)
    {
      int8_t m = modMethod(KeyToMouse)(val, type);
      byte i = m & B11;

      modMethod(AdjustMouse)(i, -(modMethod(MouseCount)[i] - 1));

      modMethod(MouseTimer)[i] = 5 * tapDanceTIMER_MULTIPLIER;
    }
  }
}

void modMethod(Serial)(String input)
{
  // ran when serial command of input is sent to device from host

  // do not delete this as this is for the identifier
  if (input.startsWith("uniquekgetmods"))
  {
    Serial.print(tokenToString(mod_modname));
  }

}

void modMethod(AdjustMouse)(byte i, int8_t amt)
{
  switch (i)
  {
    case 0: // up
      mouseKeysYVal -= amt;
      break;
    case 1: // down
      mouseKeysYVal += amt;
      break;
    case 2: // left
      mouseKeysXVal -= amt;
      break;
    case 3: // right
      mouseKeysXVal += amt;
      break;
  }
}

int8_t modMethod(KeyToMouse)(char val, byte type)
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


#undef mod_modname
#undef modMethod
