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
}

Joystick::~Joystick()
{

}

void Joystick::Update()
{
    Button::Update();

    position.x = adc1_get_raw(xPin);
    position.y = adc1_get_raw(yPin);
}

Joystick::Position_s Joystick::GetPosition()
{
    return position;
}
