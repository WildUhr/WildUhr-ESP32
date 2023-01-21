#ifndef TILTSENSOR_H
#define TILTSENSOR_H
#include "Arduino.h"
#include "EspLogger.h"

#define TILT_PIN GPIO_NUM_32

class Tiltsensor
{
public:
    static const char *TAG;

private:
    /* data */
public:
    void Setup();
    void OnTilted();
    Tiltsensor(/* args */){};
    ~Tiltsensor(){};
};

#endif