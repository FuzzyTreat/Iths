#include <stdio.h>
#include <iostream>
#include "freertos/FreeRTOS.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include <esp_timer.h>

#define INTERRUPT_PIN GPIO_NUM_27

static const char *TAG = "MAIN";

#define I2C_SDA_PIN GPIO_PIN_18
#define I2C_SDA_ADDRESS 

// 0110 1000 - AD0 low
// 0110 1001 - AD0 high

#define I2C_SCL_PIN GPIO_PIN_19
#define I2C_SCL_ADDRESS

extern "C" void app_main(void)
{

}