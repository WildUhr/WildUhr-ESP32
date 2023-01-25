//        +--------\/--------+
//        |    |‾‾‾｜‾‾‾|    |             ___A___
//  GND1  |1*  ·‾‾‾  ‾‾‾   17|  B         ｜      ｜
//  GND2  |2   |‾‾‾｜‾‾‾|  16|  G        F｜      ｜B
//     D  |3   ·‾‾‾  ‾‾‾   15|  A         ｜      ｜     ·
//  GNDD  |4     ·    ·    14|  C          ⎯⎯⎯G⎯⎯⎯⎯        DD
//     E  |5   |‾‾‾｜‾‾‾|  13|  -         ｜      ｜     ·
//  GND3  |6   ·‾‾‾  ‾‾‾·  11|  F        E｜      ｜C
//    DP  |7   |‾‾‾｜‾‾‾|  10|  -         ｜      ｜
//  GND4  |8   ·‾‾‾  ‾‾‾   9 |  ?          ‾‾‾D‾‾‾‾ ˙ DP
//        +--------=--------+
#ifndef SEGMENTDRIVER_H
#define SEGMENTDRIVER_H

#include <string>
#include <map>
#include <vector>
#include <Arduino.h>
#include "EspLogger.h"
#include "Helper.h"
extern "C"
{
#include "esp_timer.h"
}
#define GNDDD GPIO_NUM_10
#define GND1 GPIO_NUM_2
#define GND2 GPIO_NUM_1
#define GND3 GPIO_NUM_3
#define GND4 GPIO_NUM_0
#define SEGMENT_C GPIO_NUM_9
#define SEGMENT_E GPIO_NUM_7
#define SEGMENT_D GPIO_NUM_8
#define SEGMENT_A GPIO_NUM_19
#define SEGMENT_B GPIO_NUM_18
#define SEGMENT_F GPIO_NUM_6
#define SEGMENT_DD1 SEGMENT_A
#define SEGMENT_DD2 SEGMENT_B
#define SEGMENT_G GPIO_NUM_5

class SegmentDriver
{
public:
    static const char *TAG;

private:
    const std::map<char, std::vector<short>> digitMap = {
        {'0', {SEGMENT_A, SEGMENT_B, SEGMENT_C, SEGMENT_D, SEGMENT_E, SEGMENT_F}},
        {'1', {SEGMENT_B, SEGMENT_C}},
        {'2', {SEGMENT_A, SEGMENT_B, SEGMENT_D, SEGMENT_E, SEGMENT_G}},
        {'3', {SEGMENT_A, SEGMENT_B, SEGMENT_C, SEGMENT_D, SEGMENT_G}},
        {'4', {SEGMENT_B, SEGMENT_C, SEGMENT_F, SEGMENT_G}},
        {'5', {SEGMENT_A, SEGMENT_C, SEGMENT_D, SEGMENT_F, SEGMENT_G}},
        {'6', {SEGMENT_A, SEGMENT_C, SEGMENT_D, SEGMENT_E, SEGMENT_F, SEGMENT_G}},
        {'7', {
                  SEGMENT_A,
                  SEGMENT_B,
                  SEGMENT_C,
              }},
        {'8', {SEGMENT_A, SEGMENT_B, SEGMENT_C, SEGMENT_D, SEGMENT_E, SEGMENT_F, SEGMENT_G}},
        {'9', {SEGMENT_A, SEGMENT_B, SEGMENT_C, SEGMENT_F, SEGMENT_G}}};
    std::string time;
    short segmentCounter = 0;
    esp_timer_handle_t _timer;

private:
    void WriteDigit(char digit);
    void ClearSegment();
    void StartTimer();
    void SwitchMultipex();

public:
    void TestMultiplex();
    void TestDigit();
    SegmentDriver(/* args */) : _timer(nullptr) {}

    ~SegmentDriver()
    {
        if (_timer)
        {
            esp_timer_stop(_timer);
            esp_timer_delete(_timer);
            _timer = nullptr;
        }
    }

    void OnTimer();
    void Setup();
    void SetTime(std::string newTimeString);
};

#endif