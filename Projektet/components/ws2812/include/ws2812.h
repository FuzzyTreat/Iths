#ifndef WS2812_H
#define WS2812_H

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "led_strip.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_err.h"

// GPIO assignment
#define LED_STRIP_GPIO GPIO_NUM_13
// Numbers of the LED in the strip
#define LED_STRIP_LED_NUMBERS 8

// #define LED_PIXEL_FORMAT_GRB 0
// #define LED_MODEL_WS2812 0
// #define SPI2_HOST 1

typedef struct {
    uint16_t Index;
    uint32_t R = 0;
    uint32_t G = 0; 
    uint32_t B = 0;
    float scaleFactor;
    bool isOn;
} LedColor_t;

class ws2812
{
    private:
    gpio_num_t ledPin;
    uint16_t numberOfLeds;
    led_strip_handle_t led_strip;
    led_pixel_format_t pixelFormat;
    led_model_t ledModel;
    spi_host_device_t spiHostDevice;

    protected:

    public:
    ws2812(gpio_num_t pin, uint16_t numOfLeds, led_pixel_format_t pixelFmt, led_model_t ledModel,spi_host_device_t spiHostDevice);
    ~ws2812();

    LedColor_t leds[LED_STRIP_LED_NUMBERS]{};
    
    void configure_led(void);
    esp_err_t SetLedColor(uint32_t ledIndex, uint32_t r, uint32_t g, uint32_t b);
    esp_err_t SetLedColor(LedColor_t &led);
    void ResetLeds();
    esp_err_t Update();
    esp_err_t Clear();
};

#endif
