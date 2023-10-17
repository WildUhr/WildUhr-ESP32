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

void RealTimeClock::AddOne(int position){
    struct tm timeinfo;
    time_t time = GetTime();
    localtime_r(&time, &timeinfo);
    int firstMinDigit = timeinfo.tm_min % 10;
    int secondMinDigit = timeinfo.tm_min / 10;
    int firstHourDigit = timeinfo.tm_hour % 10;
    int secondHourDigit = timeinfo.tm_hour / 10;
    int maxHourDigit = 0;

    switch (position)
    {
    case 3:
        firstMinDigit = (firstMinDigit + 1) % 10;
        break;
    case 2:
        secondMinDigit = (secondMinDigit + 1) % 6;
        break;
    case 1:
        maxHourDigit = 10;
        
        if (secondHourDigit > 1)
        {
            maxHourDigit = 4;
        }

        firstHourDigit = (firstHourDigit + 1) % maxHourDigit;
        break;
    case 0:
        maxHourDigit = 3;
        if (firstHourDigit > 3)
        {
            maxHourDigit = 2;
        }
        secondHourDigit =(secondHourDigit + 1) % maxHourDigit;
        break;
    default:
        break;
    }
    timeinfo.tm_min = firstMinDigit + secondMinDigit * 10;
    timeinfo.tm_hour = firstHourDigit + secondHourDigit * 10;
    SetTime(mktime(&timeinfo));
}

void RealTimeClock::SubtractOne(int position){
    struct tm timeinfo;
    time_t time = GetTime();
    localtime_r(&time, &timeinfo);
    int firstMinDigit = timeinfo.tm_min % 10;
    int secondMinDigit = timeinfo.tm_min / 10;
    int firstHourDigit = timeinfo.tm_hour % 10;
    int secondHourDigit = timeinfo.tm_hour / 10;
    int maxHourDigit = 0;
    switch (position)
    {
    case 3:
        firstMinDigit = (firstMinDigit + 9) % 10;
        break;
    case 2:
        secondMinDigit = (secondMinDigit + 5) % 6;
        break;
    case 1: 
        maxHourDigit = 10;
        
        if (secondHourDigit > 1)
        {
            maxHourDigit = 4;
        }

        firstHourDigit = (firstHourDigit + 9) % maxHourDigit;
        break;
    case 0:  
        if (firstHourDigit > 3)
        {
            secondHourDigit = (secondHourDigit + 1) % 2;
        }
        else{
            secondHourDigit = (secondHourDigit + 2) % 3;
        }
        break;
    default:
        break;
    }
    timeinfo.tm_min = firstMinDigit + secondMinDigit * 10;
    timeinfo.tm_hour = firstHourDigit + secondHourDigit * 10;
    SetTime(mktime(&timeinfo));
}