#include "Accelerometer.h"
#define TAG "Accelerometer"

int Accelerometer::TestConnection()
{
    byte error;

    int nDevices;
    ESP_LOGE(TAG, "Scanning Gyro at address: %x", ADDRESS);
    nDevices = 0;

    Wire.beginTransmission(ADDRESS);
    error = Wire.endTransmission();
    return error;
}

void Accelerometer::Setup()
{
    Wire.begin();

    auto error = TestConnection();

    if (error != 0)
    {
        logI2CError(TAG, error);
        return;
    }
    ESP_LOGE(TAG, "Tiltsensor found");
}