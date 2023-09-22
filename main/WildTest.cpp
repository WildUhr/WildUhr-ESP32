#include <stdio.h>
#include "SegmentDriver.h"
#include "TcpDebug.h"
#include "sdkconfig.h"

void entrypoint()
{
    //SegmentDriver driver;
    //driver.Init();
    //DisplayTime time = {0, 0};
    //driver.UpdateTime(&time);
    //while (true)
    //{
    //    DisplayTime randomTime = {rand() % 24, rand() % 60};
    //    LOG_DEBUG("Updating time", nullptr);
    //    driver.UpdateTime(&randomTime);
    //    vTaskDelay(1000 / portTICK_PERIOD_MS);
    //}
    while (true)
    {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        LOG_ERROR("Error", nullptr);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        LOG_WARN("Warn", nullptr);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        LOG_INFO("Info", nullptr);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        LOG_DEBUG("Debug", nullptr);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        LOG_TRACE("Trace", nullptr);
    }
}

extern "C" void app_main(void)
{
    entrypoint();
}
