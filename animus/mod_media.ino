/*
Do not remove this comment, this comment is used by animus builder for the
build process
BUILDER_REQUIREMENT_START
BUILDER_REQUIREMENT_END
*/


class Media_ : public ModBase
{
public:
void KeyDown(char val, byte type)
{
  if (IS_PRIME)
  {
    if (type == 4)
    {
      if (val == 0)
      {
        Consumer.write(MEDIA_VOL_UP);
      }
      else if (val == 1)
      {
        Consumer.write(MEDIA_VOL_DOWN);
      }
      else if (val == 2)
      {
        Consumer.write(MEDIA_VOL_MUTE);
      }
      else if (val == 3)
      {
        Consumer.write(MEDIA_PLAY_PAUSE);
      }
      else if (val == 4)
      {
        Consumer.write(MEDIA_PAUSE);
      }
      else if (val == 5)
      {
        Consumer.write(MEDIA_NEXT);
      }
      else if (val == 6)
      {
        Consumer.write(MEDIA_PREV);
      }
    }
  }
}

void SerialCommand(String input)
{
  if (input.startsWith("uniquekgetmods"))
  {
    Serial.print("media");
  }

}

};
Media_ Media;
