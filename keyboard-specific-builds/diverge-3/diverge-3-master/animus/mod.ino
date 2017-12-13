#define builder_mstartup BledPWMStartup(); I2CStartup(); mediaStartup(); shiftedStartup(); macroStartup(); dualRolesStartup(); tapDanceStartup(); mouseKeysStartup(); stickyKeysStartup();
#define builder_mloop BledPWMLoop(); I2CLoop(); mediaLoop(); shiftedLoop(); macroLoop(); dualRolesLoop(); tapDanceLoop(); mouseKeysLoop(); stickyKeysLoop();
#define builder_mkeydown BledPWMKeyDown(val, type); I2CKeyDown(val, type); mediaKeyDown(val, type); shiftedKeyDown(val, type); macroKeyDown(val, type); dualRolesKeyDown(val, type); tapDanceKeyDown(val, type); mouseKeysKeyDown(val, type); stickyKeysKeyDown(val, type);
#define builder_mkeyup BledPWMKeyUp(val, type); I2CKeyUp(val, type); mediaKeyUp(val, type); shiftedKeyUp(val, type); macroKeyUp(val, type); dualRolesKeyUp(val, type); tapDanceKeyUp(val, type); mouseKeysKeyUp(val, type); stickyKeysKeyUp(val, type);
#define builder_mserial BledPWMSerial(input); I2CSerial(input); mediaSerial(input); shiftedSerial(input); macroSerial(input); dualRolesSerial(input); tapDanceSerial(input); mouseKeysSerial(input); stickyKeysSerial(input);

void ModStartup()
{
  builder_mstartup
}

void ModLoop()
{
  if (CheckMillis())
  {
  }
  builder_mloop
}


void ModKeyDown(char val, byte type)
{
  builder_mkeydown
}

void ModPrePress(byte x, byte y)
{
}

void ModIntercedePress(char val, byte type)
{
  shiftedIntercedePress(val, type);
  tapDanceIntercedePress(val, type);
}

void ModKeyUp(char val, byte type)
{
  builder_mkeyup
}

void ModSerial(String input)
{
  if (input == "uniquekgetmods")
  {
    Serial.print("get mods(");
  }
  builder_mserial
  if (input == "uniquekgetmods")
  {
    Serial.print("\r\n");
  }
}
