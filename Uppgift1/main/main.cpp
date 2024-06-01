#include <stdio.h>
#include <iostream>
#include "freertos/FreeRTOS.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include <esp_timer.h>
#include "esp_system.h"
#include "esp_event.h"

// Högst upp ska 8 PIN definitioner finnas enligt följande:

#define PIN1 GPIO_NUM_27    // Row 1
#define PIN2 GPIO_NUM_26    // Row 2
#define PIN3 GPIO_NUM_25    // Row 3
#define PIN4 GPIO_NUM_19    // Row 4 

#define PIN5 GPIO_NUM_23    // Col 5
#define PIN6 GPIO_NUM_22    // Col 6
#define PIN7 GPIO_NUM_21    // Col 7
#define PIN8 GPIO_NUM_18    // Col 8

static const char *TAG = "MAIN";

#define ROW_BIT_MASK ((1ULL << PIN1) | (1ULL << PIN2) | (1ULL << PIN3) | (1ULL << PIN4))
#define COL_BIT_MASK ((1ULL << PIN5) | (1ULL << PIN6) | (1ULL << PIN7) | (1ULL << PIN8))

// Keypad Pins
#define NUM_ROWS 4
#define NUM_COLS 4

#define ESP_INTR_FLAG_DEFAULT 0

static gpio_num_t row_pins[] = {PIN1, PIN2, PIN3, PIN4};
static gpio_num_t col_pins[] = {PIN5, PIN6, PIN7, PIN8};

// Uppgift
// Implementera en kod som använder sig av GPIO för att ställa och läsa av värden från Keypad matrix.
// Koden ska skriva ut i konsolen med hjälp av ESP_LOGI vilken knapp som har trycks ner.
// "A" eller "1" till exempel.

// Pinnarna ska vara definerade enligt bilden nedan, där pinne 1 är till höger i bilden och 8 till vänster.
// Ni ska också förklara om ni har använt pull up/ pull down extern eller i mjukvara på respektive pinne. Skriv den informationen i en fil som heter README.md i top nivå på erat projekt.
// Zippa hela projektet och ladda upp.
// Mattias kommer ta bort sdkconfig så se till att lägga alla inställningar som krävs i sdkconfig.defaults

// Pin placement on the keypad connector

// 8 ------- 1

// Keypad grid layout

// 8   X X X X
// 7   X X X X
// 6   X X X X
// 5   X X X X

//     4 3 2 1

int interruptCounter = 0;
void InterruptHandler(void *);

extern "C" void app_main(void)
{
    gpio_config_t gpioConfig;

    // Column config
    gpioConfig.intr_type = GPIO_INTR_POSEDGE;
    gpioConfig.mode = GPIO_MODE_INPUT;
    gpioConfig.pin_bit_mask = 1ULL << GPIO_NUM_23;
    gpioConfig.pull_down_en = GPIO_PULLDOWN_DISABLE;
    gpioConfig.pull_up_en = GPIO_PULLUP_DISABLE;

    gpio_config(&gpioConfig);

    // Row config
    gpioConfig.intr_type = GPIO_INTR_DISABLE;
    gpioConfig.mode = GPIO_MODE_OUTPUT;
    gpioConfig.pin_bit_mask = 1ULL << GPIO_NUM_27;
    gpioConfig.pull_down_en = GPIO_PULLDOWN_DISABLE;
    gpioConfig.pull_up_en = GPIO_PULLUP_ENABLE;

    gpio_config(&gpioConfig);

    // gpio_isr_register(void (*fn)(void*), void *arg, int intr_alloc_flags, gpio_isr_handle_t *handle)

    // gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT); // Installs and register the GPIO handler service
    // gpio_isr_handler_add(PIN5, InterruptHandler, (void *)&interruptCounter); // <- Should send a struct with pin info so we can parse the rows.

    // gpio_set_direction(PIN1, GPIO_MODE_INPUT);
    // gpio_set_pull_mode(PIN1, GPIO_PULLUP_ONLY);

    // gpio_set_direction(PIN5, GPIO_MODE_INPUT);
    // gpio_set_level(PIN1, 1);

    int level = 0;

    // for(;;)
    // {
    //     // ESP_LOGI(TAG,"PIN1 level: %d PIN5 level: %d", gpio_get_level(PIN1), gpio_get_level(PIN5));
    //     printf("\rPIN1 level: %d PIN5 level: %d Interrupts: %d", gpio_get_level(PIN1), gpio_get_level(PIN5), interruptCounter);

    //     vTaskDelay(pdMS_TO_TICKS(50));
    // }
}

/// @brief User defined interrupt handler method
/// @param counter 
void InterruptHandler(void *counter)
{
    // gpio_isr_handler_remove(ROW_5_PIN);
    *((int *)counter) = *((int *)counter) + 1;
    
}

