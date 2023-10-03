#include "SegmentDriver.h"
#include "TCPDebug.h"
#include "JSONHelper/TimeJSON.h"
#include "JSONHelper/PrimitiveJSON.h"

void CycleDigitsCallback(void* arg)
{
    SegmentDriver* driver = (SegmentDriver*)arg;
    driver->NextDigit();
}

void BlinkCallback(void* arg)
{
    SegmentDriver* driver = (SegmentDriver*)arg;
    driver->Blink();
}

void SegmentDriver::UpdateTime(DisplayTime* time)
{
    if (time == NULL || time->hour < 0 || time->hour > 23 || time->minute < 0 || time->minute > 59)
    {
        LOG_ERROR("INVALIDE_TIME", dynamic_cast<JsonObject*>(new TimeJSON(*time)));
    }

    this->time = *time;
    LOG_INFO("Time updated:", dynamic_cast<JsonObject*>(new TimeJSON(*time)));
}

void SegmentDriver::Init()
{
    InitGpio();
    InitTimer();
}

void SegmentDriver::FillRegister()
{
    auto digit = GetDigitBitmask(GetCurrentDigitFromTime());
    for (size_t i = 0; i < 7; i++)
    {
        
        CHECK_ERROR(gpio_set_level(SER, ~(digit >> i) & 1));
        CHECK_ERROR(gpio_set_level(SRCLK, 1));
        CHECK_ERROR(gpio_set_level(SRCLK, 0));
    }
    if (digitCount % 2 != 0)
    {
        CHECK_ERROR(gpio_set_level(SER, showPoints));
    }   
    else
    {
        CHECK_ERROR(gpio_set_level(SER, 1));
    }
    
    CHECK_ERROR(gpio_set_level(SRCLK, 1));
    CHECK_ERROR(gpio_set_level(SRCLK, 0));
}

void SegmentDriver::FlashSegments()
{
    CHECK_ERROR(gpio_set_level(RCLK, 1));
    CHECK_ERROR(gpio_set_level(RCLK, 0));
}

void SegmentDriver::ClearSegments()
{
    for (size_t i = 0; i < 8; i++)
    {
        CHECK_ERROR(gpio_set_level(SER, 0));
        CHECK_ERROR(gpio_set_level(SRCLK, 1));
        CHECK_ERROR(gpio_set_level(SRCLK, 0));
    }
}

// The GetDigitBitmask function takes in a digit and returns the bitmask that corresponds to that digit.

int SegmentDriver::GetDigitBitmask(int digit)
{
    switch (digit)
    {
    case 0:
        return DIGIT_0;
    case 1:
        return DIGIT_1;
    case 2:
        return DIGIT_2;
    case 3:
        return DIGIT_3;
    case 4:
        return DIGIT_4;
    case 5:
        return DIGIT_5;
    case 6:
        return DIGIT_6;
    case 7:
        return DIGIT_7;
    case 8:
        return DIGIT_8;
    case 9:
        return DIGIT_9;
    default:
        return DIGIT_UNKNOWN;
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

    CHECK_ERROR(gpio_set_level(CA1, true));
    CHECK_ERROR(gpio_set_level(CA2, true));
    CHECK_ERROR(gpio_set_level(CA3, true));
    CHECK_ERROR(gpio_set_level(CA4, true));
    ClearSegments();
    FillRegister();
    FlashSegments();
    CHECK_ERROR(gpio_set_level(CA1, digitCount != 0));
    CHECK_ERROR(gpio_set_level(CA2, digitCount != 1));
    CHECK_ERROR(gpio_set_level(CA3, digitCount != 2));
    CHECK_ERROR(gpio_set_level(CA4, digitCount != 3));
    digitCount++;
}

void SegmentDriver::InitTimer()
{
    esp_timer_create_args_t cycleDigitsTimerArgs = {
        .callback = &CycleDigitsCallback,
        .arg = (void*)this,
        .dispatch_method = ESP_TIMER_TASK,
        .name = "CycleDigits",
        .skip_unhandled_events = true };
    CHECK_ERROR(esp_timer_create(&cycleDigitsTimerArgs, &cycleDigitsTimerHandle));

    CHECK_ERROR(esp_timer_start_periodic(cycleDigitsTimerHandle, 5));

    esp_timer_create_args_t blinkTimerArgs = {
        .callback = &BlinkCallback,
        .arg = (void*)this,
        .dispatch_method = ESP_TIMER_TASK,
        .name = "Blink",
        .skip_unhandled_events = true };
    CHECK_ERROR(esp_timer_create(&blinkTimerArgs, &blinkTimerHandle));

    LOG_INFO("Timers started", nullptr);
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
    CHECK_ERROR(gpio_config(&io_conf));

    CHECK_ERROR(gpio_set_level(RCLK, 0));
    LOG_INFO("GPIO initiolized", nullptr);
}

void SegmentDriver::Blink(){
    showPoints = !showPoints;
}

void SegmentDriver::ToggleBlink(){
    showPoints = false;
    bool isRunning = !esp_timer_is_active(blinkTimerHandle);
    LOG_INFO("Blinking started", dynamic_cast<JsonObject*>(new PrimitiveJSON(&isRunning)));
    if(isRunning != true){
        CHECK_ERROR(esp_timer_stop(blinkTimerHandle));
        return;
    } 
    CHECK_ERROR(esp_timer_start_periodic(blinkTimerHandle, 250000));

}

bool SegmentDriver::IsInPanicMode(){
    return panic;
}