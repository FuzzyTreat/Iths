#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "driver/gpio.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_event_loop.h"
//#include "nvs_flash.h"


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


// Högst upp ska 8 PIN definitioner finnas enligt följande:

#define PIN1 GPIO_NUM_27    // Row 1
#define PIN2 GPIO_NUM_26    // Row 2
#define PIN3 GPIO_NUM_25    // Row 3
#define PIN4 GPIO_NUM_19    // Row 4 
#define PIN5 GPIO_NUM_23    // Col 5
#define PIN6 GPIO_NUM_22    // Col 6
#define PIN7 GPIO_NUM_21    // Col 7
#define PIN8 GPIO_NUM_18    // Col 8

// Keypad Pins
#define NUM_ROWS 4
#define NUM_COLS 4

static gpio_num_t row_pins[] = {PIN1, PIN2, PIN3, PIN4};
static gpio_num_t col_pins[] = {PIN5, PIN6, PIN7, PIN8};

 // Struct with the neccessary settings for the keypad.
typedef struct keypad_settings {
        uint32_t      num_row_layout;   // Number of rows of the layout
        uint32_t      num_col_layout;   // Number of columns of the layout
        uint32_t      layout[4][4];     // Keypad layout in 2 dimensions
        uint32_t      num_gpio_out;     // Number of output GPIOs
        uint32_t      num_gpio_in;      // Number of input GPIOs
        gpio_num_t    *gpio_output;     // Pointer for the output GPIOs array
        gpio_num_t    *gpio_input;      // Pointer for the input GPIOs array
        uint32_t      frequency;        // Scan frequency in Hz
        uint32_t      last_output;      // Last order in output dimension
        uint32_t      last_input;       // Last order in input dimension
        QueueHandle_t queue;            // Queue to send this structure
} keypad_settings_t;

keypad_settings_t keypad;

void setGpioConfig();
void setupGpioDirection();
uint64_t getPinMask(const gpio_num_t *pins_array, const uint32_t num_pins);

extern "C" void app_main(void)
{
    char keyMap[NUM_ROWS][NUM_COLS] = {
        {'1','2','3', 'A'},
        {'4','5','6', 'B'},
        {'7','8','9', 'C'},
        {'*','0','#', 'D'}
    };
}

void setGpioConfig(keypad_settings_t *keypad)
{
    gpio_config_t io_conf;

    //disable interrupt
    io_conf.intr_type = GPIO_INTR_DISABLE;
    //set as output mode
    io_conf.mode = GPIO_MODE_OUTPUT;
    //bit mask of the pins that you want to set,e.g.GPIO18/19
    io_conf.pin_bit_mask = getPinMask(keypad->gpio_output, keypad->num_gpio_out);
    //disable pull-down mode
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    //disable pull-up mode
    io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
    //configure GPIO with the given settings
    gpio_config(&io_conf);

    //interrupt of rising edge
    io_conf.intr_type = GPIO_INTR_DISABLE;
    //set as input mode
    io_conf.mode = GPIO_MODE_INPUT;
    //bit mask of the pins, use GPIO4/5 here
    io_conf.pin_bit_mask = getPinMask(keypad->gpio_input, keypad->num_gpio_in);

    //disable pull-down mode
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    //enable pull-up mode
    io_conf.pull_up_en = GPIO_PULLUP_ENABLE; // Bad internal pull-up
    
    //configure GPIO with the given settings
    gpio_config(&io_conf);

    //create a queue to handle gpio event from isr
    // keypad->queue = xQueueCreate(10, sizeof(keypad_settings_t));
    //start gpio task
    // xTaskCreate(keypad_sequency_task, "keypad_sequency_task", 2048, (void *) keypad, 10, NULL);
}

void setupGpioDirection()
{
    // Column pins
    esp_rom_gpio_pad_select_gpio(PIN4);
    gpio_set_direction(PIN4, GPIO_MODE_INPUT);

    esp_rom_gpio_pad_select_gpio(PIN3);
    gpio_set_direction(PIN3, GPIO_MODE_INPUT);    

    esp_rom_gpio_pad_select_gpio(PIN2);
    gpio_set_direction(PIN2, GPIO_MODE_INPUT);

    esp_rom_gpio_pad_select_gpio(PIN1);
    gpio_set_direction(PIN1, GPIO_MODE_INPUT);   

    // Row pins
    esp_rom_gpio_pad_select_gpio(PIN5);
    gpio_set_direction(PIN5, GPIO_MODE_OUTPUT);

    esp_rom_gpio_pad_select_gpio(PIN6);
    gpio_set_direction(PIN6, GPIO_MODE_OUTPUT);    

    esp_rom_gpio_pad_select_gpio(PIN7);
    gpio_set_direction(PIN7, GPIO_MODE_OUTPUT);

    esp_rom_gpio_pad_select_gpio(PIN8);
    gpio_set_direction(PIN8, GPIO_MODE_OUTPUT);   
}

uint64_t getPinMask(const gpio_num_t *pins_array, const uint32_t num_pins)
{
    uint32_t buf32_0 = 0;
    uint32_t buf32_1 = 0;
    uint64_t result = 0;
    for (uint32_t i = 0; i < num_pins; i++) {
        // result |= 1 << pins_array[i];
        if (pins_array[i] >= 32)
            buf32_1 |= 1 << (pins_array[i] - 32);
        else
            buf32_0 |= 1 << pins_array[i];
        
    }

    result = ((uint64_t)buf32_1 << 32) | ((uint64_t)buf32_0 << 0);

    return result;
}