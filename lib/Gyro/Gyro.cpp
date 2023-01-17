#include "Gyro.h"
#define TAG "Gyro"

int Gyro::TestConnection()
{
    byte error;

    int nDevices;
    ESP_LOGE(TAG, "Scanning Gyro at address: %x", ADDRESS);

    nDevices = 0;

    Wire.beginTransmission(ADDRESS);
    error = Wire.endTransmission();
    return error;
}

void Gyro::Setup()
{
    Wire.begin();

    auto error = TestConnection();

    if (error != 0)
    {
        logI2CError(TAG, error);
        return;
    }
    ESP_LOGE(TAG, "I2C found");
}