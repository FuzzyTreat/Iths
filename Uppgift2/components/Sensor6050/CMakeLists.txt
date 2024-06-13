idf_component_register(SRCS "Sensor6050.cpp" "mpu6050.c"
                    INCLUDE_DIRS "include"
                    REQUIRES driver freertos esp_timer)
