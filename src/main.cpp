#include <Arduino.h>
#include "Beeper.h"
#include "Bluetooth.h"
#include "Gyro.h"
#include "Accelerometer.h"
#include "SegmentDriver.h"
#include "Tiltsensor.h"

#include "EspLogger.h"

Bluetooth *BluetoothDriver = nullptr;
Beeper beeper;
Gyro gyro;
Accelerometer accelerometer;
// SegmentDriver segmentDriver;
Tiltsensor tiltsensor;

void setupLogger()
{
    Serial.println();
    ESP32_SET_LOG_LEVEL("*", LOG_INFO)
    ESP32_SET_LOG_LEVEL(Tiltsensor::TAG, LOG_VERBOSE)
}

void setup()
{
    Serial.begin(9600);
    // BluetoothDriver = new Bluetooth();
    // segmentDriver.Setup();
    // segmentDriver.SetTime("1234");

    setupLogger();

    // gyro.Setup();
    // accelerometer.Setup();
    tiltsensor.Setup();
}

void loop()
{
    // put your main code here, to run repeatedly:
    // beeper.PlaySong();
    // segmentDriver.TestMultiplex();
    // segmentDriver.SetTime(std::to_string(rand() % 2400 + 1));
}