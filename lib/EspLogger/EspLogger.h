#ifndef ESP_LOGGER_H
#define ESP_LOGGER_H
#include "Arduino.h"
#include <string>
#include <map>
typedef enum
{
    LOG_NONE,   /*!< No log output */
    LOG_ERROR,  /*!< Critical errors, software module can not recover on its own */
    LOG_WARN,   /*!< Error conditions from which recovery measures have been taken */
    LOG_INFO,   /*!< Information messages which describe normal flow of events */
    LOG_DEBUG,  /*!< Extra information which is not necessary for normal use (values, pointers, sizes, etc). */
    LOG_VERBOSE /*!< Bigger chunks of debugging information, or frequent messages which can potentially flood the output. */
} log_level;

class EspLogger
{
public:
private:
    static std::map<std::string, log_level> LogLevelMap;
    static log_level DefaultLogLevel;

public:
    static const char *IRAM_ATTR PathToFileName(const char *path);
    static void IRAM_ATTR LogError(std::string TAG, const char *filePath, const int line, const char *function, std::string message);
    static void IRAM_ATTR LogWarning(std::string TAG, const char *filePath, const int line, const char *function, std::string message);
    static void IRAM_ATTR LogInfo(std::string TAG, const char *filePath, const int line, const char *function, std::string message);
    static void IRAM_ATTR LogDebug(std::string TAG, const char *filePath, const int line, const char *function, std::string message);
    static void IRAM_ATTR LogVerbos(std::string TAG, const char *filePath, const int line, const char *function, std::string message);
    static void IRAM_ATTR SetLogLevel(std::string TAG, log_level level);
    EspLogger(/* args */) = delete;
    ~EspLogger() = delete;

private:
    static bool IRAM_ATTR CheckLogLevel(std::string TAG, log_level level);
};
#define ESP32_SET_LOG_LEVEL(TAG, level) EspLogger::SetLogLevel(TAG, level);

#define ESP32_LOG_E(TAG, message) EspLogger::LogError(TAG, EspLogger::PathToFileName(__FILE__), __LINE__, __FUNCTION__, message);
#define ESP32_LOG_W(TAG, message) EspLogger::LogWarning(TAG, EspLogger::PathToFileName(__FILE__), __LINE__, __FUNCTION__, message);
#define ESP32_LOG_I(TAG, message) EspLogger::LogInfo(TAG, EspLogger::PathToFileName(__FILE__), __LINE__, __FUNCTION__, message);
#define ESP32_LOG_D(TAG, message) EspLogger::LogDebug(TAG, EspLogger::PathToFileName(__FILE__), __LINE__, __FUNCTION__, message);
#define ESP32_LOG_V(TAG, message) EspLogger::LogVerbos(TAG, EspLogger::PathToFileName(__FILE__), __LINE__, __FUNCTION__, message);

#endif