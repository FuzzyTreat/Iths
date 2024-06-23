#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "led_strip.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_err.h"
#include <ws2812.h>
#include <UltrasonicSensor.h>

// Led strip
#define RGB_LED_PIN GPIO_NUM_19
#define NUMBER_OF_LEDS 8

// Ultrasonic
#define TRIGGER_GPIO GPIO_NUM_14
#define ECHO_GPIO GPIO_NUM_13
#define MAX_DISTANCE_CM 500 // 5m max

const char *TAG = "Main";

ws2812 *led_strip;


void GetPixelColor(uint16_t index, LedColor_t &led);
void ultrasonic_test(void *pvParameters);

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

    // for(;;)
    // {
    //     xTaskCreate(ultrasonic_test, "ultrasonic_test", configMINIMAL_STACK_SIZE * 3, NULL, 5, NULL);
    //     vTaskDelay(pdMS_TO_TICKS(10));
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


void ultrasonic_test(void *pvParameters)
{
    ultrasonic_sensor_t sensor{
        .trigger_pin = TRIGGER_GPIO,
        .echo_pin = ECHO_GPIO,
    };

    ultrasonic_init(&sensor);

    while (true)
    {
        float distance;
        esp_err_t res = ultrasonic_measure(&sensor, MAX_DISTANCE_CM, &distance);
        if (res != ESP_OK)
        {
            printf("Error %d: ", res);
            switch (res)
            {
                case ESP_ERR_ULTRASONIC_PING:
                    printf("Cannot ping (device is in invalid state)\n");
                    break;
                case ESP_ERR_ULTRASONIC_PING_TIMEOUT:
                    printf("Ping timeout (no device found)\n");
                    break;
                case ESP_ERR_ULTRASONIC_ECHO_TIMEOUT:
                    printf("Echo timeout (i.e. distance too big)\n");
                    break;
                default:
                    printf("%s\n", esp_err_to_name(res));
            }
        }
        else
            printf("Distance: %0.04f cm\n", distance*100);

        vTaskDelay(pdMS_TO_TICKS(500));
    }
}