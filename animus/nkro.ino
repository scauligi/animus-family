class NKRO_ : public ModBase {
  public:

static const int NKROMAX = 3;

void Startup()
{
  // things to run during hardware startup
  AnimusKeyboard.setNKROMode(EEPROM.read(1022));
}

void KeyDown(char val, byte type)
{
  // ran when a key is pressed down


  // ran if this device's IS_PRIME flag is true
  if (IS_PRIME)
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


void SetMode(byte val)
{
  if (val < NKROMAX)
  {
    EEPROM.update(1022, val);
    AnimusKeyboard.setNKROMode(val);
  }
}

void SerialCommand(String input)
{
  // ran when serial command of input is sent to device from host

  if (input.startsWith("uniqueksetnkro"))
  {
    input = input.substring(input.indexOf('(')+1);
    byte val = input.toInt();

    if (val < NKROMAX)
    {
      SetMode(val);
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

};
NKRO_ NKRO;
