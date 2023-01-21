#ifndef SEGMENTDRIVERCALLBACKREROUTE_H
#define SEGMENTDRIVERCALLBACKREROUTE_H
#include "SegmentDriver.h"

void segmentDriverCallBackReroute(void *arg)
{
    static_cast<SegmentDriver *>(arg)->OnTimer();
}

#endif