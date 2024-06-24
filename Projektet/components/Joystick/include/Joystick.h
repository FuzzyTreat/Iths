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

    typedef void (*onJoystick_cb)(void *);

    private:
        adc1_channel_t xPin;
        adc1_channel_t yPin;
        Position_s position = {0,0};
        int xStatus;
        int yStatus;

        onJoystick_cb onXPressed = nullptr;
        onJoystick_cb onXRelease = nullptr;

        void* onXPressed_ptr = nullptr;
        void* onXRelease_ptr = nullptr;

        onJoystick_cb onYPressed = nullptr;
        onJoystick_cb onYRelease = nullptr;

        void* onYPressed_ptr = nullptr;
        void* onYRelease_ptr = nullptr;
       
    public:
        Joystick(gpio_num_t buttonPin, adc1_channel_t xPin,adc1_channel_t yPin);
        ~Joystick();
        void SetOnXPressed(onJoystick_cb onXPressed, void *ptr);
        void SetOnXRelease(onJoystick_cb OnXRelease, void *ptr);
        void SetOnYPressed(onJoystick_cb onYPressed, void *ptr);
        void SetOnYRelease(onJoystick_cb OnYRelease, void *ptr);
        void Update();
        Position_s GetPosition();
};


#endif
