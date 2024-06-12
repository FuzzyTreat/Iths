#include <stdio.h>
#include "freertos\FreeRTOS.h"
#include "driver\gpio.h"
#include "esp_log.h"
#include "Button.h"

#define UP_BUTTON_PIN GPIO_NUM_18
#define DOWN_BUTTON_PIN GPIO_NUM_19

#define LED_RED_PIN GPIO_NUM_25
#define LED_YELLOW_PIN GPIO_NUM_26

const char *TAG = "Main";

Button *upButton;
Button *downButton;

void OnButtonPressed(void *ptr);
void OnButtonReleased(void *ptr);
void RegisterButtons();

gpio_num_t prevButton;

extern "C" void app_main(void)
{
    gpio_pulldown_en(LED_RED_PIN);
    gpio_pulldown_en(LED_YELLOW_PIN);

    gpio_pullup_dis(LED_RED_PIN);
    gpio_pullup_dis(LED_YELLOW_PIN);

    gpio_set_level(LED_RED_PIN, 0);
    gpio_set_level(LED_YELLOW_PIN, 0);

    gpio_set_direction(LED_RED_PIN, GPIO_MODE_OUTPUT);
    gpio_set_direction(LED_YELLOW_PIN, GPIO_MODE_OUTPUT);

    RegisterButtons();

    while(true)
    {
        vTaskDelay(pdMS_TO_TICKS(500));
        gpio_set_level(LED_RED_PIN, 0);
        gpio_set_level(LED_YELLOW_PIN, 0);

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
    prevButton = pin;

    switch (pin)
    {
    case UP_BUTTON_PIN:
        {
            gpio_set_level(LED_RED_PIN, 1);
            break;
        }
    case DOWN_BUTTON_PIN:
        {
            gpio_set_level(LED_YELLOW_PIN, 1);
            break;
        }
    default:
        break;
    }

    
    ESP_LOGI(TAG,"Button pressed on pin %d", pin);
}

void OnButtonReleased(void *ptr)
{
    gpio_num_t pin = ((Button *)ptr)->GetPin();
    ESP_LOGI(TAG,"Button released on pin %d", pin);
}