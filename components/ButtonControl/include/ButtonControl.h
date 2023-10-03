#ifndef BUTTON_CONTROL_H
#define BUTTON_CONTROL_H
#include <string>
#include "driver/gpio.h"
#include "TCPDebug.h"
#include "JSONHelper/PrimitiveJSON.h"

#define GPIO_DOWN             GPIO_NUM_0
#define GPIO_UP               GPIO_NUM_1
#define GPIO_ACTION           GPIO_NUM_2
#define GPIO_INPUT_PIN_SEL    ((1ULL<<GPIO_DOWN) | (1ULL<<GPIO_UP)| (1ULL<<GPIO_ACTION))
#define ESP_INTR_FLAG_DEFAULT 0

struct ButtonMap
{
    bool up;
    bool down;
    bool action;
};

class ButtonControl
{
private:
    bool panic = false;
public:
    enum Button
    {
        NONE = -1,
        UP = 0,
        DOWN,
        ACTION
    };
public:
    ButtonControl(/* args */){};
    ~ButtonControl(){};

    void Init();
    Button TryPop(uint32_t waitTime = 0);
    bool IsInPanicMode();
    ButtonMap GetButtonMap();
    void ClearQueue();
};

#endif