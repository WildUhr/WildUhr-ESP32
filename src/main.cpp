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
SegmentDriver segmentDriver;
Tiltsensor tiltsensor;

void setupLogger()
{
    Serial.println();
    ESP32_SET_LOG_LEVEL("*", LOG_INFO)
    ESP32_SET_LOG_LEVEL(SegmentDriver::TAG, LOG_VERBOSE)
}

void setup()
{
    gpio_reset_pin(GPIO_NUM_10);
    gpio_reset_pin(GPIO_NUM_2);
    gpio_reset_pin(GPIO_NUM_1);
    gpio_reset_pin(GPIO_NUM_3);
    gpio_reset_pin(GPIO_NUM_0);
    gpio_reset_pin(GPIO_NUM_9);
    gpio_reset_pin(GPIO_NUM_7);
    gpio_reset_pin(GPIO_NUM_8);
    gpio_reset_pin(GPIO_NUM_19);
    gpio_reset_pin(GPIO_NUM_18);
    gpio_reset_pin(GPIO_NUM_6);
    gpio_reset_pin(GPIO_NUM_5);
    Serial.begin(9600);
    // BluetoothDriver = new Bluetooth();
    segmentDriver.Setup();
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
    segmentDriver.TestDigit();
    // segmentDriver.SetTime(std::to_string(rand() % 2400 + 1));
}