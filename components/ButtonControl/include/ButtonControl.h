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

class ButtonControl
{
private:
    /* data */
public:
    enum Button
    {
        UP = 0,
        DOWN,
        ACTION
    };
private:
    void (*handlers[3])(void*);
    void* handlerArgs[3];
public:
    ButtonControl(/* args */){};
    ~ButtonControl(){};

    void Init();
    void AddNewHandler(ButtonControl::Button button, void (*handler)(void*), void* args);
    void RemoveHandler(ButtonControl::Button button);
    void RemoveAllHandlers();
};

#endif