#include <stdio.h>
#include "SegmentDriver.h"
#include "TcpDebug.h"
#include "sdkconfig.h"

void entrypoint()
{
    LOG_DEBUG("goodDay", nullptr);

    SegmentDriver driver;
    driver.Init();
    DisplayTime time = {0, 0};
    driver.UpdateTime(&time);
    while (true)
    {
        DisplayTime randomTime = {rand() % 24, rand() % 60};
        driver.UpdateTime(&randomTime);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

extern "C" void app_main(void)
{
    entrypoint();
}
