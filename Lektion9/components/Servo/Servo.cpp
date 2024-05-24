#include "Servo.h"

// #define MAX_DUTY 0b111111111111
// #define DEGREE_0_DUTY 102
// #define DEGREE_180_DUTY 512

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
    channelConfig.hpoint = 0; // Var finns cykelns hig point

    ESP_ERROR_CHECK(ledc_channel_config(&channelConfig));
};

Servo::~Servo(){};

void Servo::goToAngle(double degrees)
{
    uint32_t duty = 0;

    if(degrees >= 180)
    {
        duty = DEGREE_180_DUTY;
    }
    else if(degrees <= 0)
    {
        duty = DEGREE_0_DUTY;
    }

    duty = (degrees * (((double)DEGREE_180_DUTY - (double)DEGREE_0_DUTY) / (double)180.0)) + (double)DEGREE_0_DUTY;

    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 0b111111);
    duty += 10;
    duty = duty % 0b111111111111;

    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
}
