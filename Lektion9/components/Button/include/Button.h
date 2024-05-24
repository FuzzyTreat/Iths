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
    typedef void (*onButton_cb)(void *);

    private:
        int64_t debounceTimer;
        onButton_cb onPressed = nullptr;
        onButton_cb onRelease = nullptr;

        void* onPressed_ptr = nullptr;
        void* onRelease_ptr = nullptr;

    protected:
        gpio_num_t pin;
        PullType_e pullType;
        int status;
    public:
        Button(gpio_num_t pin);
        Button(gpio_num_t pin, PullType_e pullType);
        ~Button();
        void Update();
        void SetOnPressed(onButton_cb onPressed, void *ptr);
        void SetOnRelease(onButton_cb OnRelease, void *ptr);
        gpio_num_t GetPin();
};

#endif