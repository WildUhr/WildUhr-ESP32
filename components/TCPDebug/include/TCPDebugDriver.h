#ifndef TCP_DEBUG_DRIVER_H
#define TCP_DEBUG_DRIVER_H
#include <stdio.h>
#include "esp_wifi.h"
#include <sys/socket.h>
#include <string>
#include "JsonObject.h"
#include "SourceInfo.h"
#include "nvs_flash.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_event.h"

#define EXAMPLE_ESP_WIFI_SSID      "FRITZ!Box 7530 PS"
#define EXAMPLE_ESP_WIFI_PASS      "06346084740791889371"
#define EXAMPLE_ESP_MAXIMUM_RETRY  5

#define ESP_WIFI_SCAN_AUTH_MODE_THRESHOLD WIFI_AUTH_WPA2_PSK

/* The event group allows multiple bits for each event, but we only care about two events:
 * - we are connected to the AP with an IP
 * - we failed to connect after the maximum amount of retries */
#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT      BIT1

static void event_handler(void* arg, esp_event_base_t event_base,
    int32_t event_id, void* event_data);

enum LogLevel
{
    TRACE = 0,
    DEBUG,
    INFO,
    WARN,
    ERROR,
    CRITICAL,
    NONE,
};

class TCPDebugDriver
{
private:
    int _sock;
    bool _isConnected = false;
    EventGroupHandle_t s_wifi_event_group;
    int s_retry_num = 0;

private:
    void InitNVS();
    void InitTCP();
    void InitNVS_dep();
    void InitWifi();
    void DeinitWifi();
    void InitWifi_dep();
    void DeinitWifi_dep();
    void DeinitTCP();
    void GenericLog(enum LogLevel LogLevel, const std::string msg, const SourceInfo sourceInfo, JsonObject* json);
    void SendLogLevel(enum LogLevel LogLevel);
    void SendMsg(const std::string msg, const SourceInfo sourceInfo);
    void SendJson(JsonObject* json);
    void SendPackageLength(ushort length);

public:
    TCPDebugDriver()
    {

    }
    ~TCPDebugDriver()
    {
        DeinitTCP();
        DeinitWifi();
    }

    void LogTracing(const std::string msg, const SourceInfo sourceInfo, JsonObject* json = nullptr);
    void LogDebug(const std::string msg, const SourceInfo sourceInfo, JsonObject* json = nullptr);
    void LogInfo(const std::string msg, const SourceInfo sourceInfo, JsonObject* json = nullptr);
    void LogWarning(const std::string msg, const SourceInfo sourceInfo, JsonObject* json = nullptr);
    void LogError(const std::string msg, const SourceInfo sourceInfo, JsonObject* json = nullptr);
    void LogCritical(const std::string msg, const SourceInfo sourceInfo, JsonObject* json = nullptr);
    void Init();
    void Connect();
    void Reconnect();
    void FinishConnect(ip_event_got_ip_t* event_data);
};

#endif