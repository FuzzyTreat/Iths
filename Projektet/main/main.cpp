#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "led_strip.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_err.h"
#include <ws2812.h>
#include <ultrasonic.h>

// Led strip
#define RGB_LED_PIN GPIO_NUM_5
#define NUMBER_OF_LEDS 8
#define DEFAULT_LED_BRIGHTNESS 10

// Ultrasonic
#define TRIGGER_PIN GPIO_NUM_19
#define ECHO_PIN GPIO_NUM_18
#define MAX_DISTANCE_CM 500 // 5m max
#define RANGE_STEP MAX_DISTANCE_CM / 8

const char *TAG = "Main";

ws2812 *led_strip;
ultrasonic_sensor_t sensor;

void GetLedColor(LedColor_t &led);
void SetRangeLed(uint32_t range);
void ultrasonic_test(void *pvParameters);

extern "C" void app_main(void)
{
    led_strip = new ws2812(RGB_LED_PIN, NUMBER_OF_LEDS, LED_PIXEL_FORMAT_GRB, LED_MODEL_WS2812, SPI2_HOST);
    led_strip->configure_led();

    sensor.echo_pin = ECHO_PIN;
    sensor.trigger_pin = TRIGGER_PIN;

    printf("Range_Step is %d cm\n 7 * RANGE_STEP is %d cm\n", RANGE_STEP, RANGE_STEP * 8);

    // xTaskCreate(ultrasonic_test, "ultrasonic_test", configMINIMAL_STACK_SIZE * 3, NULL, 5, NULL);
    
    // for(;;)
    // {
    //     ESP_ERROR_CHECK(led_strip->Update());

    //     vTaskDelay(pdMS_TO_TICKS(50));
    // }
}

void ultrasonic_test(void *pvParameters)
{
    ultrasonic_init(&sensor);
    uint32_t distance = 0;

    while (true)
    {
        esp_err_t res = ultrasonic_measure_cm(&sensor, MAX_DISTANCE_CM, &distance);

        if (res != ESP_OK)
        {
            switch (res)
            {
                case ESP_ERR_ULTRASONIC_PING:
                    printf("\rCannot ping (device is in invalid state)\n");
                    break;
                case ESP_ERR_ULTRASONIC_PING_TIMEOUT:
                    printf("\rPing timeout (no device found)\n");
                    break;
                case ESP_ERR_ULTRASONIC_ECHO_TIMEOUT:
                    printf("\rEcho timeout (i.e. distance too big)\n");
                    break;
                default:
                    printf("\r%s\n", esp_err_to_name(res));
            }

            led_strip->ResetLeds();
        }
        else
        {
            SetRangeLed(distance);

            printf("\rDistance is: %lu cm ", distance);
            fflush(stdout);
        }

        vTaskDelay(pdMS_TO_TICKS(250));
    }
}

void SetRangeLed(uint32_t range)
{
    led_strip->ResetLeds();

    uint32_t step = RANGE_STEP;

    for(int i = 0; i < NUMBER_OF_LEDS; i++)
    {
        if(i == 0)
        {
            led_strip->leds[i].isOn = true;
            led_strip->leds[i].scaleFactor = 10;
            GetLedColor(led_strip->leds[i]);
            led_strip->SetLedColor(led_strip->leds[i]);  
        }
        else if(step <= range)
        {
            led_strip->leds[i].isOn = true;
            led_strip->leds[i].scaleFactor = 10;
            GetLedColor(led_strip->leds[i]);
            led_strip->SetLedColor(led_strip->leds[i]);  
        }
        else
        {
            led_strip->leds[i].isOn = false;
            led_strip->leds[i].scaleFactor = 0;
            led_strip->leds[i].R = 0;
            led_strip->leds[i].G = 0;
            led_strip->leds[i].B = 0;
            led_strip->SetLedColor(led_strip->leds[i]);  
        }

        step = step + RANGE_STEP;
    }
}

// RED 255,0,0
// GREEN 0,255,0
// BLUE 0,0,255

// GREEN 0,255,0
// YELLOW 255,255,0
// ORANGE 255,128,0
// RED 255,0,0

/// @brief 0 - 255 set color register and brightness, so 0 is unlit, 255 is very bright
/// Reduce the brightness of all 3 parts by the same factor
/// @param index 
/// @param led 
void GetLedColor(LedColor_t &led)
{
    switch(led.Index)
    {
        case 0:
        case 1:
        {
            led.R = DEFAULT_LED_BRIGHTNESS;
            led.G = 0;
            led.B = 0;
            led.isOn = true;
            break;
        }
        case 2:
        case 3:
        {
            led.R = DEFAULT_LED_BRIGHTNESS;
            led.G = DEFAULT_LED_BRIGHTNESS/2;
            led.B = 0;
            led.isOn = true;
            break;
        }
        case 4:
        case 5:
        {
            led.R = DEFAULT_LED_BRIGHTNESS;
            led.G = DEFAULT_LED_BRIGHTNESS;
            led.B = 0;
            led.isOn = true;
            break;
        }
        case 6:
        case 7:
        {
            led.R = 0;
            led.G = DEFAULT_LED_BRIGHTNESS;
            led.B = 0;
            led.isOn = true;
            break;
        }
        default:
        {
            led.R = 0;
            led.G = 0;
            led.B = 0;
            led.isOn = false;
            break;
        }
    }
}