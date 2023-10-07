#include "TCPDebugDriver.h"


void TCPDebugDriver::InitTCP()
{
    try
    {
        struct sockaddr_in tcpServerAddr;
        tcpServerAddr.sin_addr.s_addr = inet_addr("192.168.178.32");
        tcpServerAddr.sin_family = AF_INET;
        tcpServerAddr.sin_port = htons(13000);

        _sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (_sock < 0)
            return;

        int err = connect(_sock, (struct sockaddr*)&tcpServerAddr, sizeof(tcpServerAddr));
    }
    catch (const std::exception& e)
    {
    }
}

void TCPDebugDriver::DeinitTCP()
{
    close(_sock);
}

void TCPDebugDriver::GenericLog(enum LogLevel LogLevel, const std::string msg, const SourceInfo sourceInfo, JsonObject* json)
{
    if (!_isConnected){
        ESP_LOGW("LOGGER", "not Connected");
        this->Init();

    }
    SendLogLevel(LogLevel);
    SendMsg(msg, sourceInfo);
    SendJson(json);
    ESP_LOGW("LOGGER", "%s", msg.c_str());
    free(json);
}

void TCPDebugDriver::SendLogLevel(enum LogLevel LogLevel)
{
    char* buffer = (char*)malloc(1);
    buffer[0] = LogLevel;
    if(send(_sock, buffer, 1, 0) < 0)
        ESP_LOGW("LOGGER", "send failed with %d", errno);
    free(buffer);
}

void TCPDebugDriver::SendMsg(std::string msg, const SourceInfo sourceInfo)
{
    msg.append("\n");
    msg.append(sourceInfo.file);
    msg.append(": ");
    msg.append(std::to_string(sourceInfo.line));
    msg.append(" ");
    msg.append(sourceInfo.func);

    char* buffer = (char*)malloc(msg.length() + 1);
    strcpy(buffer, msg.c_str());
    SendPackageLength(strlen(buffer));
    send(_sock, buffer, strlen(buffer), 0);
    free(buffer);
}

void TCPDebugDriver::SendJson(JsonObject* json)
{
    if (!json) {
        SendPackageLength(0);
        return;
    }

    std::string jsonStr = json->ToJson();
    char* buffer = (char*)malloc(jsonStr.length() + 1);
    strcpy(buffer, jsonStr.c_str());
    SendPackageLength(strlen(buffer));
    send(_sock, buffer, strlen(buffer), 0);
    free(buffer);
}

void TCPDebugDriver::SendPackageLength(ushort length)
{
    char* buffer = (char*)malloc(2);
    buffer[0] = length & 0xFF;
    buffer[1] = length >> 8;

    send(_sock, buffer, 2, 0);
    free(buffer);
}

void TCPDebugDriver::InitNVS()
{
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
}

void TCPDebugDriver::InitWifi()
{
    s_wifi_event_group = xEventGroupCreate();
    ESP_ERROR_CHECK(esp_netif_init());

    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    esp_event_handler_instance_t instance_any_id;
    esp_event_handler_instance_t instance_got_ip;
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
        ESP_EVENT_ANY_ID,
        &event_handler,
        this,
        &instance_any_id));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
        IP_EVENT_STA_GOT_IP,
        &event_handler,
        this,
        &instance_got_ip));

    wifi_config_t wifi_config = {
        {
            EXAMPLE_ESP_WIFI_SSID,
            EXAMPLE_ESP_WIFI_PASS,
            /* Authmode threshold resets to WPA2 as default if password matches WPA2 standards (pasword len => 8).
             * If you want to connect the device to deprecated WEP/WPA networks, Please set the threshold value
             * to WIFI_AUTH_WEP/WIFI_AUTH_WPA_PSK and set the password with length and format matching to
             * WIFI_AUTH_WEP/WIFI_AUTH_WPA_PSK standards.
             */
            ESP_WIFI_SCAN_AUTH_MODE_THRESHOLD,
        }
    };
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());


    /* Waiting until either the connection is established (WIFI_CONNECTED_BIT) or connection failed for the maximum
     * number of re-tries (WIFI_FAIL_BIT). The bits are set by event_handler() (see above) */
    EventBits_t bits = xEventGroupWaitBits(s_wifi_event_group,
        WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
        pdFALSE,
        pdFALSE,
        portMAX_DELAY);
}

void TCPDebugDriver::DeinitWifi()
{

}

void TCPDebugDriver::Connect()
{
    esp_wifi_connect();
}

void TCPDebugDriver::Reconnect()
{
    if (s_retry_num < EXAMPLE_ESP_MAXIMUM_RETRY) {
        esp_wifi_connect();
        s_retry_num++;
    }
    else {
        xEventGroupSetBits(s_wifi_event_group, WIFI_FAIL_BIT);
    }

}

void TCPDebugDriver::FinishConnect(ip_event_got_ip_t* event_data)
{
    s_retry_num = 0;
    xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
    _isConnected = true;

}

void TCPDebugDriver::Init()
{
    InitNVS();

    InitWifi();
    InitTCP();
    ReadCoreDump();
}

static void event_handler(void* arg, esp_event_base_t event_base,
    int32_t event_id, void* event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        ((TCPDebugDriver*)arg)->Connect();
    }
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        ((TCPDebugDriver*)arg)->Reconnect();
    }
    else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ((TCPDebugDriver*)arg)->FinishConnect((ip_event_got_ip_t*)event_data);
    }
}

void TCPDebugDriver::LogTracing(const std::string msg, const SourceInfo sourceInfo, JsonObject* json)
{
    GenericLog(LogLevel::TRACE, msg, sourceInfo, json);
}

void TCPDebugDriver::LogDebug(const std::string msg, const SourceInfo sourceInfo, JsonObject* json)
{
    GenericLog(LogLevel::DEBUG, msg, sourceInfo, json);
}

void TCPDebugDriver::LogInfo(const std::string msg, const SourceInfo sourceInfo, JsonObject* json)
{
    GenericLog(LogLevel::INFO, msg, sourceInfo, json);
}

void TCPDebugDriver::LogWarning(const std::string msg, const SourceInfo sourceInfo, JsonObject* json)
{
    GenericLog(LogLevel::WARN, msg, sourceInfo, json);
}

void TCPDebugDriver::LogError(const std::string msg, const SourceInfo sourceInfo, JsonObject* json)
{
    GenericLog(LogLevel::ERROR, msg, sourceInfo, json);
}

void TCPDebugDriver::LogCritical(const std::string msg, const SourceInfo sourceInfo, JsonObject* json)
{
    GenericLog(LogLevel::CRITICAL, msg, sourceInfo, json);
}

void TCPDebugDriver::ReadCoreDump()
{
    esp_core_dump_summary_t *summary = (esp_core_dump_summary_t *)malloc(sizeof(esp_core_dump_summary_t));
    if (summary) {
        auto error = esp_core_dump_get_summary(summary);
        if (error == ESP_OK) {
            LogCritical("Core dump", SourceInfo(__FILE__, __LINE__, __FUNCTION__), dynamic_cast<JsonObject*>(new CoreDumpJSON(*summary)));
        }
        else{
        LogCritical("Core dump not found", SourceInfo(__FILE__, __LINE__, __FUNCTION__), dynamic_cast<JsonObject*>(new ErrorJSON(error)));
        }
    }
    else{
            LogCritical("Core dump not found", SourceInfo(__FILE__, __LINE__, __FUNCTION__));
    }
    free(summary);
}

bool TCPDebugDriver::IsInited()
{
    return _isConnected;
}