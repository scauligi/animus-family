#ifndef ANIMUS_MOD_MODLED
#define ANIMUS_MOD_MODLED
#define LED_PIN 11
#include "ModTemplate.h"
#include "Animus.h"
#include <FastLED.h>

class CModLED : public CModTemplate
{
private:
  bool Enabled = true;
  CRGB LEDs[36]
public:
  CModLED(void);
  void Begin(void);
  void LoadData(void);
  void Loop(void);
  void PressCoords(byte x, byte y);
  void PrePress(byte val, byte type);
  void PressKey(byte val, byte type);
  void ReleaseKey(byte val, byte type);
  void SerialComms(byte mode);
};
extern CModLED ModLED;

#endif
