#ifndef TILTSENSORCALLBACKREROUTE_H
#define TILTSENSORCALLBACKREROUTE_H
#include "Tiltsensor.h"

void tildsensorCallbackReroute(void *arg)
{
    static_cast<Tiltsensor *>(arg)->OnTilted();
}

#endif