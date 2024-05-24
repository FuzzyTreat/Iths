#ifndef SERVO_H
#define SERVO_H

#include "driver/gpio.h"
#include <driver/ledc.h>

#define MAX_DUTY 0b111111111111
#define DEGREE_0_DUTY 102
#define DEGREE_180_DUTY 512

class Servo
{
    private:
        gpio_num_t pin;
        ledc_channel_t channel;

    public:
        Servo(gpio_num_t pin, ledc_channel_t);
        ~Servo();
        void goToAngle(double angle);
};


#endif
