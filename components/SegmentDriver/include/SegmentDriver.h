#ifndef SEGMENT_DRIVER_H
#define SEGMENT_DRIVER_H

#include <stdio.h>
#include "driver/gpio.h"
#include "DisplayTime.h"
#include "esp_timer.h"

#define RCLK GPIO_NUM_18
#define SRCLK GPIO_NUM_19
#define SER GPIO_NUM_9
#define CA1 GPIO_NUM_4
#define CA2 GPIO_NUM_5
#define CA3 GPIO_NUM_7
#define CA4 GPIO_NUM_8

class SegmentDriver
{
private:
    DisplayTime time = {0, 0};
    esp_timer_handle_t cycleDigitsTimerHandle;

    int digitCount = 0;

private:
    void InitTimer();
    void InitGpio();
    void FillRegister();
    void FlashSegments();
    void ClearSegments();
    int GetCurrentDigitFromTime();
    int GetDigitBitmask(int digit);

public:
    SegmentDriver(/* args */){};
    ~SegmentDriver(){};
    void Init();
    void UpdateTime(DisplayTime *time);
    void NextDigit();
};

#endif