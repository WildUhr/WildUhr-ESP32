#ifndef TCP_DEBUG_H
#define TCP_DEBUG_H

#ifdef TCP_LOGGER_ENABLE

#include "TCPDebugDriver.h"
#include "esp_err.h"
#include "JSONHelper/ErrorJSON.h"
#include "JsonObject.h"

inline extern TCPDebugDriver TCPDebug = TCPDebugDriver();

#define LOG_TRACE(msg, json) TCPDebug.LogTracing(msg, SourceInfo(__FILE__, __LINE__, __FUNCTION__), json)
#define LOG_DEBUG(msg, json) TCPDebug.LogDebug(msg, SourceInfo(__FILE__, __LINE__, __FUNCTION__), json)
#define LOG_INFO(msg, json) TCPDebug.LogInfo(msg, SourceInfo(__FILE__, __LINE__, __FUNCTION__), json)
#define LOG_WARN(msg, json) TCPDebug.LogWarning(msg, SourceInfo(__FILE__, __LINE__, __FUNCTION__), json)
#define LOG_ERROR(msg, json) TCPDebug.LogError(msg, SourceInfo(__FILE__, __LINE__, __FUNCTION__), json)
#define LOG_CRITICAL(msg, json) TCPDebug.LogCritical(msg, SourceInfo(__FILE__, __LINE__, __FUNCTION__), json)
#define CHECK_ERROR(condition) if (condition != ESP_OK) { LOG_ERROR("CHECK_ERROR failed in", dynamic_cast<JsonObject*>(new ErrorJSON(condition))); panic = true;}
#define LOG_IS_INITED TCPDebug.IsInited() 
#else
#include "esp_log.h"
#define LOG_TRACE(msg, json) ESP_LOGV("LOGGER", msg)
#define LOG_DEBUG(msg, json) ESP_LOGD("LOGGER", msg)
#define LOG_INFO(msg, json) ESP_LOGI("LOGGER", msg)
#define LOG_WARN(msg, json) ESP_LOGW("LOGGER", msg)
#define LOG_ERROR(msg, json) ESP_LOGE("LOGGER", msg)
#define LOG_CRITICAL(msg, json) ESP_LOGE("LOGGER", msg)
#define CHECK_ERROR(condition) if (condition != ESP_OK) { ESP_LOGE("LOGGER", "CHECK_ERROR failed in %s:%d", __FILE__, __LINE__); panic = true;}
#define LOG_IS_INITED true 
#endif
#endif