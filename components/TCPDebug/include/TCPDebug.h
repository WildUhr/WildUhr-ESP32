#ifndef TCP_DEBUG_H
#define TCP_DEBUG_H
#include "TCPDebugDriver.h"

inline extern TCPDebugDriver TCPDebug = TCPDebugDriver();

#define LOG_TRACE(msg, json) TCPDebug.LogTracing(msg, SourceInfo(__FILE__, __LINE__, __FUNCTION__), json)
#define LOG_DEBUG(msg, json) TCPDebug.LogDebug(msg, SourceInfo(__FILE__, __LINE__, __FUNCTION__), json)
#define LOG_INFO(msg, json) TCPDebug.LogInfo(msg, SourceInfo(__FILE__, __LINE__, __FUNCTION__), json)
#define LOG_WARN(msg, json) TCPDebug.LogWarning(msg, SourceInfo(__FILE__, __LINE__, __FUNCTION__), json)
#define LOG_ERROR(msg, json) TCPDebug.LogError(msg, SourceInfo(__FILE__, __LINE__, __FUNCTION__), json)
#define LOG_CRITICAL(msg, json) TCPDebug.LogCritical(msg, SourceInfo(__FILE__, __LINE__, __FUNCTION__), json)

#endif