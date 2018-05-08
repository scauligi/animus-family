

#include "Mod.h"

CMod::CMod(void)
{
  //nothing
}

void CMod::Begin(void)
{
  builder_mod_begin
}

void CMod::Loop(void)
{
  builder_mod_loop
  if (Comms.mode == 3) // print mod guid list
  {
    PrintMods();
    Comms.mode == 0; // need this to reset comms back to normal mode
  }

  if (Global.RequiresLoadData = true)
  {
    LoadData();
  }

}

void CMod::LoadData(void)
{
  builder_mod_loaddata
}

void CMod::PressCoords(byte x, byte y)
{
  builder_mod_prepress
}

void CMod::PrePress(byte val, byte type)
{
  builder_mod_prepress
}
void CMod::PressKey(byte val, byte type)
{
  builder_mod_press
}
void CMod::ReleaseKey(byte val, byte type)
{
  builder_mod_release
}

void CMod::PrintMods()
{
  builder_mod_print
}

void CMod::End(void)
{
  //nothing
}

CMod Mod;
