#ifndef BUTTON_CONTROL_H
#define BUTTON_CONTROL_H
#include <string>
#include "driver/gpio.h"
#include "TCPDebug.h"
#include "JSONHelper/PrimitiveJSON.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_timer.h"

#define GPIO_DOWN             GPIO_NUM_0
#define GPIO_UP               GPIO_NUM_1
#define GPIO_ACTION           GPIO_NUM_2
#define GPIO_INPUT_PIN_SEL    ((1ULL<<GPIO_DOWN) | (1ULL<<GPIO_UP)| (1ULL<<GPIO_ACTION))
#define ESP_INTR_FLAG_DEFAULT 0

class ButtonControl
{
private:
    bool panic = false;
    esp_timer_handle_t calibrateTimerHandle;
public:
    enum Button
    {
        NONE = -1,
        DOWN = 0,
        UP,
        ACTION
    };
public:
    ButtonControl(/* args */){};
    ~ButtonControl(){};

    void Init();
    void Calibrate();
    Button TryPop(uint32_t waitTime = 0);
    bool IsInPanicMode();
    void ClearQueue();
    bool AllButtonsReleased();
    bool AllButtonsPressed();
    bool IsPressed(Button button);
private:
    void InitTimer();

};

#endif