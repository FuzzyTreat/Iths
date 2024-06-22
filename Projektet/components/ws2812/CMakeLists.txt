idf_component_register(SRCS "ws2812.cpp"
                    INCLUDE_DIRS "include"
                    REQUIRES driver freertos esp_timer espressif__led_strip)
