#include <stdio.h>
#include <iostream>
#include "freertos/FreeRTOS.h"
#include "driver/gpio.h"
#include "esp_log.h"
// #include <Led_Light.h>

#define LED_PIN GPIO_NUM_18
static const char *MAIN_TAG = "MAIN";
// Led_Light *Led1;

/* Allow resolution of undecorated (a.k.a. not mangled), C-style references.  */
extern "C" {
    void app_main(void);
}

void app_main(void) {
    ESP_LOGI(MAIN_TAG, "Create LED on pin %d", LED_PIN);

    // Led1 = new Led_Light(LED_PIN);
    // Led1->blink(1000,500);
    
    // Remove the pointer once it is of any use.
    // delete Led1;
} 