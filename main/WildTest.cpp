#include <stdio.h>
#include "Kernel.h"

void entrypoint()
{
    Kernel kernel;
    kernel.entrypoint();
}

extern "C" void app_main(void)
{
    entrypoint();
}
