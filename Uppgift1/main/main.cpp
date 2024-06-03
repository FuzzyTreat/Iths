#include <stdio.h>
#include <iostream>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include <esp_timer.h>

// Pin placement on the keypad connector
// 8 ------- 1

// Keypad grid layout

// 8   X X X X
// 7   X X X X
// 6   X X X X
// 5   X X X X

//     4 3 2 1

#define PIN1 GPIO_NUM_4     // Col 1    PULLDOWN SW
#define PIN2 GPIO_NUM_18    // Col 2    PULLDOWN SW
#define PIN3 GPIO_NUM_32    // Col 3    PULLDOWN SW
#define PIN4 GPIO_NUM_33    // Col 4    PULLDOWN SW

#define PIN8 GPIO_NUM_19    // Row 5    PULLUP SW
#define PIN7 GPIO_NUM_21    // Row 6    PULLUP SW
#define PIN6 GPIO_NUM_22    // Row 7    PULLUP SW
#define PIN5 GPIO_NUM_23    // Row 8    PULLUP SW

static const char *TAG = "Keypad";

#define COL_BIT_MASK ((1ULL << PIN1) | (1ULL << PIN2) | (1ULL << PIN3) | (1ULL << PIN4))
#define ROW_BIT_MASK ((1ULL << PIN5) | (1ULL << PIN6) | (1ULL << PIN7) | (1ULL << PIN8))

// Keypad Pins
#define NUM_ROWS 4
#define NUM_COLS 4

#define ESP_INTR_FLAG_DEFAULT 0

static QueueHandle_t gpio_evt_queue = NULL;
static void gpio_isr_handler(void* arg);
static void gpio_task(void* arg);
char32_t ReadKeyChar(uint32_t row_num, gpio_num_t col_pin);

void configureGpioPins();

static gpio_num_t col_pins[] = {PIN4, PIN3, PIN2, PIN1};
static gpio_num_t row_pins[] = {PIN5, PIN6, PIN7, PIN8};

char32_t keyMap[NUM_ROWS][NUM_COLS] = {
    {'1','2','3', 'A'},
    {'4','5','6', 'B'},
    {'7','8','9', 'C'},
    {'*','0','#', 'D'}
};

uint64_t debounceTimer = 0;

extern "C" void app_main(void)
{
    debounceTimer = esp_timer_get_time();

    configureGpioPins();

    //create a queue to handle gpio event from isr
    gpio_evt_queue = xQueueCreate(10, sizeof(uint32_t));

    //start gpio task, runs a loop which will handle the Queue events when they happen, a way to get out of ISR with data.
    xTaskCreate(gpio_task, "gpio_task", 2048, NULL, 10, NULL);

    //install gpio isr service
    gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);

    for(int i = 0; i< NUM_COLS; i++)
    {
        // not really sure why this is done this way, could be the same slowness which the lcd had.
        //hook isr handler for specific gpio pin
        gpio_isr_handler_add(col_pins[i], gpio_isr_handler, (void*) col_pins[i]); // Tells you which column the trigger was
        //remove isr handler for gpio number.
        gpio_isr_handler_remove(col_pins[i]);
        //hook isr handler for specific gpio pin again
        gpio_isr_handler_add(col_pins[i], gpio_isr_handler, (void*) col_pins[i]);
    }

    for(;;)
    {
        // Main loop, contains a delay just to stop the process from being a run away.
        vTaskDelay(pdMS_TO_TICKS(25));
    }
}

/// @brief Configures the required pins
void configureGpioPins()
{
    gpio_config_t gpioConfig;

    gpioConfig.intr_type = GPIO_INTR_POSEDGE;
    gpioConfig.mode = GPIO_MODE_INPUT;
    gpioConfig.pin_bit_mask = COL_BIT_MASK;
    gpioConfig.pull_down_en = GPIO_PULLDOWN_ENABLE;
    gpioConfig.pull_up_en = GPIO_PULLUP_DISABLE;

    gpio_config(&gpioConfig);

    gpioConfig.intr_type = GPIO_INTR_DISABLE;
    gpioConfig.mode = GPIO_MODE_INPUT;
    gpioConfig.pin_bit_mask = ROW_BIT_MASK;
    gpioConfig.pull_down_en = GPIO_PULLDOWN_DISABLE;
    gpioConfig.pull_up_en = GPIO_PULLUP_ENABLE;

    gpio_config(&gpioConfig);
}

static void gpio_isr_handler(void* arg)
{
    uint32_t gpio_num = (uint32_t) arg;
    // Register the handler in order to switch context once an event has been triggered.
    // Things like ESP_LOGI can be used once the context no longer is ISR 
    xQueueSendFromISR(gpio_evt_queue, &gpio_num, NULL);
}

static void gpio_task(void* arg)
{
    uint32_t io_num;
    gpio_num_t col_num;

    for (;;) {
        if (xQueueReceive(gpio_evt_queue, &io_num, pdMS_TO_TICKS(10))) // portMAX_DELAY will block indefinetly if  INCLUDEvTaskSuspend  is set to 1, otherwise has a block time of 0xffffffff 
        {
            // 50 * 1000 converts seconds to milliseconds.
            if(esp_timer_get_time() - debounceTimer < 50 * 1000)
            {
                return;
            }

            col_num = (gpio_num_t)io_num;

            for(int i = 0; i < NUM_ROWS; i++)
            {
                if(gpio_get_level(row_pins[i]) == 0)
                {
                    // Call method for extracting the letter from the pre prepared matrix
                    char32_t key = ReadKeyChar(i, col_num);

                    // Send the key press to the terminal
                    ESP_LOGI(TAG,"%c", key);
                    break;
                }
            }
        }
    }
}

/// @brief Reads the keypad char in the position provided by row and col parameters
/// @param row_num 
/// @param col_pin 
/// @return Returns a single char32_t value or throws and error
char32_t ReadKeyChar(uint32_t row_num, gpio_num_t col_pin)
{
    uint32_t col_num = 0;

    for(int i = 0; i < NUM_COLS; i++)
    {
        if(col_pins[i] == col_pin)
        {
            col_num = i;
            break;
        }
    }

    return keyMap[row_num][col_num];
}

