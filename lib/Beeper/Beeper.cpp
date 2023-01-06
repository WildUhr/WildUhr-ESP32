#include "Beeper.h"
void Beeper::Tone(byte pin, int freq)
{
  ledcSetup(0, 2000, 8);  // setup beeper
  ledcAttachPin(pin, 0);  // attach beeper
  ledcWriteTone(0, freq); // play tone
}

void Beeper::NoTone(byte pin)
{
  tone(pin, 0);
}

void Beeper::PlaySong()
{
  int song[] = {262, 294, 330, 349, 392, 392, 440, 440, 440, 440, 392, 440, 440, 440, 440, 392, 349, 349, 349, 349, 330, 330, 392, 392, 392, 392, 262};
  int tonDauer[] = {200, 200, 200, 200, 400, 400, 200, 200, 200, 200, 800, 200, 200, 200, 200, 800, 200, 200, 200, 200, 400, 400, 200, 200, 200, 200, 800};
  for (int i = 0; i < 27; i++)
  {
    tone(ledPin, song[i]);
    delay(tonDauer[i]); // 1 sec Pause
  }
  NoTone(ledPin);
}