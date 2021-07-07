#include "ModLED.h"
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
  InitiateLED();
  LEDPreviousBrightness == Global.LEDBrightness;
}

void CModLED::InitiateLED(void) // this should only be called once per boot up
{
  uint8_t ledPin = GetData(LED_PIN_MEM);
  if (ledPin == 255) // this be janky due to hardware problems
  {
    LEDCount = GetData(LED_COUNT_MEM);
    if (LEDCount < 1 || LEDCount == 255)
    {
      LEDCount = 36;
    }
    FastLED.addLeds<NEOPIXEL, 26>(LEDs, LEDCount);
    RefreshLED();
  }
  else
  {
    LEDCount = GetData(LED_COUNT_MEM);
    if (LEDCount < 1 || LEDCount == 255)
    {
      LEDCount = 36;
    }
    FastLED.addLeds<NEOPIXEL, 11>(LEDs, LEDCount);
    RefreshLED();
  }
}

void CModLED::RefreshLED(void)
{
  for (short i = 0; i < LEDCount; i++)
  {
    LEDs[i] = CHSV (GetData(2 + i * 2), GetData(2 + i * 2 + 1), Global.LEDBrightness);
  }
  FastLED.show();
}

void CModLED::LoadData(void)
{
  CModTemplate::LoadData();

  if (Global.HasUSB)
  {

  }
  RefreshLED();
}

void CModLED::Loop(void)
{
  CModTemplate::Loop();
  if (Animus.Async1MSDelay())
  {
    if (RefreshCounter > 0)
    {
      RefreshCounter--;
    }
    else
    {
      RefreshCounter = 500;
      RefreshLED();
    }


    if (Global.LEDBrightness != LEDPreviousBrightness)
    {
      LEDPreviousBrightness = Global.LEDBrightness;
      RefreshLED();
    }
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
  if (Global.HasUSB)
  {

    if (type == 38) // LED brightness adjust
    {
      short adjust = val - 127;

      if (Global.LEDBrightness + adjust < 0)
      {
        Global.LEDBrightness = 0;
      }
      else if (Global.LEDBrightness + adjust > LED_MAX_BRIGHTNESS)
      {
        Global.LEDBrightness = LED_MAX_BRIGHTNESS;
      }
      else
      {
        Global.LEDBrightness = Global.LEDBrightness + adjust;
      }
    }
  }

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
