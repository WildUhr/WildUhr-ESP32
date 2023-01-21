#ifndef ACCELEROMETER_H
#define ACCELEROMETER_H
#include <Arduino.h>
#include <Wire.h>
#include "esp_app_trace.h"
#include "Helper.h"
#include "EspLogger.h"
#include "EspLoggerHelper.h"

#define ACCELEROMETER_ADDRESS 0x68

class Accelerometer
{
public:
    static const char *TAG;

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