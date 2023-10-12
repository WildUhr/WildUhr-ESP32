#include <stdio.h>
#include "ButtonControl.h"

static QueueHandle_t gpio_evt_queue = NULL;
static uint32_t* deboundeTimer = new uint32_t[3]{0,0,0};
static uint32_t* buttonState = new uint32_t[3]{0,0,0};


void CalibrateCallback(void* arg)
{
    ButtonControl* driver = (ButtonControl*)arg;
    driver->Calibrate();
}

static void IRAM_ATTR gpio_isr_handler(void* arg)
{
    uint32_t gpio_num = (uint32_t) arg;

    if(xTaskGetTickCount() - deboundeTimer[gpio_num] > 2){
        xQueueSendFromISR(gpio_evt_queue, &gpio_num, NULL);
        deboundeTimer[gpio_num] = 0;
        buttonState[gpio_num] = !buttonState[gpio_num];
    }
    deboundeTimer[gpio_num] = xTaskGetTickCount();
}

ButtonControl::Button ButtonControl::TryPop(uint32_t waitTime)
{
    uint32_t io_num;
    if(xQueueReceive(gpio_evt_queue, &io_num, ( TickType_t ) (waitTime == 0? 10 : waitTime / portTICK_PERIOD_MS))) {
        return (Button)io_num;
    }
    return Button::NONE;
}

void ButtonControl::Init()
{
    gpio_config_t io_conf = {};
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_conf.intr_type = GPIO_INTR_ANYEDGE;
    io_conf.pin_bit_mask = GPIO_INPUT_PIN_SEL;
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pull_up_en = GPIO_PULLUP_ENABLE;

    CHECK_ERROR(gpio_config(&io_conf));
    
    gpio_evt_queue = xQueueCreate(10, sizeof(uint32_t));

    CHECK_ERROR(gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT));
    CHECK_ERROR(gpio_isr_handler_add(GPIO_UP, gpio_isr_handler, (void*)Button::UP));
    CHECK_ERROR(gpio_isr_handler_add(GPIO_DOWN,gpio_isr_handler, (void*)Button::DOWN));
    CHECK_ERROR(gpio_isr_handler_add(GPIO_ACTION, gpio_isr_handler, (void*)Button::ACTION));
    InitTimer();
    LOG_INFO("ButtonControl initialized", nullptr);
}

bool ButtonControl::IsInPanicMode(){
    return panic;
}

void ButtonControl::Calibrate(){
    for (size_t i = 0; i < 3; i++)
    {
        buttonState[i] = !gpio_get_level((gpio_num_t)i);
    }
}

ButtonMap ButtonControl::GetButtonMap(){
    ButtonMap map;
    map.up = buttonState[0];
    map.down = buttonState[1];
    map.action = buttonState[2];
    return map;
}

void ButtonControl::InitTimer(){
    esp_timer_create_args_t calibrateTimerArgs = {
        .callback = &CalibrateCallback,
        .arg = (void*)this,
        .dispatch_method = ESP_TIMER_TASK,
        .name = "Calibrate",
        .skip_unhandled_events = true };
    CHECK_ERROR(esp_timer_create(&calibrateTimerArgs, &calibrateTimerHandle));

    CHECK_ERROR(esp_timer_start_periodic(calibrateTimerHandle, 5000000));
}