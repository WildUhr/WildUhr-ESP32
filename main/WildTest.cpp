#include <stdio.h>
#include "SegmentDriver.h"
#include "ButtonControl.h"
#include "TcpDebug.h"
#include "sdkconfig.h"

void UpButtonHandler(void* arg)
{
    SegmentDriver* driver = (SegmentDriver*)arg;
    DisplayTime randomTime = {rand() % 24, rand() % 60};
    driver->UpdateTime(&randomTime);

}

void DownButtonHandler(void* arg)
{
    SegmentDriver* driver = (SegmentDriver*)arg;
    DisplayTime randomTime = {rand() % 24, rand() % 60};
    driver->UpdateTime(&randomTime);

}

void ActionButtonHandler(void* arg)
{
    SegmentDriver* driver = (SegmentDriver*)arg;
    driver->ToggleBlink();
}

void LoadDump()
{
    // https://github.com/espressif/esp-idf/blob/master/components/espcoredump/include/esp_core_dump.h}
}
void entrypoint()
{
    LOG_DEBUG("goodDay", nullptr);
    while (LOG_IS_INITED == false)
    {
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }

    //LoadDump();
    
    SegmentDriver driver;
    ButtonControl buttonControl;
    driver.Init();
    buttonControl.Init();

    buttonControl.AddNewHandler(ButtonControl::Button::UP, UpButtonHandler, &driver);
    buttonControl.AddNewHandler(ButtonControl::Button::DOWN, DownButtonHandler, &driver);
    buttonControl.AddNewHandler(ButtonControl::Button::ACTION, ActionButtonHandler, &driver);
    while (true)
    {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

extern "C" void app_main(void)
{
    entrypoint();
}
