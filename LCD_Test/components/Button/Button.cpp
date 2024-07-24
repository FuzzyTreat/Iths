#include <stdio.h>
#include <esp_log.h>
#include <esp_timer.h>
#include "Button.h"

static const char *TAG = "BUTTON";

Button::Button(gpio_num_t pin):Button(pin, PT_none)
{
}

Button::Button(gpio_num_t pin, PullType_e pullType):pin(pin), pullType(pullType)
{
    esp_rom_gpio_pad_select_gpio(pin);
    gpio_set_direction(pin, GPIO_MODE_INPUT);

    switch (pullType)
    {
        case PT_up:
            gpio_pullup_en(pin);
            gpio_pulldown_dis(pin);
            break;

        case PT_down:
            gpio_pullup_dis(pin);
            gpio_pulldown_en(pin);
            break;

        default:
            gpio_pullup_dis(pin);
            gpio_pulldown_dis(pin);
            break;
    }

    debounceTimer = esp_timer_get_time();
}

Button::~Button()
{
}

// Läs av pinnen och se om den är hög.
// Se till att pinnen inte har ändrats inom ett kort tidsintervall.
// Intervallet mellan två knapptryck måste vara mer än tex 50 millisekunder  // debounce check
void Button::Update()
{
    int newStatus;
    
    // 50 * 1000 converts seconds to microsekunder.
    if(esp_timer_get_time() - debounceTimer < 50 * 1000)
    {
        return;
    }
    
    newStatus = gpio_get_level(pin);

    if(newStatus != status)
    {
        status = newStatus;
        debounceTimer = esp_timer_get_time();

        if(onRelease && status == 0)
        {
            onRelease(onRelease_ptr);
        }
        else if(onPressed && status == 1)
        {
            onPressed(onPressed_ptr);
        }
    }
}

void Button::SetOnPressed(onButton_cb onPressed, void* ptr)
{
    // ESP_LOGI(TAG,"Button %d was pressed.",pin);
    this->onPressed = onPressed;
    this->onPressed_ptr = ptr;
}

void Button::SetOnRelease(onButton_cb onRelease, void* ptr)
{
    // ESP_LOGI(TAG,"Button %d was released.",pin);
    this->onRelease = onRelease;
    this->onRelease_ptr = ptr;
}

gpio_num_t Button::GetPin()
{
    return pin;
}


