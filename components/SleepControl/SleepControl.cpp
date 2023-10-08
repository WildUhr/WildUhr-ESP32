#include <stdio.h>
#include "SleepControl.h"

bool SleepControl::IsInPanicMode()
{
    return panic;
}

void SleepControl::EnterInactiveMode()
{
    LOG_INFO("Entering sleep mode", nullptr);

    CHECK_ERROR(gpio_deep_sleep_wakeup_enable(WAKE_UP_GPIO, GPIO_INTR_HIGH_LEVEL));
    LOG_INFO("Sleep GPIO Init", nullptr);
    esp_deep_sleep_start();
}

void SleepControl::Init()
{
    
}