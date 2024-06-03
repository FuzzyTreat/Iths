#include <stdio.h>
#include <iostream>
#include "freertos/FreeRTOS.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include <esp_timer.h>
#include "mpu6050.h"

#define I2C_SDA_PIN GPIO_NUM_13
#define I2C_SCL_PIN GPIO_NUM_14
#define I2C_FREQ_HZ 400000
#define I2C_PORT I2C_NUM_0

static const char *TAG = "MPU6050";

mpu6050_handle_t mpuHandle;
mpu6050_raw_acce_value_t acceleration;
mpu6050_raw_gyro_value_t rotation;

extern "C" void app_main(void)
{
    i2c_config_t i2cConfig;

    i2cConfig.mode = I2C_MODE_MASTER;
    i2cConfig.scl_io_num = I2C_SCL_PIN;
    i2cConfig.sda_io_num = I2C_SDA_PIN;
    i2cConfig.scl_pullup_en = GPIO_PULLUP_ENABLE;
    i2cConfig.sda_pullup_en = GPIO_PULLUP_ENABLE;
    i2cConfig.master.clk_speed = I2C_FREQ_HZ;

    ESP_ERROR_CHECK(i2c_param_config(I2C_PORT, &i2cConfig));
    ESP_ERROR_CHECK(i2c_driver_install(I2C_PORT, i2cConfig.mode, 0, 0, 0));
    
    mpuHandle = mpu6050_create(I2C_PORT, MPU6050_I2C_ADDRESS);
    mpu6050_config(mpuHandle, ACCE_FS_4G, GYRO_FS_250DPS);
    mpu6050_wake_up(mpuHandle);
    
    ESP_LOGI(TAG,"Starting loop!");

    // while(true)
    // {
    //     ESP_ERROR_CHECK_WITHOUT_ABORT(mpu6050_get_raw_acce(mpuHandle, &acceleration));
    //     ESP_ERROR_CHECK_WITHOUT_ABORT(mpu6050_get_raw_gyro(mpuHandle, &rotation));

    //     ESP_LOGI(TAG,"X: %d Y: %d Z: %d", acceleration.raw_acce_x, acceleration.raw_acce_y, acceleration.raw_acce_z);

    //     vTaskDelay(pdMS_TO_TICKS(500));        
    // }
}