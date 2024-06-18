#include <stdio.h>
#include <string.h>
#include "mpu6050.h"
#include "Sensor6050.h"

static const char *TAG = "MPU6050";

Sensor6050::Sensor6050(i2c_port_t port) : ic2_port(port)
{
    mpuHandle = mpu6050_create(ic2_port, MPU6050_I2C_ADDRESS);
    mpu6050_config(mpuHandle, ACCE_FS_8G, GYRO_FS_2000DPS);
    mpu6050_wake_up(mpuHandle);
}

Sensor6050::~Sensor6050(){}

void Sensor6050::GetReadOut(uint16_t selected, std::string &text)
{
    int16_t value = ReadValue(selected);

    switch (selected)
    {
        case 0:
        {
            text = "Acc. X: ";
            break;
        }
        case 1:
        {
            text = "Acc. Y: ";
            break;
        }
        case 2:
        {
            text = "Acc. Z: ";
            break;
        }
        case 3:
        {
            text = "Rot. X: ";
            break;
        }
        case 4:
        {
            text = "Rot. Y: ";
            break;
        }
        case 5:
        {
            text = "Rot. Z: ";
            
            break;
        }
        default:
        {
            text = "Invalid value";
            break;
        }
    }

    text.append(std::to_string(value));
}

/// @brief Reads a value from the sensor, 0 - 2 reads the accelerometer values, 3 - 5 reads the rotational values
/// @param selected Index of the desired value
int16_t Sensor6050::ReadValue(uint16_t selected)
{
    if(selected <= 2)
    {
        return ReadAccelerationValue(selected);
    }
    else
    {
        return ReadRotationValue(selected);
    }
}

int16_t Sensor6050::ReadAccelerationValue(uint16_t selected)
{
    int16_t sensorValue = 0;

    ESP_ERROR_CHECK_WITHOUT_ABORT(mpu6050_get_raw_acce(mpuHandle, &acceleration));

    switch (selected)
    {
        case 0:
        {
            sensorValue = acceleration.raw_acce_x;
            break;
        }
        case 1:
        {
            sensorValue = acceleration.raw_acce_y;
            break;
        }
        case 2:
        {
            sensorValue = acceleration.raw_acce_z;
            break;
        }
        default:
        {
            sensorValue = 0;
            break;
        }
    }

    return sensorValue;
}

int16_t Sensor6050::ReadRotationValue(uint16_t selected)
{
    int16_t sensorValue = 0;

    ESP_ERROR_CHECK_WITHOUT_ABORT(mpu6050_get_raw_gyro(mpuHandle, &rotation));

    switch (selected)
    {
        case 3:
        {
            sensorValue = rotation.raw_gyro_x;
            break;
        }
        case 4:
        {
            sensorValue = rotation.raw_gyro_y;
            break;
        }
        case 5:
        {
            sensorValue = rotation.raw_gyro_z;
            break;
        }
        default:
        {
            sensorValue = 0;
            break;
        }
    }

    return sensorValue;
}


