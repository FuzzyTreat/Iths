#include <stdio.h>
#include <iostream>
#include "freertos/FreeRTOS.h"
#include "driver/gpio.h"
#include "esp_log.h"

#define LED_PIN GPIO_NUM_18
static const char *MAIN_TAG = "MAIN";

/* Allow resolution of undecorated (a.k.a. not mangled), C-style references.  */
extern "C" {
    void app_main(void);
}

void app_main(void) {
    ESP_LOGI(MAIN_TAG, "Create LED on pin %d", LED_PIN);
} 