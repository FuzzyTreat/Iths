#include <stdio.h>
#include <iostream>
#include "freertos/FreeRTOS.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include <Led_Light.h>
#include <Button.h>

#define LED_PIN GPIO_NUM_18
#define BUTTON_PIN GPIO_NUM_27

static const char *TAG = "MAIN";

Button *button;

void OnPressed(void* ptr);
void OnReleased(void* ptr);

/* Allow resolution of undecorated (a.k.a. not mangled), C-style references.  */
extern "C" 
{
    void app_main(void);
}

void app_main(void) 
{
    button = new Button(BUTTON_PIN,PT_down);

    button->SetOnPressed(OnPressed, (void *)button);
    button->SetOnRelease(OnReleased, (void *)button);

    for(;;)
    {
        button->Update();
        vTaskDelay(pdMS_TO_TICKS(10));
    }
} 

void OnPressed(void* ptr)
{
    gpio_num_t pin = ((Button *)ptr)->GetPin();
    ESP_LOGI(TAG,"Button pressed on pin %d", pin);
}

void OnReleased(void* ptr)
{
    gpio_num_t pin = ((Button *)ptr)->GetPin();
    ESP_LOGI(TAG,"Button released on pin %d", pin);
}