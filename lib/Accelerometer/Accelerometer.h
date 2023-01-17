#ifndef ACCELEROMETER_H
#define ACCELEROMETER_H
#include <Arduino.h>
#include <Wire.h>
#include "esp_app_trace.h"
#include "Helper.h"

#define ADDRESS 0x68

class Accelerometer
{
private:
    int TestConnection();

public:
    void Setup();
    Accelerometer(/* args */)
    {
    }
    ~Accelerometer() {}
};

#endif