#include <stdio.h>
#include <iostream>
#include "freertos/FreeRTOS.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include <esp_timer.h>
#include <esp_task_wdt.h>
#include <driver/dedic_gpio.h>

#define 


#define NUM_ROWS  4
#define NUM_COLS  4

const int rowPins[NUM_ROWS] = {14, 27, 26, 25}; // GPIO_NUM_14, GPIO_NUM_27, GPIO_NUM_26, GPIO_NUM_25
const int colPins[NUM_COLS] = {33, 32, 18, 19}; // GPIO_NUM_33, GPIO_NUM_32, GPIO_NUM_18, GPIO_NUM_19

static const char *TAG = "MAIN";

/* Allow resolution of undecorated (a.k.a. not mangled), C-style references.  */
extern "C" void app_main(void) 
{
    char keyMap[NUM_ROWS][NUM_COLS] = {
        {'1','2','3', 'A'},
        {'4','5','6', 'B'},
        {'7','8','9', 'C'},
        {'*','0','#', 'D'}
    };

    // Configure the pin bundle for the matrix rows, set pins to high
    gpio_config_t rowConfig = {
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
    };

    for(int32_t i = 0; i < sizeof(rowPins) / sizeof(rowPins[0]); i++)
    {
        rowConfig.pin_bit_mask = 1ULL << rowPins[i];
        gpio_config(&rowConfig);
    }

    // create the row pin bundle
    dedic_gpio_bundle_handle_t rowBundle = NULL;
    dedic_gpio_bundle_config_t rowBundleConfig = {
        .gpio_array = rowPins,
        .array_size = sizeof(rowPins) / sizeof(rowPins[0]),
        .flags = {
            .out_en = 1,
        }
    };

    ESP_ERROR_CHECK(dedic_gpio_new_bundle(&rowBundleConfig, &rowBundle));

    // Disable interrupt
    // dedic_gpio_bundle_set_interrupt_and_callback(mkbd->row_bundle, (1 << config->nr_row_gpios) - 1,
    //         DEDIC_GPIO_INTR_NONE, NULL, NULL);

    for(;;)
    {
        // ESP_LOGI(TAG,"Nothing to see here yet!");
    }
} 