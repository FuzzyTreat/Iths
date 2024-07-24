#include <stdio.h>
#include <iostream>
#include "freertos/FreeRTOS.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include <esp_timer.h>
#include <esp_task_wdt.h>

#define INTERRUPT_PIN GPIO_NUM_27

static const char *TAG = "MAIN";

int interruptCounter = 0;
void InterruptHandler(void *);

/* Allow resolution of undecorated (a.k.a. not mangled), C-style references.  */
extern "C" void app_main(void) 
{
    int counter = 0;
    int64_t timer = esp_timer_get_time();
    int64_t currentTime = 0;

    // assumes the watchdog in config has been turned off
    /// define and start watchdog
    esp_task_wdt_config_t watchDogConfig;
    gpio_config_t interruptConfig;

    watchDogConfig.timeout_ms = 5000;
    watchDogConfig.trigger_panic = false; // true -> reboot board
    watchDogConfig.idle_core_mask = 0b10; // watchdog Idle0 
    
    esp_task_wdt_init(&watchDogConfig);
    esp_task_wdt_add(NULL); // NULL is the context where the watchdog is called from, in this case main()

    interruptConfig.pin_bit_mask = 1ULL << INTERRUPT_PIN; // 1 usigned long long bitshift INTERRUPT_PIN
    interruptConfig.mode = GPIO_MODE_INPUT;
    interruptConfig.intr_type = GPIO_INTR_POSEDGE; // when is the interrupt triggered, high or low on the pin
    interruptConfig.pull_down_en = GPIO_PULLDOWN_DISABLE;
    interruptConfig.pull_up_en = GPIO_PULLUP_DISABLE;

    gpio_config(&interruptConfig);

    // gpio_isr_register(void (*fn)(void*), void *arg, int intr_alloc_flags, gpio_isr_handle_t *handle)

    gpio_install_isr_service(0); // Installs and register the GPIO handler service
    gpio_isr_handler_add(INTERRUPT_PIN, InterruptHandler, (void *)&interruptCounter);

    for(;;)
    {
        currentTime = esp_timer_get_time();

        if((currentTime - timer)  > 4500 * 1000)
        {
            timer = currentTime;
            esp_task_wdt_reset(); // Wake the dog, pet it to make it aware
            ESP_LOGI(TAG,"Watchdog reset, interrupt count %d", counter);
        }
    }
} 

/// @brief User defined interrupt handler method
/// @param counter 
void InterruptHandler(void *counter)
{
    gpio_isr_handler_remove(INTERRUPT_PIN);
    *((int *)counter) = *((int *)counter) + 1;
}


/// @brief User defined watchdog handler method
/// @param  
extern "C" void esp_task_wdt_isr_user_handler(void)
{

}