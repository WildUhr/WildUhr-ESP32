#include "Accelerometer.h"
const char *Accelerometer::TAG = "Accelerometer";

int Accelerometer::TestConnection()
{
    byte error;

    int nDevices;
    ESP32_LOG_I(TAG, string_format("Scanning Accelerometer at address: %x", ACCELEROMETER_ADDRESS));
    nDevices = 0;

    Wire.beginTransmission(ACCELEROMETER_ADDRESS);
    error = Wire.endTransmission();
    return error;
}

void Accelerometer::Setup()
{
    Wire.begin();

    auto error = TestConnection();

    if (error != 0)
    {
        ESP32_LOG_I2C(TAG, error);
        return;
    }
    ESP32_LOG_I(TAG, "Accelerometer found");
}