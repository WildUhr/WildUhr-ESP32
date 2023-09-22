#include "TCPDebugDriver.h"

void TCPDebugDriver::InitNVS_dep()
{
    esp_err_t error = nvs_flash_init();
    if (error == ESP_ERR_NVS_NO_FREE_PAGES || error == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        error = nvs_flash_erase();

        error = nvs_flash_init();
    }
}

void TCPDebugDriver::InitWifi_dep()
{
    
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_err_t error = ESP_OK;
    try
    {
        /* code */
         error = esp_wifi_init(&cfg);
    }
    catch(const std::exception& e)
    {
    }
    

    wifi_config_t wifi_config = {
        .sta = {
            .ssid = "FRITZ!Box 7530 PS",
            .password = "06346084740791889371"
        }
    };
    error = esp_wifi_set_config(WIFI_IF_STA, &wifi_config);
    error = esp_wifi_set_mode(WIFI_MODE_STA);
    error = esp_wifi_start();
    error = esp_wifi_connect();
}

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

        int err = connect(_sock, (struct sockaddr *)&tcpServerAddr, sizeof(tcpServerAddr));
    }
    catch(const std::exception& e)
    {
    }
}

void TCPDebugDriver::DeinitWifi_dep()
{
    esp_wifi_stop();
}

void TCPDebugDriver::DeinitTCP()
{
    close(_sock);
}

void TCPDebugDriver::GenericLog(enum LogLevel LogLevel, const std::string msg, const SourceInfo sourceInfo, const JsonObject *json)
{
    if (!_isConnected)
        this->Init();
    SendLogLevel(LogLevel);
    SendMsg(msg);
    SendJson(json);
}

void TCPDebugDriver::SendLogLevel(enum LogLevel LogLevel)
{
    char *buffer = (char *)malloc(1);
    buffer[0] = LogLevel;
    send(_sock, buffer, 1, 0);
    free(buffer);
}

void TCPDebugDriver::SendMsg(const std::string msg)
{
    char *buffer = (char *)malloc(msg.length() + 1);
    strcpy(buffer, msg.c_str());
    SendPackageLength(strlen(buffer));
    send(_sock, buffer, strlen(buffer), 0);
    free(buffer);
}

void TCPDebugDriver::SendJson(const JsonObject *json)
{
    if (!json){
        SendPackageLength(0);
        return;
    }
    
    std::string jsonStr = json->ToJson();
    char *buffer = (char *)malloc(jsonStr.length() + 1);
    strcpy(buffer, jsonStr.c_str());
    SendPackageLength(strlen(buffer));
    send(_sock, buffer, strlen(buffer), 0);
    free(buffer);
}

void TCPDebugDriver::SendPackageLength(ushort length)
{
    char *buffer = (char *)malloc(2);
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
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA) );
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config) );
    ESP_ERROR_CHECK(esp_wifi_start() );


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
        } else {
            xEventGroupSetBits(s_wifi_event_group, WIFI_FAIL_BIT);
        }

}

void TCPDebugDriver::FinishConnect(ip_event_got_ip_t* event_data)
{
    s_retry_num = 0;
    xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);}

void TCPDebugDriver::Init()
{
    this->InitNVS();

    this->InitWifi();
    InitTCP();
    _isConnected = true;

}

static void event_handler(void* arg, esp_event_base_t event_base,
                                int32_t event_id, void* event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        ((TCPDebugDriver *)arg)->Connect();
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        ((TCPDebugDriver *)arg)->Reconnect();
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ((TCPDebugDriver *)arg)->FinishConnect((ip_event_got_ip_t*) event_data);
    }
}

void TCPDebugDriver::LogTracing(const std::string msg, const SourceInfo sourceInfo, const JsonObject *json)
{
    GenericLog(LogLevel::TRACE, msg, sourceInfo, json);
}

void TCPDebugDriver::LogDebug(const std::string msg, const SourceInfo sourceInfo, const JsonObject *json)
{
    GenericLog(LogLevel::DEBUG, msg, sourceInfo, json);
}

void TCPDebugDriver::LogInfo(const std::string msg, const SourceInfo sourceInfo, const JsonObject *json)
{
    GenericLog(LogLevel::INFO, msg, sourceInfo, json);
}

void TCPDebugDriver::LogWarning(const std::string msg, const SourceInfo sourceInfo, const JsonObject *json)
{
    GenericLog(LogLevel::WARN, msg, sourceInfo, json);
}

void TCPDebugDriver::LogError(const std::string msg, const SourceInfo sourceInfo, const JsonObject *json)
{
    GenericLog(LogLevel::ERROR, msg, sourceInfo, json);
}

void TCPDebugDriver::LogCritical(const std::string msg, const SourceInfo sourceInfo, const JsonObject *json)
{
    GenericLog(LogLevel::CRITICAL, msg, sourceInfo, json);
}
