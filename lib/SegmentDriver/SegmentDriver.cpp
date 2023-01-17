#include "SegmentDriver.h"
#include "CallbackReroute.h"
#define TAG "Segment Display"

void SegmentDriver::WriteDigit(char digit)
{
    auto gpios = digitMap.find(digit);
    if (gpios != digitMap.end())
        return;

    ClearSegment();

    for (auto &&gpio : digitMap.at(digit))
    {
        digitalWrite(gpio, HIGH);
    }
    ESP_LOGV(TAG, "Write Digit: %c", digit);
}

void SegmentDriver::ClearSegment()
{
    digitalWrite(SEGMENT_A, LOW);
    digitalWrite(SEGMENT_B, LOW);
    digitalWrite(SEGMENT_C, LOW);
    digitalWrite(SEGMENT_D, LOW);
    digitalWrite(SEGMENT_E, LOW);
    digitalWrite(SEGMENT_F, LOW);
    digitalWrite(SEGMENT_G, LOW);
    digitalWrite(SEGMENT_DP, LOW);
    ESP_LOGV(TAG, "Clear Segment", digit);
}

void SegmentDriver::StartTimer()
{
    esp_timer_create_args_t _timerConfig;
    _timerConfig.arg = reinterpret_cast<void *>(this);
    _timerConfig.callback = callBackReroute;
    _timerConfig.dispatch_method = ESP_TIMER_TASK;
    _timerConfig.name = "Ticker";
    if (_timer)
    {
        esp_timer_stop(_timer);
        esp_timer_delete(_timer);
    }
    esp_timer_create(&_timerConfig, &_timer);
    esp_timer_start_periodic(_timer, 1000 * 1000ULL);
    ESP_LOGI(TAG, "Timer started");
}

void SegmentDriver::SetTime(std::string newTimeString)
{
    if (newTimeString.length() != 4)
        return;
    if (std::all_of(newTimeString.begin(), newTimeString.end(), ::isdigit))
        time = newTimeString;
    ESP_LOGV(TAG, "timeset: %s", newTimeString);
}

void SegmentDriver::SwitchMultipex()
{
    digitalWrite(GND1, HIGH);
    digitalWrite(GND2, HIGH);
    digitalWrite(GND3, HIGH);
    digitalWrite(GND4, HIGH);
    ESP_LOGD(TAG, "Multiplex with: %s", segmentCounter);

    switch (segmentCounter)
    {
    case 0:
        digitalWrite(GND1, LOW);
        break;
    case 1:
        digitalWrite(GND2, LOW);
        break;
    case 2:
        digitalWrite(GND3, LOW);
        break;
    case 3:
        digitalWrite(GND4, LOW);
        break;

    default:
        ESP_LOGE(TAG, "Multiplexfailed");
        break;
    }
}

void SegmentDriver::OnTimer()
{
    auto digit = time[segmentCounter];
    WriteDigit(digit);
    SwitchMultipex();
    ESP_LOGD(TAG, "should show: %s", time);
    segmentCounter++;
    if (segmentCounter > 3)
        segmentCounter = 0;
}

void SegmentDriver::TestMultiplex()
{
    ESP_LOGD(TAG, "TestMultiplex started");
    while (true)
    {
        WriteDigit('8');
        digitalWrite(GND1, HIGH);
        digitalWrite(GND2, HIGH);
        digitalWrite(GND3, HIGH);
        digitalWrite(GND4, HIGH);
        ESP_LOGD(TAG, "TestMultiplex 1");
        digitalWrite(GND1, LOW);
        delay(1000);
        ESP_LOGD(TAG, "TestMultiplex 2");
        digitalWrite(GND2, LOW);
        delay(1000);
        ESP_LOGD(TAG, "TestMultiplex 3");
        digitalWrite(GND3, LOW);
        delay(1000);
        ESP_LOGD(TAG, "TestMultiplex 4");
        digitalWrite(GND4, LOW);
        delay(1000);
    }
}

void SegmentDriver::Setup()
{
    ESP_LOGD(TAG, "TrySet: GNDDD");
    pinMode(GNDDD, OUTPUT);
    ESP_LOGD(TAG, "TrySet: GND1");
    pinMode(GND1, OUTPUT);
    ESP_LOGD(TAG, "TrySet: GND2");
    pinMode(GND2, OUTPUT);
    ESP_LOGD(TAG, "TrySet: GND3");
    pinMode(GND3, OUTPUT);
    ESP_LOGD(TAG, "TrySet: GND4");
    pinMode(GND4, OUTPUT);
    ESP_LOGD(TAG, "TrySet: SEGMENT_C");
    pinMode(SEGMENT_C, OUTPUT);
    ESP_LOGD(TAG, "TrySet: SEGMENT_E");
    pinMode(SEGMENT_E, OUTPUT);
    ESP_LOGD(TAG, "TrySet: SEGMENT_DP");
    pinMode(SEGMENT_DP, OUTPUT);
    ESP_LOGD(TAG, "TrySet: SEGMENT_D");
    pinMode(SEGMENT_D, OUTPUT);
    ESP_LOGD(TAG, "TrySet: SEGMENT_A");
    pinMode(SEGMENT_A, OUTPUT);
    ESP_LOGD(TAG, "TrySet: SEGMENT_F");
    pinMode(SEGMENT_F, OUTPUT);
    ESP_LOGD(TAG, "TrySet: SEGMENT_DD");
    pinMode(SEGMENT_DD, OUTPUT);
    ESP_LOGD(TAG, "TrySet: SEGMENT_G");
    pinMode(SEGMENT_G, OUTPUT);
    ESP_LOGD(TAG, "TrySet: SEGMENT_B");
    pinMode(SEGMENT_B, OUTPUT);

    digitalWrite(GNDDD, LOW);
    digitalWrite(GND1, HIGH);
    digitalWrite(GND2, HIGH);
    digitalWrite(GND3, HIGH);
    digitalWrite(GND4, HIGH);
    digitalWrite(SEGMENT_DD, HIGH);
    StartTimer();
}
