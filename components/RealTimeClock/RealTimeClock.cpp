#include <stdio.h>
#include "RealTimeClock.h"
#include "rom/rtc.h"

void RealTimeClock::Init()
{
    LOG_INFO("RealTimeClock initialized", nullptr);
    if (GetTime() <= 0)
    {
        SetTime(1672531200);
    }
}

bool RealTimeClock::IsInPanicMode(){
    return panic;
}

time_t RealTimeClock::GetTime(){
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec;
}

void RealTimeClock::SetTime(time_t time){
    struct timeval tv = {.tv_sec = time};
    settimeofday(&tv, NULL);
    LOG_INFO("RealTimeClock time set", dynamic_cast<JsonObject*>(new PrimitiveJSON(&tv)));
}