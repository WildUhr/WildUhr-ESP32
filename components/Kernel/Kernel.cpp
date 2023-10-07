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
    DisplayTime dTime = {
        .hour = 0,
        .minute = 0,
    };
    time_t time = 0;
    LOG_INFO("STARTING BOOT", nullptr);
    while (!LOG_IS_INITED)
    {
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
    segmentDriver.Init();
    buttonControl.Init();
    realTimeClock.Init();
    sleepControl.Init();

    if (segmentDriver.IsInPanicMode() || buttonControl.IsInPanicMode() || realTimeClock.IsInPanicMode() || sleepControl.IsInPanicMode())
    {
        state = State::PANIC;
        return;
    }
    time = realTimeClock.GetTime();
    dTime.hour = (int)((time / 3600) % 24);
    dTime.minute = (int)((time / 60) % 60);
    segmentDriver.UpdateTime(&dTime);
    LOG_INFO("BOOT FINISHED", nullptr);
    LOG_TRACE("Ready", nullptr);

}

void Kernel::ReadyLogic()
{    
    ButtonMap bMap = buttonControl.GetButtonMap();
    ButtonControl::Button button = ButtonControl::Button::NONE;
    time_t time = 0;
    DisplayTime dTime = {
        .hour = 0,
        .minute = 0,
    };
    while (!(bMap.up && bMap.down && bMap.action)){
        button = buttonControl.TryPop();
        bMap = buttonControl.GetButtonMap();
        time = realTimeClock.GetTime();
        dTime.hour = (int)((time / 3600) % 24);
        dTime.minute = (int)((time / 60) % 60);
        segmentDriver.UpdateTime(&dTime);
    }
    
    for (size_t i = 0; i < 20; i++)
    {
        bMap = buttonControl.GetButtonMap();
        if(!(bMap.up && bMap.down && bMap.action))
        {
            state = State::ARMED;
            segmentDriver.ToggleDash();
            while (!(!bMap.up && !bMap.down && !bMap.action)){
                button = buttonControl.TryPop();
                bMap = buttonControl.GetButtonMap();
                time = realTimeClock.GetTime();
                dTime.hour = (int)((time / 3600) % 24);
                dTime.minute = (int)((time / 60) % 60);
                segmentDriver.UpdateTime(&dTime);
            }
            return;
        }
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
    state = State::MENU; 
    segmentDriver.ToggleBlink();   
    while (!(!bMap.up && !bMap.down && !bMap.action)){
        button = buttonControl.TryPop();
        bMap = buttonControl.GetButtonMap();
        time = realTimeClock.GetTime();
        dTime.hour = (int)((time / 3600) % 24);
        dTime.minute = (int)((time / 60) % 60);
        segmentDriver.UpdateTime(&dTime);
    }
    LOG_TRACE("MenuLogic", nullptr);
}

void Kernel::ArmedLogic()
{  
    LOG_TRACE("ARMED", nullptr);
    ButtonControl::Button button = ButtonControl::Button::NONE;
    LOG_TRACE("ARMED1", nullptr);
    button = buttonControl.TryPop();
    LOG_TRACE("ARMED2", nullptr);
    button = buttonControl.TryPop();
    LOG_TRACE("ARMED3", nullptr);
    button = buttonControl.TryPop();
    LOG_TRACE("ARMED4", nullptr);
    segmentDriver.ToggleDash();
    LOG_TRACE("ARMED5", nullptr);

    if(button == ButtonControl::Button::NONE)
    {
        state = State::RECORDING;
        segmentDriver.TurnOff();
        LOG_TRACE("RECORDING", nullptr);
        return;
    }
    LOG_TRACE("ARMED6", nullptr);
    
    LOG_TRACE("ReadyLogic", nullptr);
    state = State::READY;    
}

void Kernel::RecordingLogic()
{
    sleepControl.EnterInactiveMode();
    LOG_INFO("Woke up from sleep", nullptr);
    buttonControl.HardReset();
    auto time = realTimeClock.GetTime();
    recordedTime.hour = (int)((time / 3600) % 24);
    recordedTime.minute = (int)((time / 60) % 60);
    segmentDriver.UpdateTime(&recordedTime);
    state = State::WAITING;
}

void Kernel::WaitingLogic()
{
    ButtonMap bMap = buttonControl.GetButtonMap();
    ButtonControl::Button button = ButtonControl::Button::NONE;
    while (!(bMap.up && bMap.down && bMap.action)){
        button = buttonControl.TryPop();
        bMap = buttonControl.GetButtonMap();
    }
    state = State::READY;
}

void Kernel::MenuLogic()
{
    ButtonMap bMap = buttonControl.GetButtonMap();
    time_t time = 0;
    ButtonControl::Button button = ButtonControl::Button::NONE;
    DisplayTime dTime = {
        .hour = 0,
        .minute = 0,
    };
    while (!bMap.up && !bMap.down && !bMap.action){
        button = buttonControl.TryPop();
        bMap = buttonControl.GetButtonMap();
        time = realTimeClock.GetTime();
        dTime.hour = (int)((time / 3600) % 24);
        dTime.minute = (int)((time / 60) % 60);
        segmentDriver.UpdateTime(&dTime);
    }

    button = buttonControl.TryPop();
    time = realTimeClock.GetTime();
    switch (button)
    {
    case ButtonControl::Button::UP:
        LOG_DEBUG("UP", dynamic_cast<JsonObject*>(new PrimitiveJSON(&bMap.up)));
        time += 60;
        realTimeClock.SetTime(time);

        dTime.hour = (int)((time / 3600) % 24);
        dTime.minute = (int)((time / 60) % 60);

        segmentDriver.UpdateTime(&dTime);
        break;
    case ButtonControl::Button::DOWN:
        LOG_DEBUG("DOWN", dynamic_cast<JsonObject*>(new PrimitiveJSON(&bMap.down)));
        time -= 60;
        realTimeClock.SetTime(time);

        dTime.hour = (int)((time / 3600) % 24);
        dTime.minute = (int)((time / 60) % 60);

        segmentDriver.UpdateTime(&dTime);
        break;
    case ButtonControl::Button::ACTION:
        LOG_DEBUG("ACTION", dynamic_cast<JsonObject*>(new PrimitiveJSON(&bMap.action)));
        state = State::READY;
        segmentDriver.ToggleBlink();
        break;
    case ButtonControl::Button::NONE:
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

    if (realTimeClock.IsInPanicMode())
    {
        LOG_CRITICAL("RealTimeClock is in panic", nullptr);
    }

    if (sleepControl.IsInPanicMode())
    {
        LOG_CRITICAL("SleepControl is in panic", nullptr);
    }
    
    vTaskDelay(100 / portTICK_PERIOD_MS);
    
}

Kernel::State Kernel::getState()
{
    return state;
}

