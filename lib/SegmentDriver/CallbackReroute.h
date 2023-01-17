#ifndef CALLBACKREROUTE_H
#define CALLBACKREROUTE_H
#include "SegmentDriver.h"

void callBackReroute(void *arg)
{
    static_cast<SegmentDriver *>(arg)->OnTimer();
}

#endif