#include <string>
#include <vector>

std::vector<std::string> *splitstr(std::string str, std::string deli = "")
{
    std::vector<std::string> *splitVec = new std::vector<std::string>();
    int start = 0;
    int end = str.find(deli);
    while (end != -1)
    {
        splitVec->push_back(str.substr(start, end - start));
        start = end + deli.size();
        end = str.find(deli, start);
    }
    splitVec->push_back(str.substr(start, end - start));

    return splitVec;
}

void logI2CError(const char *TAG, int error)
{
    switch (error)
    {
    case 1:
        ESP_LOGE(TAG, "Error 1");
        ESP_LOGD(TAG, "busy timeout upon entering endTransmission()");
        break;
    case 2:
        ESP_LOGE(TAG, "Error 2");
        ESP_LOGD(TAG, "START bit generation timeout");
        break;
    case 3:
        ESP_LOGE(TAG, "Error 3");
        ESP_LOGD(TAG, "end of address transmission timeout");
        break;
    case 4:
        ESP_LOGE(TAG, "Error 4");
        ESP_LOGD(TAG, "data byte transfer timeout");
        break;
    case 5:
        ESP_LOGE(TAG, "Error 5");
        ESP_LOGD(TAG, "data byte transfer succeeded, busy timeout immediately after");
        break;
    case 6:
        ESP_LOGE(TAG, "Error 6");
        ESP_LOGD(TAG, "timeout waiting for peripheral to clear stop bit");
        break;
    default:
        ESP_LOGE(TAG, "Unknown error");
        break;
    }
}
