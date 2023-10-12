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
    state = RecoverState();
    
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
    DisplayTime dTime = {
        .hour = 0,
        .minute = 0,
    };
    auto time = realTimeClock.GetTime();
    dTime.hour = (int)((time / 3600) % 24);
    dTime.minute = (int)((time / 60) % 60);
    segmentDriver.UpdateTime(&dTime);
    LOG_INFO("BOOT FINISHED", nullptr);
}

void Kernel::ReadyLogic()
{    
    ButtonMap bMap = buttonControl.GetButtonMap();
    ButtonControl::Button button = ButtonControl::Button::NONE;
    time_t time = realTimeClock.GetTime();
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
    LOG_INFO("MenuLogic", nullptr);
}

void Kernel::ArmedLogic()
{  
    LOG_INFO("Armed", nullptr);

    auto button = buttonControl.TryPop(12000);
    segmentDriver.ToggleDash();

    if(button == ButtonControl::Button::NONE)
    {
        state = State::RECORDING;
        LOG_INFO("RECORDING", nullptr);
        return;
    }

    LOG_INFO("ReadyLogic", nullptr);
    state = State::READY;    
}

void Kernel::RecordingLogic()
{
    SaveState(State::WAITING);
    segmentDriver.ReadyForSleep();
    sleepControl.EnterInactiveMode();
}

void Kernel::WaitingLogic()
{
    LOG_INFO("Waiting...", nullptr);
    ButtonControl::Button button = ButtonControl::Button::NONE;
    while (button == ButtonControl::Button::NONE){
        button = buttonControl.TryPop();
    }
    state = State::READY;
    LOG_INFO("Ready again", nullptr);
}

void Kernel::MenuLogic()
{
    ButtonMap bMap = buttonControl.GetButtonMap();
    time_t time = realTimeClock.GetTime();
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
    LOG_INFO("SleepLogic", nullptr);
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

void Kernel::SaveState(State saveState)
{
    auto handle = GetHandler();
    
    CHECK_ERROR(handle->set_item("firstRead", true));
    CHECK_ERROR(handle->set_item("state", saveState));
    CHECK_ERROR(handle->commit());
}

Kernel::State Kernel::RecoverState()
{
    auto handle = GetHandler();
    State nvsState = State::READY; // value will default to 0, if not set yet in NVS
    bool nvsFirstRead = false; // value will default to 0, if not set yet in NVS
    esp_err_t err = handle->get_item("firstRead", nvsFirstRead);
    CHECK_ERROR(err);
    if (err != ESP_ERR_NVS_NOT_FOUND && nvsFirstRead)
    {
        CHECK_ERROR(handle->get_item("state", nvsState));
        ESP_LOGI("NVS", "Recovered state: %d", nvsState);
        LOG_INFO("Recovered state", dynamic_cast<JsonObject*>(new PrimitiveJSON(&nvsState)));
    }
    CHECK_ERROR(handle->set_item("firstRead", false));
    CHECK_ERROR(handle->commit());

    return nvsState;    
}

std::unique_ptr<nvs::NVSHandle> Kernel::GetHandler()
{
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS partition was truncated and needs to be erased
        // Retry nvs_flash_init
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK( err );

    return nvs::open_nvs_handle("storage", NVS_READWRITE, &err);
}

Kernel::State Kernel::getState()
{
    return state;
}

