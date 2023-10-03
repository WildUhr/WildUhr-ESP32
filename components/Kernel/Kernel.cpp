#include <stdio.h>
#include "Kernel.h"

void Kernel::entrypoint()
{
    while (true)
    {
        switch (state)
        {
        case State::SHUTDOWN:
            ShutdownLogic();
            break;
        case State::BOOTING:
            BootingLogic();
            break;
        case State::READY:
            ReadyLogic();
            break;
        case State::ARMED:
            ArmedLogic();
            break;
        case State::RECORDING:
            RecordingLogic();
            break;
        case State::WAITING:
            WaitingLogic();
            break;
        case State::MENU:
            MenuLogic();
            break;  
        case State::SLEEP:
            SleepLogic();
            break;
        case State::PANIC:
            PanicLogic();
            break;
        default:
            PanicLogic();
            break;
        }
    }
}

void Kernel::ShutdownLogic()
{
    state = State::BOOTING;
}

void Kernel::BootingLogic()
{
    state = State::READY;
    LOG_INFO("STARTING BOOT", nullptr);
    while (!LOG_IS_INITED)
    {
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
    segmentDriver.Init();
    if(segmentDriver.IsInPanicMode()){
        state = State::PANIC;
    }

    buttonControl.Init();
    if(buttonControl.IsInPanicMode()){
        state = State::PANIC;
    }   
        
    LOG_INFO("BOOT FINISHED", nullptr);
}

void Kernel::ReadyLogic()
{
    LOG_TRACE("ReadyLogic", nullptr);
    
    ButtonMap bMap = buttonControl.GetButtonMap();
    while (!(bMap.up && bMap.down && bMap.action)){bMap = buttonControl.GetButtonMap();}
    
    for (size_t i = 0; i < 20; i++)
    {
        bMap = buttonControl.GetButtonMap();
        if(!(bMap.up && bMap.down && bMap.action))
        {
            state = State::ARMED;
            return;
        }
        vTaskDelay(100 / portTICK_PERIOD_MS); //propably wait with xQueueReceive ticks
    }
    state = State::MENU;    
}

void Kernel::ArmedLogic()
{
    LOG_TRACE("ArmedLogic", nullptr);
}

void Kernel::RecordingLogic()
{
    LOG_TRACE("RecordingLogic", nullptr);
}

void Kernel::WaitingLogic()
{
    LOG_TRACE("WaitingLogic", nullptr);
}

void Kernel::MenuLogic()
{
    LOG_TRACE("MenuLogic", nullptr);
    ButtonControl::Button button = buttonControl.TryPop();
    ButtonMap bMap = buttonControl.GetButtonMap();
    switch (button)
    {
    case ButtonControl::Button::UP:
        LOG_DEBUG("UP", dynamic_cast<JsonObject*>(new PrimitiveJSON(&bMap.up)));
        break;
    case ButtonControl::Button::DOWN:
        LOG_DEBUG("DOWN", dynamic_cast<JsonObject*>(new PrimitiveJSON(&bMap.down)));
        break;
    case ButtonControl::Button::ACTION:
        LOG_DEBUG("ACTION", dynamic_cast<JsonObject*>(new PrimitiveJSON(&bMap.action)));
        break;
    default:
        break;
    }
}

void Kernel::SleepLogic()
{
    LOG_TRACE("SleepLogic", nullptr);
}

void Kernel::PanicLogic()
{
    LOG_WARN("WildClock is in panic:", nullptr);

    if (segmentDriver.IsInPanicMode())
    {
        LOG_CRITICAL("SegmentDriver is in panic", nullptr);
    }
    if (buttonControl.IsInPanicMode())
    {
        LOG_CRITICAL("ButtonControl is in panic", nullptr);
    }
    
    LOG_CRITICAL("Application is in panic mode but all Components work, how???????????????????", nullptr);

    vTaskDelay(100 / portTICK_PERIOD_MS);
    
}

Kernel::State Kernel::getState()
{
    return state;
}

