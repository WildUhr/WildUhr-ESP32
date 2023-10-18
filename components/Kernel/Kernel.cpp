#include <stdio.h>
#include "Kernel.h"

void Kernel::entrypoint()
{
    while (true)
    {
        buttonControl.ClearQueue();
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

    UpdateTime();

    InitInactiveTimer();

    LOG_INFO("BOOT FINISHED", nullptr);
}

void Kernel::ReadyLogic()
{    
    LOG_INFO("ReadyLogic", nullptr);
    auto button = ButtonControl::Button::NONE;
    while (!buttonControl.AllButtonsPressed()){
        UpdateTime();
        SatisfyWatchdog();
        button = buttonControl.TryPop();
        if (button != ButtonControl::Button::NONE)
        {
            ResetInactiveTimer();
        }
    }
    buttonControl.ClearQueue();
    button = buttonControl.TryPop(2000);

    ResetInactiveTimer();
    if (button == ButtonControl::Button::NONE)
    {
        state = State::MENU; 
        segmentDriver.ToggleBlink();   
    }
    else
    {
        state = State::ARMED;
        segmentDriver.ToggleDash();
    }

    while (!buttonControl.AllButtonsReleased()){
        UpdateTime();
        SatisfyWatchdog();
    }
}

void Kernel::ArmedLogic()
{  
    LOG_INFO("Armed", nullptr);
    state = State::RECORDING;

    auto button = buttonControl.TryPop(12000);
    segmentDriver.ToggleDash();

    if(button != ButtonControl::Button::NONE)
    {
        state = State::READY;
        ResetInactiveTimer();
    }
}

void Kernel::RecordingLogic()
{
    LOG_INFO("Recording Logic", nullptr);
    ResetInactiveTimer();
    SaveWaitingTime(0);

    GoToSleep(SleepReason::PLANNED);    
}

void Kernel::WaitingLogic()
{
    LOG_INFO("Waiting...", nullptr);
    time_t time = ReadWaitingTime();
    if (time == 0)
    {
        SaveWaitingTime(realTimeClock.GetTime());
        time = realTimeClock.GetTime();
    }
    DisplayTime dTime = {
        .hour = (int)((time / 3600) % 24),
        .minute = (int)((time / 60) % 60),
    };
    segmentDriver.UpdateTime(&dTime);
    while (!buttonControl.AllButtonsReleased())
    {
        buttonControl.TryPop(10);
    }
    buttonControl.ClearQueue();
    LOG_INFO("Waiting for button press", nullptr);
    
    ButtonControl::Button button = ButtonControl::Button::NONE;
    ResetInactiveTimer();
    while (button == ButtonControl::Button::NONE){
        button = buttonControl.TryPop();
    }
    SaveWaitingTime(0);
    state = State::READY;
}

void Kernel::MenuLogic()
{
    LOG_INFO("Menu Logic", nullptr);
    ButtonControl::Button button = ButtonControl::Button::NONE;
    int position = 3;
    segmentDriver.BlinkOnlyDigit(position);
    while (true)
    {    
        button = buttonControl.TryPop();
        if (!buttonControl.AllButtonsReleased())
        {
            continue;
        }
        
        switch (button)
        {
        case ButtonControl::Button::ACTION:
            LOG_DEBUG("ACTION", nullptr);
            position--;
            segmentDriver.BlinkOnlyDigit(position);
            ResetInactiveTimer();
            break;
        case ButtonControl::Button::UP:
            LOG_DEBUG("UP", nullptr);
            realTimeClock.AddOne(position);
            ResetInactiveTimer();
            break;
        case ButtonControl::Button::DOWN:
            LOG_DEBUG("DOWN", nullptr);
            realTimeClock.SubtractOne(position);
            ResetInactiveTimer();
            break;
        case ButtonControl::Button::NONE:
        default:
            break;
        }

        UpdateTime();
        if (position < 0)
        {
            segmentDriver.ToggleBlink();
            state = State::READY;
            return;
        }
    }
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

void Kernel::SaveWaitingTime(time_t saveTime)
{
    auto handle = GetHandler();
    
    CHECK_ERROR(handle->set_item("waitingTime", saveTime));
    CHECK_ERROR(handle->commit());
}

time_t Kernel::ReadWaitingTime()
{
    auto handle = GetHandler();
    time_t nvsTime = 0;
    CHECK_ERROR(handle->get_item("waitingTime", nvsTime));
    return nvsTime;
}

Kernel::State Kernel::RecoverState()
{
    auto handle = GetHandler();
    State nvsState = State::READY; 
    bool nvsFirstRead = false; 
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
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK( err );

    return nvs::open_nvs_handle("storage", NVS_READWRITE, &err);
}

void Kernel::UpdateTime()
{
    auto time = realTimeClock.GetTime();
    DisplayTime dTime = {
        .hour = (int)((time / 3600) % 24),
        .minute = (int)((time / 60) % 60),
    };
    segmentDriver.UpdateTime(&dTime);
}

void Kernel::GoToSleep(SleepReason reason)
{
    segmentDriver.ReadyForSleep();
    switch (reason)
    {
    case SleepReason::PLANNED:
        SaveState(State::WAITING);
        sleepControl.SleepWithShakeWeakup();
        break;
    case SleepReason::INACTIVE:
        SaveState(state);
        sleepControl.SleepWithButtonWeakup();
        break;    
    default:
        break;
    }
}

void Kernel::InitInactiveTimer(){
    esp_timer_create_args_t inactiveTimerArgs;
    inactiveTimerArgs.callback = [](void* arg){
        Kernel* kernel = (Kernel*)arg;
        kernel->GoToSleep(SleepReason::INACTIVE);
    };
    inactiveTimerArgs.arg = this;
    inactiveTimerArgs.dispatch_method = ESP_TIMER_TASK;
    inactiveTimerArgs.name = "InactiveTimer";
    esp_timer_create(&inactiveTimerArgs, &inactiveTimerHandle);
    esp_timer_start_periodic(inactiveTimerHandle, 60000000);
}

void Kernel::ResetInactiveTimer(){
    esp_timer_stop(inactiveTimerHandle);
    esp_timer_start_periodic(inactiveTimerHandle, 60000000);
}

void Kernel::SatisfyWatchdog()
{
    vTaskDelay(1);
}