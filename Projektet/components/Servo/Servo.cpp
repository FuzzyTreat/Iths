#include "Servo.h"

static const char *TAG = "Servo";

Servo::Servo(gpio_num_t pin, ledc_channel_t channel):pin(pin), channel(channel)
{
    // PWM TIMER
    ledc_timer_config_t timerConfig;
    timerConfig.clk_cfg = LEDC_AUTO_CLK;
    timerConfig.duty_resolution = LEDC_TIMER_12_BIT;
    timerConfig.freq_hz = 50;
    timerConfig.speed_mode = LEDC_LOW_SPEED_MODE;
    timerConfig.timer_num = LEDC_TIMER_0;
    timerConfig.deconfigure = false;
    ESP_ERROR_CHECK(ledc_timer_config(&timerConfig));

    // PWM PIN CHANNEL
    ledc_channel_config_t channelConfig;
    channelConfig.channel = channel;
    channelConfig.gpio_num = pin;
    channelConfig.intr_type = LEDC_INTR_DISABLE;
    channelConfig.speed_mode = LEDC_LOW_SPEED_MODE;
    channelConfig.timer_sel = LEDC_TIMER_0;

    channelConfig.duty = 0;
    channelConfig.hpoint = 0; // Var finns cykelns high point, 0 => Hög läge i början av cykel

    ESP_ERROR_CHECK(ledc_channel_config(&channelConfig));
};

Servo::~Servo(){};

/// @brief Updates the selected ledc_channel with the calculated duty value based on the current angle.
/// @param angle 
void Servo::Update(double angle)
{
    ledc_set_duty(LEDC_LOW_SPEED_MODE, channel, calculateDuty(angle));
    ledc_update_duty(LEDC_LOW_SPEED_MODE, channel);
}

/// @brief Makes sure the angle stays with valid boundry values, ie 0 degrees and 180 degrees
/// @param angle 
/// @return returns 0 if <= 0 or 180 if >= 180 or a calulated value somewhere between the boundries
int32_t Servo::calculateDuty(double angle)
{
    if(angle >= 180)
    {
        return DEGREE_180_DUTY;
    }
    else if(angle <= 0)
    {
        return DEGREE_0_DUTY;
    }

    return (angle * (((double)DEGREE_180_DUTY - (double)DEGREE_0_DUTY) / (double)180.0)) + (double)DEGREE_0_DUTY;

}

/// @brief Calulates the new angle and changes direction if necessary at 0 deg and 180 deg
/// @param angle 
/// @return the updated value of angle
double Servo::CalculateAngle(double angle)
{
    if(direction)
    {
        angle += 0.5;

        if(angle >= 180)
        {
            direction = false;
        }
    }
    else
    {
        angle -= 0.5;

        if(angle <= 0)
        {
            direction = true;
        }
    }

    return angle;
}
