#ifndef SENSOR6050_H
#define SENSOR6050_H

#include <stdio.h>
#include <iostream>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include <esp_timer.h>
#include "mpu6050.h"

class Sensor6050
{
    private:
        mpu6050_handle_t mpuHandle;

        i2c_port_t i2c_port;

        mpu6050_acce_fs_t accelerationSensitivity;
        mpu6050_gyro_fs_t rotationSensitivity;

        mpu6050_acce_value_t acceleration;
        mpu6050_gyro_value_t rotation;

        int16_t ReadAccelerationValue(uint16_t selected);
        int16_t ReadRotationValue(uint16_t selected);

    protected:

    public:
        Sensor6050(i2c_port_t port, mpu6050_acce_fs_t accSensitivity, mpu6050_gyro_fs_t rotSensitivity);
        ~Sensor6050();
        void GetReadOut(uint16_t selected, std::string &text);
        int16_t ReadValue(uint16_t selected);
        void GetDeviceId(uint8_t *deviceid);
};

#endif