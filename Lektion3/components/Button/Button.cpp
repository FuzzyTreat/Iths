#include <stdio.h>
#include "esp_log.h"
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
}

Button::~Button()
{
}

// Läs av pinnen och se om den är hög.
// Se till att pinnen inte har ändrats inom ett kort tidsintervall  // debounce check
void Button::update()
{
    int newStatus = gpio_get_level(pin);

    if(newStatus != status)
    {
        status = newStatus;
        ESP_LOGI(TAG,"New status %d", status);
    }
}


