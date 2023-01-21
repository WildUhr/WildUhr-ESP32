#ifndef GYRO_H
#define GYRO_H
#include <Arduino.h>
#include <Wire.h>
#include <string>
#include "Helper.h"
#include "EspLogger.h"
#include "EspLoggerHelper.h"

#define GYRO_ADDRESS 0x6A

class Gyro
{
public:
    static const char *TAG;

private:
    int TestConnection();

public:
    void Setup();
    Gyro(/* args */)
    {
    }
    ~Gyro() {}
};

#endif