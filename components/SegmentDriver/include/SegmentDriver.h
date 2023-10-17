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
//      
//          8
//       -------
//    3 |       | 7
//      |   2   |
//       -------
//    4 |       | 6
//      |       |    1
//       -------   .
//          5
//      
#define DIGIT_0 0b00111111
#define DIGIT_1 0b00000110
#define DIGIT_2 0b01011011
#define DIGIT_3 0b01001111
#define DIGIT_4 0b01100110
#define DIGIT_5 0b01101101
#define DIGIT_6 0b01111101
#define DIGIT_7 0b00000111
#define DIGIT_8 0b01111111
#define DIGIT_9 0b01101111
//                      12345678
#define DIGIT_UNKNOWN 0b01000000
class SegmentDriver
{
private:
    DisplayTime time = { 0, 0 };
    esp_timer_handle_t cycleDigitsTimerHandle;
    esp_timer_handle_t blinkTimerHandle;
    esp_timer_handle_t blinkDigitTimerHandle;

    int digitCount = 0;
    int testDigit = -1;
    int digitToBlink = 0;
    bool showPoints = false;
    bool showBlinkDigit = false;
    bool showDash = false;
    bool panic = false;
public:
private:
    void InitTimer();
    void InitGpio();
    void FillRegister();
    void FlashSegments();
    void ClearSegments();
    int GetCurrentDigitFromTime();
    int GetDigitBitmask(int digit);

public:
    SegmentDriver(/* args */) {};
    ~SegmentDriver() {};
    void Init();
    void UpdateTime(DisplayTime* time);
    void NextDigit();
    void Blink();
    void ToggleBlink();
    void ToggleDash();
    bool IsInPanicMode();
    void ReadyForSleep();
    void BlinkOnlyDigit(int digit);
    void BlinkDigit();
    void StopBlinkOnlyDigit();  
};

#endif