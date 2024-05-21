#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "driver/gpio.h"
#include "esp_log.h"

static const char *BUTTON_TAG = "BUTTON";

typedef enum PullType_e
{
    
} PullType_e;

class Button
{
    private:
        gpio_num_t pin;
        PullType_e pullType;

    public:
        Button(gpio_num_t pin);
        ~Button();

        void Update();
};