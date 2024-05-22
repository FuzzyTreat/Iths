#ifndef BUTTON_H
#define BUTTON_H

#include "driver/gpio.h"

class Button
{
    private:
        gpio_num_t pin;

    public:
        Button(gpio_num_t pin);
        ~Button();
};

#endif