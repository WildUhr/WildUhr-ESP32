idf_component_register(SRCS "TCPDebugDriver.cpp"
                    INCLUDE_DIRS "include"
                    REQUIRES esp_wifi nvs_flash espcoredump)