#ifndef SLEEP_CONTROL_H
#define SLEEP_CONTROL_H

#include <string>
#include "driver/gpio.h"
#include "TCPDebug.h"
#include "JSONHelper/PrimitiveJSON.h"
#include "esp_sleep.h"

#define SHAKE_WAKE_UP_GPIO             GPIO_NUM_3
#define BUTTON1_WAKE_UP_GPIO           GPIO_NUM_0
#define BUTTON2_WAKE_UP_GPIO           GPIO_NUM_1
#define BUTTON3_WAKE_UP_GPIO           GPIO_NUM_2




class SleepControl
{
private:
    bool panic = false;
public:
    
public:
    SleepControl(/* args */){};
    ~SleepControl(){};

    void Init();
    void SleepWithShakeWeakup();
    void SleepWithButtonWeakup();
    bool IsInPanicMode();
};

#endif