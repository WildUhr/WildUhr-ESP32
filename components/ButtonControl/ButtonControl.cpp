#include <stdio.h>
#include "ButtonControl.h"

void ButtonControl::Init()
{
    gpio_config_t io_conf = {};
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_conf.intr_type = GPIO_INTR_POSEDGE;
    io_conf.pin_bit_mask = GPIO_INPUT_PIN_SEL;
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
    gpio_config(&io_conf);

    gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);

    gpio_isr_handler_add(GPIO_UP, [](void* args) {
        LOG_INFO("Button up pressed", nullptr);
        //ButtonControl* buttonControl = (ButtonControl*)args;
        //if (buttonControl->handlers[ButtonControl::Button::UP] != nullptr)
        //{
        //    buttonControl->handlers[ButtonControl::Button::UP](buttonControl->handlerArgs[ButtonControl::Button::UP]);
        //}
    }, this);
    gpio_isr_handler_add(GPIO_DOWN, [](void* args) {
        LOG_INFO("Button down pressed", nullptr);
        //ButtonControl* buttonControl = (ButtonControl*)args;
        //if (buttonControl->handlers[ButtonControl::Button::DOWN] != nullptr)
        //{
        //    buttonControl->handlers[ButtonControl::Button::DOWN](buttonControl->handlerArgs[ButtonControl::Button::DOWN]);
        //}
    }, this);
    gpio_isr_handler_add(GPIO_ACTION, [](void* args) {
        LOG_INFO("Button action pressed", nullptr);
        //ButtonControl* buttonControl = (ButtonControl*)args;
        //if (buttonControl->handlers[ButtonControl::Button::ACTION] != nullptr)
        //{
        //    buttonControl->handlers[ButtonControl::Button::ACTION](buttonControl->handlerArgs[ButtonControl::Button::ACTION]);
        //}
    }, this);



    LOG_INFO("ButtonControl initialized", nullptr);
}

void ButtonControl::AddNewHandler(ButtonControl::Button button, void (*handler)(void*), void* args)
{
    int buttonIndex = (int)button;
    handlers[buttonIndex] = handler;
    handlerArgs[buttonIndex] = args;
    LOG_INFO("New handler added", dynamic_cast<JsonObject*>(new PrimitiveJSON(&buttonIndex)));
}

void ButtonControl::RemoveHandler(ButtonControl::Button button)
{
    int buttonIndex = (int)button;
    handlers[buttonIndex] = nullptr;
    handlerArgs[buttonIndex] = nullptr;
    LOG_INFO("Handler removed", dynamic_cast<JsonObject*>(new PrimitiveJSON(&buttonIndex)));
}

void ButtonControl::RemoveAllHandlers()
{
    for (int i = 0; i < 3; i++)
    {
        handlers[i] = nullptr;
        handlerArgs[i] = nullptr;
    }
    LOG_INFO("All handlers removed", nullptr);
}
