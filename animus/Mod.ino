#include <Array.h>

/* Order:
    x I2C
    media
    x macro
    x dualRoles
    x tapDance
    x mouseKeys
    x stickyKeys
    x shifted
    x zcauligi
*/

#define Mod(name) class name##_; extern name##_ name;
class ModBase;
Mod(NKRO)
Mod(I2C)
Mod(I2CSub)
Mod(Media)
Mod(Macro)
Mod(DualRoles)
Mod(TapDance)
Mod(MouseKeys)
Mod(StickyKeys)
Mod(Shifted)
Mod(ZCauligi)
#undef Mod

ModBase* mods[] = {
  (ModBase*)&NKRO,
  (ModBase*)&I2C,
  (ModBase*)&I2CSub,
  (ModBase*)&Media,
  (ModBase*)&Macro,
  (ModBase*)&DualRoles,
  (ModBase*)&TapDance,
  (ModBase*)&MouseKeys,
  (ModBase*)&StickyKeys,
  (ModBase*)&Shifted,
  (ModBase*)&ZCauligi,
};

class ModBase
{
public:
  virtual void Startup() {}
  virtual void OuterLoop() {}
  virtual void Loop() {}
  virtual void PressCoord(byte x, byte y) {}
  virtual void PrePress(char val, byte type) {}
  virtual void KeyDown(char val, byte type) {}
  virtual void KeyUp(char val, byte type) {}
  virtual void SerialCommand(String input) {}
};

#define withAllMods(meth) \
  for (auto mod : mods) { \
    mod->meth; \
  }
class Mod_ {
public:
  void Startup() { debug_log("startup"); withAllMods(Startup()); }
  void OuterLoop() { withAllMods(OuterLoop()); }
  void Loop() { withAllMods(Loop()); }
  void PressCoord(byte x, byte y) { withAllMods(PressCoord(x, y)); }
  void PrePress(char val, byte type) { withAllMods(PrePress(val, type)); }
  void KeyDown(char val, byte type) { withAllMods(KeyDown(val, type)); }
  void KeyUp(char val, byte type) { withAllMods(KeyUp(val, type)); }
  void SerialCommand(String input) { withAllMods(SerialCommand(input)); }
};
Mod_ Mod;
