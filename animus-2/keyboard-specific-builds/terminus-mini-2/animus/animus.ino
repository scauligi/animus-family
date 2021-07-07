#define builder_row 5
#define builder_col 10
#define builder_kbname "Terminus Mini 2"
#define builder_kbvariant "Terminus Mini 2 typeZERO"
#define builder_kbdriver "Animus Innova v2.8"
#define builder_kbdriver_build "OEM"
#define builder_vpins 16, 14, 15, A0, A1
#define builder_hpins 10, 9, 8, 7, 6, 4, 3, 2, A2, A3
#define builder_refresh 5
#define builder_keyup_delay 5
#define I2C_HOST_ADDRESS 7
#define I2C_GUEST_ADDRESS 8
#include "EEPROM.h"
#include "KeyboardInterface.h"

// constants: change this only to edit keyboard info

const byte ROW = builder_row;
const byte COL = builder_col;
byte VPins[ROW] = {builder_vpins};
byte HPins[COL] = {builder_hpins};

byte RefreshDelay = builder_refresh;
byte KeyUpDelay = builder_keyup_delay;
