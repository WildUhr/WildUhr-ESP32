#include <Arduino.h>
#include "Beeper.h"
#include "Bluetooth.h"

Bluetooth *BluetoothDriver = nullptr;
Beeper beeper;
void setup()
{
  Serial.begin(9600);
  // BluetoothDriver = new Bluetooth();
  Serial.println("1- Download and install an BLE scanner app in your phone");
  Serial.println("2- Scan for BLE devices in the app");
  Serial.println("3- Connect to MyESP32");
  Serial.println("4- Go to CUSTOM CHARACTERISTIC in CUSTOM SERVICE and write something");
  Serial.println("5- See the magic =)");
}

void loop()
{
  // put your main code here, to run repeatedly:
  beeper.PlaySong();
}