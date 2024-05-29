#ifndef JOYSTICK_H
#define JOYSTICK_H

#include "driver/gpio.h"
#include "driver/adc.h"
#include "Button.h"

class Joystick : public Button
{
    typedef struct Position_s
    {
        int x;
        int y;
    } Position_s;

    private:
        adc1_channel_t xPin;
        adc1_channel_t yPin;
        Position_s position = {0,0};
       
    public:
        Joystick(gpio_num_t buttonPin, adc1_channel_t xPin,adc1_channel_t yPin);
        ~Joystick();
        void Update();
        Position_s GetPosition();
};


#endif
