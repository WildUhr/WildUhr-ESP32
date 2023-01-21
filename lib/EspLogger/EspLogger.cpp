#include "EspLogger.h"

std::map<std::string, log_level> EspLogger::LogLevelMap = {};
log_level EspLogger::DefaultLogLevel = LOG_NONE;

void EspLogger::LogError(std::string TAG, const char *filePath, const int line, const char *function, std::string message)
{
    if (!EspLogger::CheckLogLevel(TAG, LOG_ERROR))
    {
        return;
    }

    Serial.printf("[E][%s:%i] %s(): [%s] %s \r\n", filePath, line, function, TAG.c_str(), message.c_str());
}

void EspLogger::LogWarning(std::string TAG, const char *filePath, const int line, const char *function, std::string message)
{
    if (!EspLogger::CheckLogLevel(TAG, LOG_WARN))
    {
        return;
    }

    Serial.printf("[W][%s:%i] %s(): [%s] %s \r\n", filePath, line, function, TAG.c_str(), message.c_str());
}

void EspLogger::LogInfo(std::string TAG, const char *filePath, const int line, const char *function, std::string message)
{
    if (!EspLogger::CheckLogLevel(TAG, LOG_INFO))
    {
        return;
    }

    Serial.printf("[I][%s:%i] %s(): [%s] %s \r\n", filePath, line, function, TAG.c_str(), message.c_str());
}

void EspLogger::LogDebug(std::string TAG, const char *filePath, const int line, const char *function, std::string message)
{
    if (!EspLogger::CheckLogLevel(TAG, LOG_DEBUG))
    {
        return;
    }

    Serial.printf("[D][%s:%i] %s(): [%s] %s \r\n", filePath, line, function, TAG.c_str(), message.c_str());
}

void EspLogger::LogVerbos(std::string TAG, const char *filePath, const int line, const char *function, std::string message)
{
    if (!EspLogger::CheckLogLevel(TAG, LOG_VERBOSE))
    {
        return;
    }

    Serial.printf("[V][%s:%i] %s(): [%s] %s \r\n", filePath, line, function, TAG.c_str(), message.c_str());
}

void EspLogger::SetLogLevel(std::string TAG, log_level level)
{
    if (TAG.compare("*") == 0)
    {
        DefaultLogLevel = level;
        return;
    }
    LogLevelMap[TAG] = level;
}

const char *EspLogger::PathToFileName(const char *path)
{
    size_t i = 0;
    size_t pos = 0;
    char *p = (char *)path;
    while (*p)
    {
        i++;
        if (*p == '/' || *p == '\\')
        {
            pos = i;
        }
        p++;
    }
    return path + pos;
}

bool EspLogger::CheckLogLevel(std::string TAG, log_level level)
{
    return (LogLevelMap.find(TAG) == LogLevelMap.end()) ? DefaultLogLevel >= level : LogLevelMap[TAG] >= level;
}