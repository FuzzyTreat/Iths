#include <stdio.h>
#include <esp_log.h>
#include <esp_timer.h>

#include "Joystick.h"

static const char *TAG = "JOYSTICK";

Joystick::Joystick(gpio_num_t buttonPin, adc1_channel_t xPin, adc1_channel_t yPin) : Button(buttonPin, PT_none), xPin(xPin), yPin(yPin)
{
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(xPin, ADC_ATTEN_DB_12);
    adc1_config_channel_atten(yPin, ADC_ATTEN_DB_12);

    gpio_set_direction(buttonPin, GPIO_MODE_INPUT);
    gpio_intr_disable(buttonPin);
    gpio_pulldown_en(buttonPin);
    gpio_set_level(buttonPin, 0);
}

Joystick::~Joystick()
{

}

void Joystick::SetOnXPressed(onJoystick_cb onPressed, void *ptr)
{
    this->onXPressed = onPressed;
    this->onXPressed_ptr = ptr;
}
void Joystick::SetOnXRelease(onJoystick_cb OnRelease, void *ptr)
{
    this->onXPressed = OnRelease;
    this->onXPressed_ptr = ptr;
}

void Joystick::Update()
{
    int newXStatus;
    int newYStatus;

    newXStatus = position.x = adc1_get_raw(xPin);
    newYStatus = position.y = adc1_get_raw(yPin);

    if(newXStatus != xStatus)
    {
        xStatus = newXStatus;

        if(onXRelease && xStatus == 0)
        {
            onXRelease(onXRelease_ptr);
        }
        else if(onXPressed && xStatus == 1)
        {
            onXPressed(onXPressed_ptr);
        }

        if(onYRelease && yStatus == 0)
        {
            onYRelease(onYRelease_ptr);
        }
        else if(onYPressed && yStatus == 1)
        {
            onYPressed(onYPressed_ptr);
        }
    }

    Button::Update();
}

Joystick::Position_s Joystick::GetPosition()
{
    return position;
}
