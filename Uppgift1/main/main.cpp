#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include <iostream>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include <esp_timer.h>
#include "esp_system.h"
#include "esp_event.h"

// Högst upp ska 8 PIN definitioner finnas enligt följande:

#define PIN1 GPIO_NUM_23    // Col 1
#define PIN2 GPIO_NUM_22    // Col 2
#define PIN3 GPIO_NUM_21    // Col 3
#define PIN4 GPIO_NUM_19    // Col 4 

#define PIN5 GPIO_NUM_4   // Row 5
#define PIN6 GPIO_NUM_18   // Row 6
#define PIN7 GPIO_NUM_32  // Row 7
#define PIN8 GPIO_NUM_33    // Row 8

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

typedef struct PinArray_s
{
    gpio_num_t *pins;
    uint32_t numPins;
    gpio_num_t activePin;
    gpio_num_t rowPin;
    gpio_num_t colPin;
} PinArray_s;

int interruptCounter = 0;
void InterruptHandler(void *);
void gpioCallback (void* arg);

static QueueHandle_t gpio_evt_queue = NULL;

static void gpio_isr_handler(void* arg)
{
    uint32_t gpio_num = (uint32_t) arg;
    xQueueSendFromISR(gpio_evt_queue, &gpio_num, NULL);
}

static void gpio_task(void* arg)
{
    uint32_t io_num;
    gpio_num_t row_num;
    gpio_num_t col_num;

    for (;;) {
        if (xQueueReceive(gpio_evt_queue, &io_num, portMAX_DELAY)) 
        {
            col_num = (gpio_num_t)io_num;

            for(int i = 0; i < NUM_ROWS; i++)
            {
                if(gpio_get_level(row_pins[i]) == 0)
                {
                    printf("Column pin: %d Col value: %d Row pin: %d Row value: %d\n", col_num, gpio_get_level(col_num), row_pins[i], gpio_get_level(row_pins[i]));
                }
            }

            // printf(" GPIO[%"PRIu32"] intr, val: %d Row pin: %d\n", io_num, gpio_get_level((gpio_num_t)io_num), row_num);
        }
    }
}

PinArray_s *pinArray;

extern "C" void app_main(void)
{
    // pinArray = new PinArray_s();
    // pinArray->pins = row_pins;
    // pinArray->numPins = sizeof(row_pins) / sizeof(row_pins[0]);
    // pinArray->activePin = GPIO_NUM_0;

    // esp_rom_gpio_pad_select_gpio(PIN6);
    // gpio_pullup_dis(PIN6);
    // gpio_pulldown_en(PIN6);
    // gpio_set_direction(PIN6, GPIO_MODE_INPUT);
    // gpio_intr_enable(PIN6);
    // gpio_set_intr_type(PIN6, GPIO_INTR_POSEDGE);

    // esp_rom_gpio_pad_select_gpio(PIN8);
    // gpio_pullup_dis(PIN8);
    // gpio_pulldown_dis(PIN8);
    // gpio_set_direction(PIN8, GPIO_MODE_INPUT);

    // esp_rom_gpio_pad_pullup_only(PIN1);
    gpio_config_t gpioConfig;

    gpioConfig.intr_type = GPIO_INTR_POSEDGE;
    gpioConfig.mode = GPIO_MODE_INPUT;
    gpioConfig.pin_bit_mask = COL_BIT_MASK;
    gpioConfig.pull_down_en = GPIO_PULLDOWN_ENABLE;
    gpioConfig.pull_up_en = GPIO_PULLUP_DISABLE;

    gpio_config(&gpioConfig);

    gpioConfig.intr_type = GPIO_INTR_NEGEDGE;
    gpioConfig.mode = GPIO_MODE_INPUT;
    gpioConfig.pin_bit_mask = ROW_BIT_MASK;
    gpioConfig.pull_down_en = GPIO_PULLDOWN_DISABLE;
    gpioConfig.pull_up_en = GPIO_PULLUP_ENABLE;

    gpio_config(&gpioConfig);

    //gpio_isr_register(void (*fn)(void*), void *arg, int intr_alloc_flags, gpio_isr_handle_t *handle)

    //create a queue to handle gpio event from isr
    gpio_evt_queue = xQueueCreate(10, sizeof(uint32_t));

    //start gpio task, runs a loop which will handle the Queue events when they happen, a way to get out of ISR with data.
    xTaskCreate(gpio_task, "gpio_task", 2048, NULL, 10, NULL);

    //install gpio isr service
    gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);
    //hook isr handler for specific gpio pin
    gpio_isr_handler_add(PIN6, gpio_isr_handler, (void*) PIN6); // Tells you which column the trigger was
    //hook isr handler for specific gpio pin
    gpio_isr_handler_add(PIN5, gpio_isr_handler, (void*) PIN5);

    //remove isr handler for gpio number.
    gpio_isr_handler_remove(PIN6);
    //hook isr handler for specific gpio pin again
    gpio_isr_handler_add(PIN6, gpio_isr_handler, (void*) PIN6);

    printf("Minimum free heap size: %"PRIu32" bytes\n", esp_get_minimum_free_heap_size());

    // gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT); // Installs and register the GPIO handler service
    // gpio_isr_handler_add(PIN4, InterruptHandler, (void *)pinArray); // <- Should send a struct with pin info so we can parse the rows.

    // gpio_set_direction(PIN1, GPIO_MODE_INPUT);
    // gpio_set_pull_mode(PIN1, GPIO_PULLUP_ONLY);

    // gpio_set_direction(PIN5, GPIO_MODE_INPUT);
    // gpio_set_level(PIN1, 1);

    gpio_dump_io_configuration(stdout, 1ULL << PIN6);
    gpio_dump_io_configuration(stdout, 1ULL << PIN4);

    int level = 0;

    for(;;)
    {
        // gpio_set_level(GPIO_NUM_27, 0);

        // ESP_LOGI(TAG,"PIN1 level: %d PIN5 level: %d", gpio_get_level(PIN1), gpio_get_level(PIN5));
        // printf("\rPIN1 level: %d Active pin: %d ", gpio_get_level(PIN4), pinArray->activePin);
        
        vTaskDelay(pdMS_TO_TICKS(50));
        // gpio_set_level(PIN4, 0);
        // gpio_set_level(PIN5, 0);
    }
}




/// @brief User defined interrupt handler method
/// @param counter 
void InterruptHandler(void *pins)
{
    // PinArray_s *pinArray = (PinArray_s *)pins;

    // for(int i=0; i < pinArray->numPins; i++)
    // {
    //     if(i >= pinArray->numPins)
    //     {
    //         i = pinArray->numPins - 1;
    //     }

    //     gpio_num_t currentPin = *(pinArray->pins + i);

    //     if(gpio_get_level(currentPin) == 0)
    //     {
    //         pinArray->activePin = currentPin;
    //     }
    // }

    // gpio_isr_handler_remove(ROW_5_PIN);
    // *((int *)counter) = *((int *)counter) + 1;
    
}

void gpioCallback(void *args)
{
    
}

