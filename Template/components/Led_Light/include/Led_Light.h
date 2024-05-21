#ifndef LED_H
#define LED_H

#include "driver/gpio.h"

class Led_Light
{
    private:
        gpio_num_t pin;

    public:
    Led_Light(gpio_num_t pin);
    ~Led_Light();

    void On();
    void Off();

    void blink();
    void blink(int timeOn, int timeOff);
};

#endif