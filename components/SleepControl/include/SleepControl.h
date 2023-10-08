#ifndef SLEEP_CONTROL_H
#define SLEEP_CONTROL_H

#include <string>
#include "driver/gpio.h"
#include "TCPDebug.h"
#include "JSONHelper/PrimitiveJSON.h"
#include "esp_sleep.h"

#define WAKE_UP_GPIO             GPIO_NUM_0


class SleepControl
{
private:
    bool panic = false;
public:
    
public:
    SleepControl(/* args */){};
    ~SleepControl(){};

    void Init();
    void EnterInactiveMode();
    bool IsInPanicMode();
};

#endif