#include "Tiltsensor.h"
#include "TiltsensorCallbackReroute.h"
const char *Tiltsensor::TAG = "Tiltsensor";

void Tiltsensor::OnTilted()
{
    ESP32_LOG_I(TAG, "Tilt detected");
}

void Tiltsensor::Setup()
{
    gpio_pad_select_gpio(TILT_PIN);
    gpio_set_direction(TILT_PIN, GPIO_MODE_INPUT);
    gpio_pulldown_en(TILT_PIN);
    gpio_pullup_dis(TILT_PIN);
    gpio_set_intr_type(TILT_PIN, GPIO_INTR_ANYEDGE);

    gpio_install_isr_service(0);
    gpio_isr_handler_add(TILT_PIN, tildsensorCallbackReroute, (void *)this);
    ESP32_LOG_I(TAG, "Tilt setup complete");
}