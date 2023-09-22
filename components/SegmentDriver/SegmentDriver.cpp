#include "SegmentDriver.h"
#include "TCPDebug.h"

void CycleDigitsCallback(void* arg)
{
    SegmentDriver* driver = (SegmentDriver*)arg;
    driver->NextDigit();
}

void SegmentDriver::UpdateTime(DisplayTime* time)
{
    if (time == NULL || time->hour < 0 || time->hour > 23 || time->minute < 0 || time->minute > 59)
        throw "Invalid time";
    this->time = *time;
}

void SegmentDriver::Init()
{
    InitGpio();
    InitTimer();
}

void SegmentDriver::FillRegister()
{
    for (size_t i = 0; i < 8; i++)
    {
        gpio_set_level(SER, (GetCurrentDigitFromTime() >> i) & 1);
        gpio_set_level(SRCLK, 1);
        gpio_set_level(SRCLK, 0);
    }
}

void SegmentDriver::FlashSegments()
{
    gpio_set_level(RCLK, 1);
    gpio_set_level(RCLK, 0);
}

void SegmentDriver::ClearSegments()
{
    for (size_t i = 0; i < 8; i++)
    {
        gpio_set_level(SER, 0);
        gpio_set_level(SRCLK, 1);
        gpio_set_level(SRCLK, 0);
    }
}

// The GetDigitBitmask function takes in a digit and returns the bitmask that corresponds to that digit.

int SegmentDriver::GetDigitBitmask(int digit)
{
    switch (digit)
    {
    case 0:
        return 0b11111100;
    case 1:
        return 0b01100000;
    case 2:
        return 0b11011010;
    case 3:
        return 0b11110010;
    case 4:
        return 0b01100110;
    case 5:
        return 0b10110110;
    case 6:
        return 0b10111110;
    case 7:
        return 0b11100000;
    case 8:
        return 0b11111110;
    case 9:
        return 0b11110110;
    default:
        return 0;
    }
}

int SegmentDriver::GetCurrentDigitFromTime()
{
    switch (digitCount)
    {
    case 0:
        return time.hour / 10;
    case 1:
        return time.hour % 10;
    case 2:
        return time.minute / 10;
    case 3:
        return time.minute % 10;
    default:
        return 0;
    }
}

void SegmentDriver::NextDigit()
{
    if (digitCount >= 4)
    {
        digitCount = 0;
    }

    gpio_set_level(CA1, digitCount == 0);
    gpio_set_level(CA2, digitCount == 1);
    gpio_set_level(CA3, digitCount == 2);
    gpio_set_level(CA4, digitCount == 3);

    digitCount++;
}

void SegmentDriver::InitTimer()
{
    esp_timer_init();
    esp_timer_create_args_t cycleDigitsTimerArgs = {
        .callback = &CycleDigitsCallback,
        .arg = (void*)this,
        .dispatch_method = ESP_TIMER_TASK,
        .name = "CycleDigits",
        .skip_unhandled_events = true };
    esp_timer_create(&cycleDigitsTimerArgs, &cycleDigitsTimerHandle);

    esp_timer_start_periodic(cycleDigitsTimerHandle, 10);
}

void SegmentDriver::InitGpio()
{
    gpio_config_t io_conf;
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask =
        (1ULL << RCLK) |
        (1ULL << SRCLK) |
        (1ULL << SER) |
        (1ULL << CA1) |
        (1ULL << CA2) |
        (1ULL << CA3) |
        (1ULL << CA4);
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
    gpio_config(&io_conf);

    gpio_set_level(RCLK, 0);
}
