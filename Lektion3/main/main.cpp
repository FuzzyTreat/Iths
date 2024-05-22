#include <stdio.h>
#include <iostream>
#include "freertos/FreeRTOS.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include <Led_Light.h>

#define LED_PIN GPIO_NUM_18
#define BUTTON_PIN GPIO_NUM_27

static const char *TAG = "MAIN";

// Led_Light *Led1;

/* Allow resolution of undecorated (a.k.a. not mangled), C-style references.  */
extern "C" {
    void app_main(void);
}

void app_main(void) {
    
    esp_rom_gpio_pad_select_gpio(BUTTON_PIN);
    gpio_set_direction(BUTTON_PIN, GPIO_MODE_INPUT);

    gpio_pullup_dis(BUTTON_PIN);
    gpio_pulldown_en(BUTTON_PIN);

    for(size_t i = 0; i < 10 * 50; i++)
    {
        vTaskDelay(pdMS_TO_TICKS(20));
    }

    // Led1 = new Led_Light(LED_PIN);
    // Led1->blink(1000,500);
    
    // Remove the pointer once it is of any use.
    // delete Led1;
} 