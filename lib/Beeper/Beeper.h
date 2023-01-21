#ifndef BEEPER_H
#define BEEPER_H
#include <Arduino.h>

class Beeper
{
private:
    const int ledPin = 15; // 16 corresponds to GPIO16
public:
    static const char *TAG;

private:
    void Tone(byte pin, int freq);
    void NoTone(byte pin);

public:
    Beeper(/* args */) {}
    ~Beeper() {}
    void PlaySong();
};

#endif