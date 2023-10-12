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
        SLEEP,
        PANIC,
    };
private:
    State state = State::SHUTDOWN;
    SegmentDriver segmentDriver;
    ButtonControl buttonControl;
    RealTimeClock realTimeClock;
    SleepControl sleepControl; 
    DisplayTime recordedTime = { 0, 0 };
    bool panic = false;
private:
    void ShutdownLogic();
    void BootingLogic();
    void ReadyLogic();
    void ArmedLogic();
    void RecordingLogic();
    void WaitingLogic();
    void MenuLogic();
    void SleepLogic();
    void PanicLogic();
    void SaveState(State saveState);
    State RecoverState();
    std::unique_ptr<nvs::NVSHandle> GetHandler();
public:
    Kernel(/* args */){};
    ~Kernel(){};

    void entrypoint();
    State getState();
};

#endif