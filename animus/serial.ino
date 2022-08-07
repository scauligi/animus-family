// serial data
String inputData = "";

// serial functions start

void TestSerial()
{
  if (Serial.available()>0)
  {
    char inputByte = (char)Serial.read();

    if (inputByte != ' ')
    {
      inputData += inputByte;
    }
    else
    {
      SerialCommand(inputData);
      inputData = "";
    }

    if (inputData.length() > 100)
    {
      inputData = "";
    }
  }
}

void SerialCommand(String input)
{
  if (input == "uniquekreqinfo")
  {
    Serial.println("DEPRECIATED");
  }
  if (input == "uniquekreqname")
  {
    Serial.println(builder_kbname);
  }
  if (input == "uniquekreqvariant")
  {
    Serial.println(builder_kbvariant);
  }
  if (input == "uniquekreqdriver")
  {
    Serial.println(builder_kbdriver);
  }
  if (input == "uniquekreqbuild")
  {
    Serial.println(builder_kbdriver_build);
  }
  else if (input.startsWith("uniqueksetkey"))
  {
    input = input.substring(input.indexOf('(')+1);
    byte x = input.substring(0, input.indexOf('(')).toInt();
    input = input.substring(input.indexOf('(')+1);
    byte y = input.substring(0, input.indexOf('(')).toInt();
    input = input.substring(input.indexOf('(')+1);
    byte z = input.substring(0, input.indexOf('(')).toInt();
    input = input.substring(input.indexOf('(')+1);
    byte val = input.substring(0, input.indexOf('(')).toInt();
    input = input.substring(input.indexOf('(')+1);
    byte type = input.toInt();
    SetEEPROM(x, y, z, val, type);
    Serial.print("set key(");
    Serial.print(x);
    Serial.print("(");
    Serial.print(y);
    Serial.print("(");
    Serial.print(z);
    Serial.print("(");
    Serial.print(val);
    Serial.print("(");
    Serial.println(type);
  }
  else if (input.startsWith("uniquekseteeprom"))
  {
    input = input.substring(input.indexOf('(')+1);
    int addr = input.substring(0, input.indexOf('(')).toInt();
    input = input.substring(input.indexOf('(')+1);
    byte val = input.toInt();
    EEPROM.update(addr, val);
    Serial.print("set EEPROM(");
    Serial.print(addr);
    Serial.print("(");
    Serial.println(val);
  }
  else if (input.startsWith("uniqueksetlay"))
  {
    input = input.substring(input.indexOf('(')+1);
    byte val = input.toInt();

    if (val <= (600  / (ROW * COL)))
    {
      EEPROM.update(1023, val);
      Serial.print("set layer count(");
      Serial.println(val);
    }
    else
    {
      Serial.println("error: insufficient EEPROM");
    }

  }
  else if (input == "uniquekgetlay")
  {
    Serial.println(EEPROM.read(1023));
  }
  else if (input.startsWith("uniquekgeteeprom"))
  {
    
    input = input.substring(input.indexOf('(')+1);
    byte val = input.toInt();
    Serial.println(EEPROM.read(val));    
  }
  else if (input.startsWith("uniquekgetkey"))
  {
    input = input.substring(input.indexOf('(')+1);
    byte x = input.substring(0, input.indexOf('(')).toInt();
    input = input.substring(input.indexOf('(')+1);
    byte y = input.substring(0, input.indexOf('(')).toInt();
    input = input.substring(input.indexOf('(')+1);
    byte z = input.toInt();
    byte outVal = GetValEEPROM(x, y, z);
    byte outType = GetTypeEEPROM(x, y, z);
    Serial.print("return key(");
    Serial.print(outVal);
    Serial.print("(");
    Serial.println(outType);
  }
  else if (input.startsWith("uniquekgetrow"))
  {
    Serial.println(ROW);
  }
  else if (input.startsWith("uniquekgetcol"))
  {
    Serial.println(COL);
  }
  else if (input.startsWith("uniqueksetprime"))
  {
    input = input.substring(input.indexOf('(')+1);
    byte out = input.toInt();
    if (out == 0)
    {
      IS_PRIME = false;
    }
    else
    {
      IS_PRIME = true;
    }
    Serial.print("set prime(");
    Serial.print(IS_PRIME);
  }
  else if (input.startsWith("uniquekdumpall"))
  {
    byte b;
    for (int i = 0; i < 1024; i++) {
      b = EEPROM.read(i);
      Serial.write(b);
    }
    byte numLayers = b; // last byte of EEPROM
    for (byte z = 0; z < numLayers; z++) {
      I2C.SetTempLayer(z);
      for (byte y = 0; y < ROW; y++) {
        for (byte x = 0; x < COL; x++) {
          Wire.beginTransmission(8);
          Wire.write(7);    // request updated key press due to layer change
          Wire.write(x);
          Wire.write(y);
          Wire.endTransmission();
          Wire.requestFrom(8, 3);    // request 3 bytes from sub device #8
          Wire.read();    // discard count; we don't need it
          byte cinput = Wire.read();
          byte tinput = Wire.read();
          Serial.write(cinput);
          Serial.write(tinput);
        }
      }
    }
    I2C.SetTempLayer(TempLayer);
  }
  Mod.SerialCommand(input);
}

// serial functions end
