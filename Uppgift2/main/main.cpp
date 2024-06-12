#include <stdio.h>
#include "freertos\FreeRTOS.h"
#include "driver\gpio.h"
#include "esp_log.h"
#include "Button.h"

#define UP_BUTTON_PIN GPIO_NUM_18
#define DOWN_BUTTON_PIN GPIO_NUM_19

const char *TAG = "Main";

Button *upButton;
Button *downButton;

void OnButtonPressed(void *ptr);
void OnButtonReleased(void *ptr);
void RegisterButtons();

extern "C" void app_main(void)
{
    gpio_pulldown_en(GPIO_NUM_12);
    gpio_pulldown_en(GPIO_NUM_14);

    gpio_pullup_dis(GPIO_NUM_12);
    gpio_pullup_dis(GPIO_NUM_14);

    gpio_set_level(GPIO_NUM_12, 0);
    gpio_set_level(GPIO_NUM_14, 0);

    gpio_set_direction(GPIO_NUM_12, GPIO_MODE_OUTPUT);
    gpio_set_direction(GPIO_NUM_14, GPIO_MODE_OUTPUT);

    RegisterButtons();

    while(true)
    {
        vTaskDelay(pdMS_TO_TICKS(10));

        gpio_set_level(GPIO_NUM_12, 0);
        gpio_set_level(GPIO_NUM_14, 0);

        upButton->Update();
        downButton->Update();
    }
}

void RegisterButtons()
{
    upButton = new Button(UP_BUTTON_PIN, PT_down);
    upButton->SetOnPressed(OnButtonPressed, (void *)upButton);
    upButton->SetOnRelease(OnButtonReleased, (void *)upButton);

    downButton = new Button(DOWN_BUTTON_PIN, PT_down);
    downButton->SetOnPressed(OnButtonPressed, (void *)downButton);
    downButton->SetOnRelease(OnButtonReleased, (void *)downButton);
}

void OnButtonPressed(void *ptr)
{
    gpio_num_t pin = ((Button *)ptr)->GetPin();
    ESP_LOGI(TAG,"Button pressed on pin %d", pin);

    switch (pin)
    {
        case GPIO_NUM_18:
        {
            gpio_set_level(GPIO_NUM_12, 1);
            gpio_set_level(GPIO_NUM_14, 0);
            break;
        }
        case GPIO_NUM_19:
        {
            gpio_set_level(GPIO_NUM_12, 0);
            gpio_set_level(GPIO_NUM_14, 1);
            break;
        }   
        default:
        {
            gpio_set_level(GPIO_NUM_12, 0);
            gpio_set_level(GPIO_NUM_14, 0);
            break;
        }
    }
}

void OnButtonReleased(void *ptr)
{
    gpio_num_t pin = ((Button *)ptr)->GetPin();
    ESP_LOGI(TAG,"Button released on pin %d", pin);
}