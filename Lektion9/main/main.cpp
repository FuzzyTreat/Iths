#include <stdio.h>
#include <iostream>
#include "freertos/FreeRTOS.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "driver/adc.h"
#include <driver/ledc.h>

#define LED_PIN GPIO_NUM_27

static const char *TAG = "MAIN";

/* Allow resolution of undecorated (a.k.a. not mangled), C-style references.  */
extern "C" 
{
    void app_main(void);
}

void app_main(void) 
{
    ledc_timer_config_t timerConfig;
    timerConfig.clk_cfg = LEDC_AUTO_CLK;
    timerConfig.duty_resolution = LEDC_TIMER_12_BIT;
    timerConfig.freq_hz = 100;
    timerConfig.speed_mode = LEDC_LOW_SPEED_MODE;
    timerConfig.timer_num = LEDC_TIMER_0;
    timerConfig.deconfigure = false;
    ESP_ERROR_CHECK(ledc_timer_config(&timerConfig));

    ledc_channel_config_t channelConfig;
    channelConfig.channel = LEDC_CHANNEL_0;
    channelConfig.gpio_num = LED_PIN;
    channelConfig.intr_type = LEDC_INTR_DISABLE;
    channelConfig.speed_mode = LEDC_LOW_SPEED_MODE;
    channelConfig.timer_sel = LEDC_TIMER_0;

    channelConfig.duty = 0;
    channelConfig.hpoint = 0; // Var finns cykelns hig point

    ESP_ERROR_CHECK(ledc_channel_config(&channelConfig));

    uint32_t duty = 0;

    for(;;)
    {
        ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 0b111111);
        duty += 10;
        duty = duty % 0b111111111111;

        ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);

        vTaskDelay(pdMS_TO_TICKS(100));
        ESP_LOGI(TAG,"Duty %lu", duty);
    }
} 