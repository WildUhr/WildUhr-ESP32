#include <stdio.h>
#include "SleepControl.h"

bool SleepControl::IsInPanicMode()
{
    return panic;
}

void SleepControl::EnterInactiveMode()
{
    CHECK_ERROR(esp_deep_sleep_enable_gpio_wakeup(BIT(WAKE_UP_GPIO), ESP_GPIO_WAKEUP_GPIO_LOW));

    LOG_INFO("Entering sleep mode", nullptr);
    esp_sleep_enable_gpio_wakeup();
    CHECK_ERROR(gpio_deep_sleep_wakeup_enable(WAKE_UP_GPIO, GPIO_INTR_LOW_LEVEL));
    LOG_INFO("Sleep GPIO Init", nullptr);
    esp_deep_sleep_start();
}

void SleepControl::Init()
{
    const gpio_config_t config = {
        .pin_bit_mask = BIT(WAKE_UP_GPIO),
        .mode = GPIO_MODE_INPUT,
    };
    CHECK_ERROR(gpio_config(&config));
}