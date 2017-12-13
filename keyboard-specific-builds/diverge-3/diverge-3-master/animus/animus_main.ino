#define _tokenToString(str) #str
#define tokenToString(str) _tokenToString(str)

#define _conca(str1, str2) str1##str2
#define conca(str1, str2) _conca(str1, str2)



#define Lay GetLayEEPROM()

// layering
byte KeyLayer = 0;
byte TempLayer = 0;

// key states
byte KeyState[COL][ROW];
byte KeyStateCountDown[COL][ROW];
byte LayerState[COL][ROW];

// slave states
boolean IS_MASTER = true;

// spacefn keys
boolean PressedKey = false;

// baud: dont change this it messes with the GUI interface
int BaudRate = 19200;


void setup()
{
  AnimusKeyboard.begin();
  Serial.begin(BaudRate);
  NKROStartup();
  ModStartup();
  ResetPins();

  // serial delay
  delay(300);
}

void loop()
{
  // resets millis counter
  MillisLoop();
  // checks serials
  TestSerial();


  // layering checks
  if (KeyLayer > GetLayEEPROM() || TempLayer > GetLayEEPROM())
  {
    KeyLayer = 0;
    TempLayer = 0;
  }

  // main loop starts
  KeyScan();

  for (byte i = 0; i < ROW; i++)
  {
    for (byte j = 0; j < COL; j++)
    {
      if (KeyState[j][i] == HIGH) // if key is down
      {
        if (KeyStateCountDown[j][i] == 1) // if key is still down after debounce time
        {
          LayerState[j][i] = TempLayer;
          ModIntercedePress(GetValEEPROM(j, i, TempLayer), GetTypeEEPROM(j, i, TempLayer));
          LayerState[j][i] = TempLayer;
          PressKey(GetValEEPROM(j, i, TempLayer), GetTypeEEPROM(j, i, TempLayer));
          KeyStateCountDown[j][i] = 255;

        }
        else if (KeyStateCountDown[j][i] == 0) // if key is still down after debounce time
        {
          KeyStateCountDown[j][i] = builder_refresh+1;
        }

      }
      else if (KeyState[j][i] == LOW)
      {
        if (KeyStateCountDown[j][i] == 255)
        {
          ReleaseKey(GetValEEPROM(j, i, LayerState[j][i]), GetTypeEEPROM(j, i, LayerState[j][i]));

          KeyStateCountDown[j][i] = 0;
        }

      }




    }
    // main loop ends
  }

  if (CheckMillis())
  {
    for (byte i = 0; i < ROW; i++)
    {
      for (byte j = 0; j < COL; j++)
      {
        if (KeyStateCountDown[j][i] > 0 && KeyStateCountDown[j][i] != 255)
        {
          KeyStateCountDown[j][i] = KeyStateCountDown[j][i] - 1;
        }
      }
    }
  }

  ModLoop();
  NKROLoop();
}


// key press functions start

void PressKey(char val, byte type)
{
  PressedKey = true;
  if (IS_MASTER)
  {
    if (type == 0)
    {
      AnimusKeyboard.press(val);
    }
    else if (type == 1)
    {
      TempLayer = val;
      //ReleaseAllKey();
    }
    else if (type == 2) // DEPRECIATED
    {
      PressedKey = false;
      TempLayer = val;
      //ReleaseAllKey();  // DEPRECIATED END
    }
    else if (type == 3)
    {
      if (val == 0)
      {
        SwitchLayer(true);
      }
      else if (val == 1)
      {
        SwitchLayer(false);
      }
      else
      {
        /* Rotate to the next layer in bitfield within val */
        RotateLayers(val);
      }
    }
    else if (type == 5)
    {
      if (TempLayer == val)
      {
        TempLayer = KeyLayer;
      }
      else
      {
        TempLayer = val;
      }
      //ReleaseAllKey();
    }
    /*else if (type == 11)
    {
      // shifted
      PressKey(225, 0);
      PressKey(val, 0);
    }*/
    else if (type == 12)
    {
      // ctrled
      PressKey(224, 0);
      PressKey(val, 0);
    }
    else if (type == 13)
    {
      // alted
      PressKey(226, 0);
      PressKey(val, 0);
    }
    else if (type == 14)
    {
      // altgred
      PressKey(230, 0);
      PressKey(val, 0);
    }
    else if (type == 15)
    {
      // ctrl shifted
      PressKey(224, 0);
      PressKey(225, 0);
      PressKey(val, 0);
    }
    else if (type == 16)
    {
      // ctrl shifted alted
      PressKey(224, 0);
      PressKey(225, 0);
      PressKey(226, 0);
      PressKey(val, 0);
    }
    else if (type == 17)
    {
      // ctrl alted
      PressKey(224, 0);
      PressKey(226, 0);
      PressKey(val, 0);
    }
    else if (type == 18)
    {
      // shifted alted
      PressKey(225, 0);
      PressKey(226, 0);
      PressKey(val, 0);
    }
  }

  NKROKeyDown(val, type);
  ModKeyDown(val, type);
}

void ReleaseKey(char val, byte type)
{
  if (IS_MASTER)
  {
    if (type == 0)
    {
      AnimusKeyboard.release(val);
    }
    else if (type == 1)
    {
      TempLayer = KeyLayer;
      //ReleaseAllKey();
    }
    else if (type == 2) //DEPRECIATED
    {
      if (PressedKey == false)
      {
        AnimusKeyboard.write(44);
      }
      TempLayer = KeyLayer;
      //ReleaseAllKey(); // DEPRECIATED END
    }
    /*else if (type == 11)
    {
      // shifted
      ReleaseKey(225, 0);
      ReleaseKey(val, 0);
    }*/
    else if (type == 12)
    {
      // ctrled
      ReleaseKey(224, 0);
      ReleaseKey(val, 0);
    }
    else if (type == 13)
    {
      // alted
      ReleaseKey(226, 0);
      ReleaseKey(val, 0);
    }
    else if (type == 14)
    {
      // altgred
      ReleaseKey(230, 0);
      ReleaseKey(val, 0);
    }
    else if (type == 15)
    {
      // ctrl shifted
      ReleaseKey(224, 0);
      ReleaseKey(225, 0);
      ReleaseKey(val, 0);
    }
    else if (type == 16)
    {
      // ctrl shifted alted
      ReleaseKey(224, 0);
      ReleaseKey(225, 0);
      ReleaseKey(226, 0);
      ReleaseKey(val, 0);
    }
    else if (type == 17)
    {
      // ctrl alted
      ReleaseKey(224, 0);
      ReleaseKey(226, 0);
      ReleaseKey(val, 0);
    }
    else if (type == 18)
    {
      // shifted alted
      ReleaseKey(225, 0);
      ReleaseKey(226, 0);
      ReleaseKey(val, 0);
    }
  }
  NKROKeyUp(val, type);
  ModKeyUp(val, type);
}


void ReleaseAllKey()
{


  TempLayer = KeyLayer;
  uint8_t tkeys[18] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  byte tkeyCounter = 0;

  for (byte i = 0; i < ROW; i++)
  {
    for (byte j = 0; j < COL; j++)
    {
      if (KeyState[j][i] == HIGH)
      {
        char val = GetValEEPROM(j, i, TempLayer);
        byte type = GetTypeEEPROM(j, i, TempLayer);
        if (type == 0)
        {

          tkeys[tkeyCounter] = val;
          tkeyCounter++;
        }
      }
    }
  }

  AnimusKeyboard.releaseAllExcept(tkeys);

}


void SwitchLayer(boolean increment)
{
  if (increment)
  {
    KeyLayer++;
  }
  else
  {
    KeyLayer--;
  }
  if (KeyLayer > 254)
  {
    KeyLayer = Lay - 1;
  }
  else if (KeyLayer >= Lay)
  {
    KeyLayer = 0;
  }
  //ReleaseAllKey();
}


/*
  Rotate to the next layer in bitfield within val.
  Examples:
    val=0x3: rotate between layers 0 and 1
    val=0x7: rotate between layers 0, 1, and 2
    val=0xF: rotate between layers 0, 1, 2, and 3
    val=0x11: rotate between layers 0 and 4
*/
void RotateLayers(byte val)
{
  byte newLayer = KeyLayer;
  byte mask = 1 << newLayer;

  if (! val)
  {
    /* No layers are allowed. Do nothing. */
    return;
  }

  /*
    Find the next layer within the bitfield.
    Limit iterations to 10, to ensure no infinite loop.
   */
  for (byte i = 0; i < 10; i++)
  {
    newLayer++;
    mask <<= 1;
    if ((! mask) || (newLayer >= Lay))
    {
      newLayer = 0;
      mask = 1;
    }

    if (val & mask)
    {
      /* newLayer is an allowed layer within the bitfield. */
      break;
    }
  }

  KeyLayer = newLayer;
  //ReleaseAllKey();
}



void ResetPins()
{
  for (byte i = 0; i < ROW; i++)
  {
    pinMode(VPins[i], INPUT);
    digitalWrite(VPins[i], HIGH);
  }

  for (byte i = 0; i < COL; i++)
  {
    pinMode(HPins[i], INPUT);
    digitalWrite(HPins[i], LOW);
  }

  for (byte i = 0; i < ROW; i++)
  {
    for (byte j = 0; j < COL; j++)
    {
      KeyState[j][i] = 0;
      KeyStateCountDown[j][i] = 0;
    }
  }
}





void KeyScan()
{
  for (byte i = 0; i < COL; i++)
  {
    pinMode(HPins[i], OUTPUT);

    for (byte j = 0; j < ROW; j++)
    {
      byte val = digitalRead(VPins[j]);
      KeyState[i][j] = (val == LOW);
    }

    pinMode(HPins[i], INPUT);
  }
}


// key press functions end
