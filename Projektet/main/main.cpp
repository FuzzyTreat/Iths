#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "led_strip.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_err.h"
#include <ws2812.h>

#define RGB_LED_PIN GPIO_NUM_19
#define NUMBER_OF_LEDS 8

const char *TAG = "Main";

ws2812 *led_strip;

void GetPixelColor(uint16_t index, LedColor_t &led);

extern "C" void app_main(void)
{
    // RED 255,0,0
    // GREEN 0,255,0
    // BLUE 0,0,255

    // GREEN 0,255,0
    // YELLOW 255,255,0
    // ORANGE 255,128,0
    // RED 255,0,0

    // led_strip = new ws2812(RGB_LED_PIN, NUMBER_OF_LEDS, LED_PIXEL_FORMAT_GRB, LED_MODEL_WS2812, SPI2_HOST);
    // led_strip->configure_led();

    // // bool led_on_off = false;
    // // uint32_t R = 0;
    // // uint32_t G = 0;
    // // uint32_t B = 0;

    // // ESP_LOGI(TAG, "Start blinking LED strip");
    // while (1) 
    // {
    //     /* Set the LED pixel using RGB from 0 (0%) to 255 (100%) for each color */
    //     for (int i = 0; i < LED_STRIP_LED_NUMBERS; i++) {
            
    //         GetPixelColor(i, led_strip->leds[i]);

    //         ESP_ERROR_CHECK(led_strip->SetPixel(i , led_strip->leds[i]));
    //         //ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, i, R++, G++, B++));
    //     }
    //     /* Refresh the strip to send data */
    //     ESP_ERROR_CHECK(led_strip->Update());

    // //     ESP_LOGI(TAG,"R: %lu G: %lu B: %lu", R, G, B);
    //     vTaskDelay(pdMS_TO_TICKS(500));
    // }
}

// GREEN 0,255,0
// YELLOW 255,255,0
// ORANGE 255,128,0
// RED 255,0,0

/// @brief 0 - 255 set color register and brightness, so 0 is unlit, 255 is very bright
/// @param index 
/// @param led 
void GetPixelColor(uint16_t index, LedColor_t &led)
{
    switch(index)
    {
        case 0:
        case 1:
        {
            led.R = 0;
            // led.G = 255;
            led.G = 10;
            led.B = 0;
            led.isOn = true;
            break;
        }
        case 2:
        case 3:
        {
            led.R = 10;
            led.G = 10;
            led.B = 0;
            led.isOn = true;
            break;
        }
        case 4:
        case 5:
        {
            led.R = 10;
            led.G = 5;
            led.B = 0;
            led.isOn = true;
            break;
        }
        case 6:
        case 7:
        {
            led.R = 10;
            led.G = 0;
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