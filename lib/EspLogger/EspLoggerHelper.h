#ifndef ESP_LOGGER_HELPER_H
#define ESP_LOGGER_HELPER_H
#include "EspLogger.h"
// clang-format off
#define ESP32_LOG_I2C(TAG, error)                                                  \                                                              
    switch (error)                                                                            \                                  
    {                                                                                         \                      
    case 1:                                                                                   \                          
        ESP32_LOG_E(TAG, "Error 1");                                                          \                                                      
        ESP32_LOG_D(TAG, "busy timeout upon entering endTransmission()");                     \                                                                                          
        break;                                                                                \                              
    case 2:                                                                                   \                          
        ESP32_LOG_E(TAG, "Error 2");                                                          \                                                      
        ESP32_LOG_D(TAG, "START bit generation timeout");                                     \                                                                          
        break;                                                                                \                              
    case 3:                                                                                   \                          
        ESP32_LOG_E(TAG, "Error 3");                                                          \                                                      
        ESP32_LOG_D(TAG, "end of address transmission timeout");                              \                                                                                  
        break;                                                                                \                              
    case 4:                                                                                   \                          
        ESP32_LOG_E(TAG, "Error 4");                                                          \                                                      
        ESP32_LOG_D(TAG, "data byte transfer timeout");                                       \                                                                      
        break;                                                                                \                              
    case 5:                                                                                   \                          
        ESP32_LOG_E(TAG, "Error 5");                                                          \                                                      
        ESP32_LOG_D(TAG, "data byte transfer succeeded, busy timeout immediately after");     \                                                                                                          
        break;                                                                                \                              
    case 6:                                                                                   \                          
        ESP32_LOG_E(TAG, "Error 6");                                                          \                                                      
        ESP32_LOG_D(TAG, "timeout waiting for peripheral to clear stop bit");                 \                                                                                              
        break;                                                                                \                              
    default:                                                                                  \                              
        ESP32_LOG_E(TAG, "Unknown error");                                                    \                                                          
        break;                                                                                \                              
    }                                                                                         \                      
// clang-format on

#endif