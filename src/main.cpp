#include <Arduino.h>
#include "Beeper.h"
#include "Bluetooth.h"
#include "Gyro.h"
#include "Accelerometer.h"
#include "SegmentDriver.h"

Bluetooth *BluetoothDriver = nullptr;
Beeper beeper;
Gyro gyro;
Accelerometer accelerometer;
SegmentDriver segmentDriver;

void setup()
{
  Serial.begin(9600);
  // BluetoothDriver = new Bluetooth();
  // segmentDriver.Setup();
  // segmentDriver.SetTime("1234");
  gyro.Setup();
  accelerometer.Setup();
}

void loop()
{
  // put your main code here, to run repeatedly:
  beeper.PlaySong();
  // segmentDriver.TestMultiplex();
  // segmentDriver.SetTime(std::to_string(rand() % 2400 + 1));
}