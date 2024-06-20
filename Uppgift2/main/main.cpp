#include <stdio.h>
#include <iostream>
#include <string>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/i2c.h"
#include "esp_log.h"
#include <esp_timer.h>
#include "Button.h"
#include "ValueSelector.h"
#include "Sensor6050.h"
#include "LCD1602.h"

#define UP_BUTTON_PIN GPIO_NUM_18
#define DOWN_BUTTON_PIN GPIO_NUM_19

#define I2C_SDA_PIN GPIO_NUM_13
#define I2C_SCL_PIN GPIO_NUM_14
#define I2C_FREQ_HZ 100000 // Standard-mode: 100KHz, Fast-mode: 400KHz. Should be max fast-mode if LCD isn't used otherwise 100 KHz
#define I2C_PORT I2C_NUM_0

const char *TAG = "Main";

Button *upButton;
Button *downButton;
ValueSelector *selector;
Sensor6050 *sensor;
LCD1602 *lcd;

void OnButtonPressed(void *ptr);
void OnButtonReleased(void *ptr);
void RegisterComponents();
void OnSelectedValueChanged(void *ptr);
void HandleButtons(void *ptr);

std::string displayText = "";
BaseType_t xReturned;
TaskHandle_t xHandle = NULL;

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

    RegisterComponents();

    xTaskCreate(HandleButtons, "ButtonEvent", 6000, (void *) 1, tskIDLE_PRIORITY, &xHandle);

    while(true)
    {
        sensor->GetReadOut((uint16_t)selector->GetCurrentValue(), displayText);

        // Send text to LCD display
        lcd->lcd_print(displayText.c_str());
        
        vTaskDelay(pdMS_TO_TICKS(50));
        lcd->ClearScreen();
    }

    if(xHandle != NULL)
    {
        vTaskDelete(xHandle);
    }
}

void RegisterComponents()
{
    ESP_LOGI(TAG,"Registering up button.");
    upButton = new Button(UP_BUTTON_PIN, PT_none);
    upButton->SetOnPressed(OnButtonPressed, (void *)upButton);

    ESP_LOGI(TAG,"Registering down button.");
    downButton = new Button(DOWN_BUTTON_PIN, PT_none);
    downButton->SetOnPressed(OnButtonPressed, (void *)downButton);

    ESP_LOGI(TAG,"Registering selector.");
    selector = new ValueSelector(SelectedValue_e::Acceleration_X);
    selector->SetOnChanged(NULL, (void *)selector);

    ESP_LOGI(TAG,"Registering mpu6050.");
    sensor = new Sensor6050(I2C_PORT, ACCE_FS_2G, GYRO_FS_250DPS);

    ESP_LOGI(TAG,"Registering lcd 1602.");
    lcd = new LCD1602(I2C_SCL_PIN, I2C_SDA_PIN, I2C_PORT);
    lcd->lcd_init();
}

void OnButtonPressed(void *ptr)
{
    gpio_num_t pin = ((Button *)ptr)->GetPin();

    switch (pin)
    {
        case UP_BUTTON_PIN:
            {
                selector->SetDirection(MoveUp);
                break;
            }
        case DOWN_BUTTON_PIN:
            {
                selector->SetDirection(MoveDown);
                break;
            }
        default:
            break;
    }
    
    // ESP_LOGI(TAG,"Button pressed on pin %d", pin);
}

void OnSelectedValueChanged(void *ptr)
{
    // Left empty intentionally

    ValueSelector selector = *((ValueSelector *)ptr);
    
    // // Read the current value from the sensor.
    sensor->GetReadOut((uint16_t)selector.GetCurrentValue(), displayText);

    lcd->ClearScreen();
    lcd->lcd_print(displayText.c_str());
    // ESP_LOGI(TAG,"%s", text.data());
}

void HandleButtons(void *ptr)
{
    for(;;)
    {
        upButton->Update();
        downButton->Update();

        vTaskDelay(pdMS_TO_TICKS(50));
    }
}