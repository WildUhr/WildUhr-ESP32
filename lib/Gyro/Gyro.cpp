#include "Gyro.h"
const char *Gyro::TAG = "Gyro";

int Gyro::TestConnection()
{
    byte error;

    int nDevices;
    ESP32_LOG_I(TAG, string_format("Scanning Gyro at address: %x", GYRO_ADDRESS));

    nDevices = 0;

    Wire.beginTransmission(GYRO_ADDRESS);
    error = Wire.endTransmission();
    return error;
}

void Gyro::Setup()
{
    Wire.begin();

    auto error = TestConnection();

    if (error != 0)
    {
        ESP32_LOG_I2C(TAG, error);
        return;
    }
    ESP32_LOG_I(TAG, "I2C found");
}