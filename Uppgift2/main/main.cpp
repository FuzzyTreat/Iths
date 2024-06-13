#include <stdio.h>
#include <iostream>
#include "freertos/FreeRTOS.h"
#include "driver/gpio.h"
#include "driver/i2c.h"
#include "esp_log.h"
#include <esp_timer.h>
#include "Button.h"
#include "ValueSelector.h"
// #include "mpu6050.h"
#include "Sensor6050.h"

#define UP_BUTTON_PIN GPIO_NUM_18
#define DOWN_BUTTON_PIN GPIO_NUM_19

#define LED_RED_PIN GPIO_NUM_25
#define LED_YELLOW_PIN GPIO_NUM_26

#define I2C_SDA_PIN GPIO_NUM_13
#define I2C_SCL_PIN GPIO_NUM_14
#define I2C_FREQ_HZ 100000 // Standard-mode: 100KHz, Fast-mode: 400KHz. Should be max fast-mode
#define I2C_PORT I2C_NUM_0

const char *TAG = "Main";

Button *upButton;
Button *downButton;

void OnButtonPressed(void *ptr);
void OnButtonReleased(void *ptr);
void RegisterButtons();


ValueSelector *selector;
void OnSelectedValueChanged(void *ptr);

Sensor6050 *sensor;

void SetupI2C();

void SetupLedGpio();

gpio_num_t prevButton;

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

    // SetupLedGpio(); // Can be removed, just for debug purpose
    
    RegisterButtons();

    while(true)
    {
        vTaskDelay(pdMS_TO_TICKS(50));
        gpio_set_level(LED_RED_PIN, 0);
        gpio_set_level(LED_YELLOW_PIN, 0);

        upButton->Update();
        downButton->Update();
    }
}

void SetupLedGpio()
{
    gpio_pulldown_en(LED_RED_PIN);
    gpio_pulldown_en(LED_YELLOW_PIN);

    gpio_pullup_dis(LED_RED_PIN);
    gpio_pullup_dis(LED_YELLOW_PIN);

    gpio_set_level(LED_RED_PIN, 0);
    gpio_set_level(LED_YELLOW_PIN, 0);

    gpio_set_direction(LED_RED_PIN, GPIO_MODE_OUTPUT);
    gpio_set_direction(LED_YELLOW_PIN, GPIO_MODE_OUTPUT);
};

void RegisterButtons()
{
    ESP_LOGI(TAG,"Registering up button.");
    upButton = new Button(UP_BUTTON_PIN, PT_down);
    upButton->SetOnPressed(OnButtonPressed, (void *)upButton);
    upButton->SetOnRelease(OnButtonReleased, (void *)upButton);

    ESP_LOGI(TAG,"Registering down button.");
    downButton = new Button(DOWN_BUTTON_PIN, PT_down);
    downButton->SetOnPressed(OnButtonPressed, (void *)downButton);
    downButton->SetOnRelease(OnButtonReleased, (void *)downButton);

    ESP_LOGI(TAG,"Registering selector.");
    selector = new ValueSelector(SelectedValue_e::Acceleration_X);
    selector->SetOnChanged(OnSelectedValueChanged, (void *)selector);

    ESP_LOGI(TAG,"Registering mpu6050.");
    sensor = new Sensor6050(I2C_PORT);
}

void OnButtonPressed(void *ptr)
{
    gpio_num_t pin = ((Button *)ptr)->GetPin();
    prevButton = pin;

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

    
    ESP_LOGI(TAG,"Button pressed on pin %d", pin);
}

void OnButtonReleased(void *ptr)
{
    // gpio_num_t pin = ((Button *)ptr)->GetPin();
    // ESP_LOGI(TAG,"Button released on pin %d", pin);

    int16_t value = sensor->ReadValue((int16_t)selector->GetCurrentValue());
    ESP_LOGI(TAG,"Value from sensor %d", value);
}

void OnSelectedValueChanged(void *ptr)
{
    ValueSelector selector = *((ValueSelector *)ptr);

    // Read Value from Accelerometer here!!
    // Send text to LCD display
    
    switch (selector.GetDirection())
    {
        case MoveUp:
        {
            gpio_set_level(LED_RED_PIN, 1);
            ESP_LOGI(TAG, "Current selectedValue %ld", selector.GetCurrentValue());
            break;
        }

        case MoveDown:
        {
            gpio_set_level(LED_YELLOW_PIN, 1);
            ESP_LOGI(TAG, "Current selectedValue %ld", selector.GetCurrentValue());
            break;
        }
        default:
            break;
    }
};