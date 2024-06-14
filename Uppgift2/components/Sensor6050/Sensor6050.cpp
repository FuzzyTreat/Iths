#include <stdio.h>
#include "mpu6050.h"
#include "Sensor6050.h"

static const char *TAG = "MPU6050";

Sensor6050::Sensor6050(i2c_port_t port) : ic2_port(port)
{
    mpuHandle = mpu6050_create(ic2_port, MPU6050_I2C_ADDRESS);
    mpu6050_config(mpuHandle, ACCE_FS_4G, GYRO_FS_250DPS);
    mpu6050_wake_up(mpuHandle);
}

Sensor6050::~Sensor6050(){}

std::string Sensor6050::GetReadOut(uint16_t selected)
{
    uint16_t value = ReadValue(selected);
    std::string text = "";

    switch (selected)
    {
        case 0:
        {
            text = "Acc. X: ";
            text.append(std::to_string(value));
            text.append(" g");
            break;
        }
        case 1:
        {
            text = "Acc. Y: ";
            text.append(std::to_string(value));
            text.append(" g");
            break;
        }
        case 2:
        {
            text = "Acc. Z: ";
            text.append(std::to_string(value));
            text.append(" g");
            break;
        }
        case 3:
        {
            text = "Rot. X: ";
            text.append(std::to_string(value));
            text.append(" deg");
            break;
        }
        case 4:
        {
            text = "Rot. Y: ";
            text.append(std::to_string(value));
            text.append(" deg");
            break;
        }
        case 5:
        {
            text = "Rot. Z: ";
            text.append(std::to_string(value));
            text.append(" deg");
            break;
        }
        default:
        {
            text = "Invalid value";
            break;
        }
    }
    
    return text;
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
    uint16_t sensorValue = 0;

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


