#include "ModLED.h"
//TODO this is a dummy mod, remove this once the build system is up
CModLED::CModLED(void)
{
  // nothing
}

void CModLED::Begin(void)
{
  CModTemplate::Begin();
  ModGUID = 10; // GUID of this specific mod

  if (Global.HasUSB)
  {

  }
  FastLED.addLeds<NEOPIXEL, LED_PIN>(LEDs, 36);
}

void CModLED::LoadData(void)
{
  CModTemplate::LoadData();

  if (Global.HasUSB)
  {

  }
}

void CModLED::Loop(void)
{
  CModTemplate::Loop();
  for (int i = 0; i < 36; i++)
  {
    LEDs[i] = CRGB (100, 100, 100);
  }
}

void CModLED::PressCoords(byte x, byte y)
{
  CModTemplate::PressCoords(x, y);

}

void CModLED::PrePress(byte val, byte type)
{
  CModTemplate::PrePress(val, type);
  if (Global.HasUSB)
  {

  }
}
void CModLED::PressKey(byte val, byte type)
{
  CModTemplate::PressKey(val, type);

}
void CModLED::ReleaseKey(byte val, byte type)
{
  CModTemplate::ReleaseKey(val, type);

}

void CModLED::SerialComms(byte mode)
{
  CModTemplate::SerialComms(mode);
}


CModLED ModLED;
