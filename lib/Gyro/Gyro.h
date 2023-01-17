#ifndef GYRO_H
#define GYRO_H
#include <Arduino.h>
#include <Wire.h>
#include <string>
#include "Helper.h"

#define ADDRESS 0x6A

class Gyro
{
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