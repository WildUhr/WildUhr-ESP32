#include <stdio.h>
#include "SegmentDriver.h"
#include "TcpDebug.h"
#include "sdkconfig.h"

void entrypoint()
{
    LOG_DEBUG("goodDay", nullptr);
    while (LOG_IS_INITED == false)
    {
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
    
    SegmentDriver driver;
    driver.Init();
    DisplayTime time = {0, 0};
    driver.UpdateTime(&time);
    int counter = 0;
    while (true)
    {
        DisplayTime randomTime = {rand() % 24, rand() % 60};
        driver.UpdateTime(&randomTime);
        if(counter == 15)
        {
            driver.ToggleBlink();
            counter = 0;
        }
        counter++;
        vTaskDelay(1000 / portTICK_PERIOD_MS);

    }
}

extern "C" void app_main(void)
{
    entrypoint();
}
