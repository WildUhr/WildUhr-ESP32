#ifndef Kernel_H
#define Kernel_H
#include "TCPDebug.h"
#include "JSONHelper/PrimitiveJSON.h"
#include "SegmentDriver.h"
#include "ButtonControl.h"

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