#ifndef Kernel_H
#define Kernel_H
#include "TCPDebug.h"
#include "JSONHelper/PrimitiveJSON.h"
#include "SegmentDriver.h"
#include "ButtonControl.h"
#include "RealTimeClock.h"
#include "DisplayTime.h"
#include "SleepControl.h"
class Kernel
{
public:
    enum State
    {
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
public:
    Kernel(/* args */){};
    ~Kernel(){};

    void entrypoint();
    State getState();
};

#endif