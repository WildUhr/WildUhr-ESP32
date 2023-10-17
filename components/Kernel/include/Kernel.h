#ifndef Kernel_H
#define Kernel_H

#include "TCPDebug.h"
#include "JSONHelper/PrimitiveJSON.h"
#include "SegmentDriver.h"
#include "ButtonControl.h"
#include "RealTimeClock.h"
#include "DisplayTime.h"
#include "SleepControl.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include <inttypes.h>
#include "nvs_flash.h"
#include "nvs.h"
#include "nvs_handle.hpp"
#include "esp_system.h"
class Kernel
{
public:
    enum State
    {
        UNDEFINED = 0,
        SHUTDOWN,
        BOOTING,
        READY,
        ARMED,
        RECORDING,
        WAITING,
        MENU,
        PANIC,
    };
private:
    State state = State::SHUTDOWN;
    SegmentDriver segmentDriver;
    ButtonControl buttonControl;
    RealTimeClock realTimeClock;
    SleepControl sleepControl; 
    esp_timer_handle_t inactiveTimerHandle;
    bool panic = false;

    enum SleepReason
    {
        PLANNED,
        INACTIVE,
    };
private:
    void ShutdownLogic();
    void BootingLogic();
    void ReadyLogic();
    void ArmedLogic();
    void RecordingLogic();
    void WaitingLogic();
    void MenuLogic();
    void PanicLogic();
    void SaveState(State saveState);
    State RecoverState();
    std::unique_ptr<nvs::NVSHandle> GetHandler();
    void UpdateTime();
    void SatisfyWatchdog();
    int PositionToSeconds(int possition);
    void InitInactiveTimer();
    void ResetInactiveTimer();
    void SaveWaitingTime(time_t saveTime);
    time_t ReadWaitingTime();
    
public:
    Kernel(/* args */){};
    ~Kernel(){};

    void entrypoint();
    void GoToSleep(SleepReason reason);
};

#endif