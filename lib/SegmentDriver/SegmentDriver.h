//        +--------\/--------+
//        |    |‾‾‾｜‾‾‾|    |             ___A___
//  GND1  |1*  ·‾‾‾  ‾‾‾    8|  A         ｜      ｜
//     D  |2   |‾‾‾｜‾‾‾|   9|  F        F｜      ｜B
//    DP  |3   ·‾‾‾  ‾‾‾   10|  GND2      ｜      ｜     ·
//     E  |4     ·    ·    11|  DD         ⎯⎯⎯G⎯⎯⎯⎯        DD
//  GND3  |5   |‾‾‾｜‾‾‾|  12|  G         ｜      ｜     ·
//     C  |6   ·‾‾‾  ‾‾‾   13|  GND4     E｜      ｜C
// GNDDD  |7   |‾‾‾｜‾‾‾|  14|  B         ｜      ｜
//        |    ·‾‾‾  ‾‾‾     |             ‾‾‾D‾‾‾‾ ˙ DP
//        +--------=--------+
#ifndef SEGMENTDRIVER_H
#define SEGMENTDRIVER_H

#include <string>
#include <map>
#include <vector>
#include <Arduino.h>
extern "C"
{
#include "esp_timer.h"
}
#define GNDDD 17
#define GND1 16
#define GND2 32
#define GND3 33
#define GND4 25
#define SEGMENT_C 26
#define SEGMENT_E 27
#define SEGMENT_DP 14
#define SEGMENT_D 12
#define SEGMENT_A 22
#define SEGMENT_F 23
#define SEGMENT_DD 19
#define SEGMENT_G 18
#define SEGMENT_B 5

class SegmentDriver
{
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