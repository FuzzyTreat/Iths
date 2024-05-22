#ifndef BUTTON_H
#define BUTTON_H

#include "driver/gpio.h"

typedef enum PullType_e
{   
    PT_none,
    PT_up,
    PT_down
} PullType_e;

class Button
{
    private:
        gpio_num_t pin;
        PullType_e pullType;
        int status;
        int64_t debounceTimer;
        
    public:
        Button(gpio_num_t pin);
        Button(gpio_num_t pin, PullType_e pullType);
        ~Button();
        void update();
};

#endif