#ifndef CALLBACK_H
#define CALLBACK_H
#include <Arduino.h>
#include <BLEUtils.h>
#include "Beeper.h"
#include "Helper.h"
class BeeperServiceCallbacks : public BLECharacteristicCallbacks
{
private:
    Beeper BeeperDriver;

    void onWrite(BLECharacteristic *pCharacteristic, esp_ble_gatts_cb_param_t *param)
    {
        std::string value = pCharacteristic->getValue();
        auto splitMessage = (*(splitstr(value)));
        auto command = splitMessage[0];
        auto id = splitMessage[1];
        if (command.compare("tone"))
        {
            BeeperDriver.PlaySong();
        }
    }
};
#endif