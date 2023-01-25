#include "SegmentDriver.h"
#include "SegmentDriverCallbackReroute.h"
const char *SegmentDriver::TAG = "SegmentDriver";

void SegmentDriver::WriteDigit(char digit)
{
    ESP32_LOG_V(TAG, string_format("Write Digit: %c", digit));
    auto gpios = digitMap.find(digit);
    if (gpios == digitMap.end())
    {
        ESP32_LOG_E(TAG, string_format("Digit not found: %c", digit));
        return;
    }

    ClearSegment();

    for (auto &&gpio : digitMap.at(digit))
    {
        digitalWrite(gpio, HIGH);
    }
}

void SegmentDriver::ClearSegment()
{
    ESP32_LOG_V(TAG, "Clear Segment");
    digitalWrite(SEGMENT_A, LOW);
    digitalWrite(SEGMENT_B, LOW);
    digitalWrite(SEGMENT_C, LOW);
    digitalWrite(SEGMENT_D, LOW);
    digitalWrite(SEGMENT_E, LOW);
    digitalWrite(SEGMENT_F, LOW);
    digitalWrite(SEGMENT_G, LOW);
}

void SegmentDriver::StartTimer()
{
    esp_timer_create_args_t _timerConfig;
    _timerConfig.arg = reinterpret_cast<void *>(this);
    _timerConfig.callback = segmentDriverCallBackReroute;
    _timerConfig.dispatch_method = ESP_TIMER_TASK;
    _timerConfig.name = "Ticker";
    if (_timer)
    {
        esp_timer_stop(_timer);
        esp_timer_delete(_timer);
    }
    esp_timer_create(&_timerConfig, &_timer);
    esp_timer_start_periodic(_timer, 1000 * 1000ULL);
    ESP32_LOG_I(TAG, "Timer started");
}

void SegmentDriver::SetTime(std::string newTimeString)
{
    if (newTimeString.length() != 4)
        return;
    if (std::all_of(newTimeString.begin(), newTimeString.end(), ::isdigit))
        time = newTimeString;
    ESP32_LOG_V(TAG, string_format("timeset: %s", newTimeString));
}

void SegmentDriver::SwitchMultipex()
{
    digitalWrite(GND1, HIGH);
    digitalWrite(GND2, HIGH);
    digitalWrite(GND3, HIGH);
    digitalWrite(GND4, HIGH);
    ESP32_LOG_D(TAG, string_format("Multiplex with: %s", segmentCounter));

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
        ESP32_LOG_E(TAG, "Multiplexfailed");
        break;
    }
}

void SegmentDriver::OnTimer()
{
    auto digit = time[segmentCounter];
    WriteDigit(digit);
    SwitchMultipex();
    ESP32_LOG_D(TAG, string_format("should show: %s", time));
    segmentCounter++;
    if (segmentCounter > 3)
        segmentCounter = 0;
}

void SegmentDriver::TestDigit()
{
    ESP32_LOG_D(TAG, "TestDigit started");
    while (true)
    {
        digitalWrite(SEGMENT_C, LOW);
        digitalWrite(SEGMENT_E, LOW);
        digitalWrite(SEGMENT_D, LOW);
        digitalWrite(SEGMENT_A, LOW);
        digitalWrite(SEGMENT_B, LOW);
        digitalWrite(SEGMENT_F, LOW);
        digitalWrite(SEGMENT_G, LOW);
        delay(1000);

        digitalWrite(SEGMENT_C, HIGH);
        delay(1000);
        digitalWrite(SEGMENT_E, HIGH);
        delay(1000);
        digitalWrite(SEGMENT_D, HIGH);
        delay(1000);
        digitalWrite(SEGMENT_A, HIGH);
        delay(1000);
        digitalWrite(SEGMENT_B, HIGH);
        delay(1000);
        digitalWrite(SEGMENT_F, HIGH);
        delay(1000);
        digitalWrite(SEGMENT_G, HIGH);
        delay(1000);
    }
}

void SegmentDriver::TestMultiplex()
{
    ESP32_LOG_D(TAG, "TestMultiplex started");
    while (true)
    {
        WriteDigit('8');
        digitalWrite(GND1, HIGH);
        digitalWrite(GND2, HIGH);
        digitalWrite(GND3, HIGH);
        digitalWrite(GND4, HIGH);
        ESP32_LOG_D(TAG, "TestMultiplex 1");
        digitalWrite(GND1, LOW);
        delay(1000);
        ESP32_LOG_D(TAG, "TestMultiplex 2");
        digitalWrite(GND2, LOW);
        delay(1000);
        ESP32_LOG_D(TAG, "TestMultiplex 3");
        digitalWrite(GND3, LOW);
        delay(1000);
        ESP32_LOG_D(TAG, "TestMultiplex 4");
        digitalWrite(GND4, LOW);
        delay(1000);
    }
}

void SegmentDriver::Setup()
{
    ESP32_LOG_D(TAG, "TrySet: GNDDD");
    pinMode(GNDDD, OUTPUT);
    ESP32_LOG_D(TAG, "TrySet: GND1");
    pinMode(GND1, OUTPUT);
    ESP32_LOG_D(TAG, "TrySet: GND2");
    pinMode(GND2, OUTPUT);
    ESP32_LOG_D(TAG, "TrySet: GND3");
    pinMode(GND3, OUTPUT);
    ESP32_LOG_D(TAG, "TrySet: GND4");
    pinMode(GND4, OUTPUT);
    ESP32_LOG_D(TAG, "TrySet: SEGMENT_C");
    pinMode(SEGMENT_C, OUTPUT);
    ESP32_LOG_D(TAG, "TrySet: SEGMENT_E");
    pinMode(SEGMENT_E, OUTPUT);
    ESP32_LOG_D(TAG, "TrySet: SEGMENT_D");
    pinMode(SEGMENT_D, OUTPUT);
    ESP32_LOG_D(TAG, "TrySet: SEGMENT_A");
    pinMode(SEGMENT_A, OUTPUT);
    ESP32_LOG_D(TAG, "TrySet: SEGMENT_F");
    pinMode(SEGMENT_F, OUTPUT);
    ESP32_LOG_D(TAG, "TrySet: SEGMENT_G");
    pinMode(SEGMENT_G, OUTPUT);
    ESP32_LOG_D(TAG, "TrySet: SEGMENT_B");
    pinMode(SEGMENT_B, OUTPUT);

    digitalWrite(GNDDD, HIGH);
    digitalWrite(GND1, HIGH);
    digitalWrite(GND2, HIGH);
    digitalWrite(GND3, HIGH);
    digitalWrite(GND4, HIGH);
    // StartTimer();
}
