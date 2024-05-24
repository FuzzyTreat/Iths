#include <stdio.h>
#include <iostream>
#include "freertos/FreeRTOS.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "driver/adc.h"

#include <Led_Light.h>
#include <Button.h>
#include <Joystick.h>

#define LED_PIN GPIO_NUM_18
#define BUTTON_PIN GPIO_NUM_27
#define JOYSTICK_BUTTON_PIN GPIO_NUM_26
#define ADC_CHANNEL_1 GPIO_NUM_32
#define ADC_CHANNEL_2 GPIO_NUM_33

static const char *TAG = "MAIN";

Button *button;
Joystick *joystick;

void OnPressed(void* ptr);
void OnReleased(void* ptr);

/* Allow resolution of undecorated (a.k.a. not mangled), C-style references.  */
extern "C" void app_main(void) 
{
    int counter = 0;

    button = new Button(BUTTON_PIN,PT_down);
    button->SetOnPressed(OnPressed, (void *)button);
    button->SetOnRelease(OnReleased, (void *)button);

    joystick = new Joystick(JOYSTICK_BUTTON_PIN, ADC1_CHANNEL_4, ADC1_CHANNEL_5);
    joystick->SetOnPressed(OnPressed, (void *)joystick);
    joystick->SetOnRelease(OnReleased, (void *)joystick);

    for(;;)
    {
        if(counter++ >= 100)
        {
            counter = 0;
            ESP_LOGI(TAG,"Position x: %d y: %d", joystick->GetPosition().x, joystick->GetPosition().y);
        }

        button->Update();
        joystick->Update();

        vTaskDelay(pdMS_TO_TICKS(15));
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