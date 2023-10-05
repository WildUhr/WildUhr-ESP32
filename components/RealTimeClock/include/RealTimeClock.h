#ifndef REAL_TIME_CLOCK_H
#define REAL_TIME_CLOCK_H
#include <string>
#include "TCPDebug.h"
#include "JSONHelper/PrimitiveJSON.h"

#define GPIO_DOWN             GPIO_NUM_0
#define GPIO_UP               GPIO_NUM_1
#define GPIO_ACTION           GPIO_NUM_2
#define GPIO_INPUT_PIN_SEL    ((1ULL<<GPIO_DOWN) | (1ULL<<GPIO_UP)| (1ULL<<GPIO_ACTION))
#define ESP_INTR_FLAG_DEFAULT 0


class RealTimeClock
{
private:
    bool panic = false;
public:
public:
    RealTimeClock(/* args */){};
    ~RealTimeClock(){};

    void Init();
    bool IsInPanicMode();
    time_t GetTime();
    void SetTime(time_t time);
};

#endif