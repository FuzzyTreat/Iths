#include <stdio.h>
#include <iostream>
#include "freertos/FreeRTOS.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include <esp_timer.h>
#include <esp_task_wdt.h>

static const char *TAG = "MAIN";

/* Allow resolution of undecorated (a.k.a. not mangled), C-style references.  */
extern "C" void app_main(void) 
{
    int counter = 0;
    int64_t timer = esp_timer_get_time();
    int64_t currentTime = 0;

    // assumes the watchdog in config has been turned off
    /// define and start watchdog
    esp_task_wdt_config_t watchDogConfig;

    watchDogConfig.timeout_ms = 5000;
    watchDogConfig.trigger_panic = false; // true -> reboot board
    watchDogConfig.idle_core_mask = 0b10; // watchdog Idle0 
    
    esp_task_wdt_init(&watchDogConfig);
    esp_task_wdt_add(NULL); // NULL is the context where the watchdog is called from, in this case main()

    for(;;)
    {
        currentTime = esp_timer_get_time();

        // vTaskDelay(pdMS_TO_TICKS(500));
        printf("\rTime %ld      Counter %d       ", currentTime / 1000,  counter++);

        if((currentTime - timer)  > 4500 * 1000)
        {
            timer = currentTime;

            esp_task_wdt_reset(); // Wake the dog, pet it to make it aware
        }
    }
} 

/// @brief User defined Interrupt handler method
/// @param  
extern "C" void esp_task_wdt_isr_user_handler(void)
{

}