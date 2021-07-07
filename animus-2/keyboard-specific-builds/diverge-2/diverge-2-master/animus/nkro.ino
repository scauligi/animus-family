
#define NKROMAX 3

void NKROStartup()
{
  // things to run during hardware startup
  AnimusKeyboard.setNKROMode(EEPROM.read(1022));
}

void NKROLoop()
{
  // full speed master loop


  // things inside this if statement is ran every RefreshDelay milliseconds
  if (CheckMillis())
  {

  }
}

void NKROKeyDown(char val, byte type)
{
  // ran when a key is pressed down


  // ran if this device's IS_MASTER flag is true
  if (IS_MASTER)
  {
    if (type == 6)
    {
      AnimusKeyboard.releaseAll();
      EEPROM.update(1022, val);
      AnimusKeyboard.setNKROMode(val);
      AnimusKeyboard.releaseAll();

    }
  }
}

void NKROKeyUp(char val, byte type)
{
  // ran when a key is pressed up


  // ran if this device's IS_MASTER flag is true
  if (IS_MASTER)
  {
  }
}

void NKROSetMode(byte val)
{
  if (val < NKROMAX)
  {
    EEPROM.update(1022, val);
    AnimusKeyboard.setNKROMode(val);
  }
}

void NKROSerial(String input)
{
  // ran when serial command of input is sent to device from host

  if (input.startsWith("uniqueksetnkro"))
  {
    input = input.substring(input.indexOf('(')+1);
    byte val = input.toInt();

    if (val < NKROMAX)
    {
      NKROSetMode(val);
      Serial.print("set NKRO mode(");
      Serial.println(val);
    }
    else
    {
      Serial.println("error: invalid NKRO option");
    }

  }
  else if (input == "uniquekgetnkro")
  {
    Serial.println(EEPROM.read(1022));
  }

}
#undef NKROMAX
