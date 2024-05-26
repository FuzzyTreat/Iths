#ifndef SERVO_H
#define SERVO_H

#include "driver/gpio.h"
#include <driver/ledc.h>

#define MAX_DUTY 0b111111111111
#define DEGREE_0_DUTY 102
#define DEGREE_180_DUTY 512

// 0.5 ms = 0 grader
// 2.5 ms = 180 grader
// 50 Hz
// 20 ms tidsperiod
// 0.5 / 20 = 0.025 // 2.5%
// 2.5 / 20 = 0.125 // 12.5%

// duty på 2.5% = 0 grader
// duty på 12.5% = 180 grader

// 12  bit = 4095
// 2.5% 4095 ca 102.375
// 12.5% 4095 ca 511.875

class Servo
{
    private:
        gpio_num_t pin;
        ledc_channel_t channel;
        bool direction = false;
        int32_t calculateDuty(double degrees);

    public:
        Servo(gpio_num_t pin, ledc_channel_t);
        ~Servo();
        void Update(double angle);
        double CalculateAngle(double angle);
};

#endif
