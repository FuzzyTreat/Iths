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

/* Allow resolution of undecorated (a.k.a. not mangled), C-style references.  */
extern "C" 
{
    void app_main(void);
}

void app_main(void) 
{
    button = new Button(BUTTON_PIN,PT_down);
    
    for(;;)
    {
        button->update();
        vTaskDelay(pdMS_TO_TICKS(10));
    }
} 