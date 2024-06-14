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
    mpu6050_raw_acce_value_t acceleration;
    mpu6050_raw_gyro_value_t rotation;

    i2c_port_t ic2_port;

    int16_t ReadAccelerationValue(uint16_t selected);
    int16_t ReadRotationValue(uint16_t selected);

    protected:

    public:
    Sensor6050(i2c_port_t port);
    ~Sensor6050();
    void GetReadOut(uint16_t selected, std::string &text);
    int16_t ReadValue(uint16_t selected);
};

#endif