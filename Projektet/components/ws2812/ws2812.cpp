#include <stdio.h>
#include "esp_log.h"
#include "esp_err.h"

#include "ws2812.h"

static const char *TAG = "ws2812";

ws2812::ws2812(gpio_num_t pin, uint16_t numOfLeds, led_pixel_format_t pixelFormat, led_model_t ledModel,spi_host_device_t spiHostDevice):ledPin(pin), numberOfLeds(numOfLeds), pixelFormat(pixelFormat), ledModel(ledModel), spiHostDevice(spiHostDevice)
{
    for(int i=0; i< numberOfLeds; i++)
    {
        leds[i].Index = i;
        leds[i].R = 0;
        leds[i].G = 0;
        leds[i].B = 0;
        leds[i].isOn = false;
    }
}

ws2812::~ws2812()
{
    led_strip_del(led_strip);
}

void ws2812::configure_led(void)
{
    // LED strip general initialization, according to your led board design
    led_strip_config_t strip_config = {
        .strip_gpio_num = ledPin,   // The GPIO that connected to the LED strip's data line
        .max_leds = numberOfLeds,        // The number of LEDs in the strip,
        .led_pixel_format = pixelFormat, // Pixel format of your LED strip
        .led_model = ledModel,            // LED strip model
        .flags{.invert_out = false},                // whether to invert the output signal
    };

    // LED strip backend configuration: SPI
    led_strip_spi_config_t spi_config = {
        .clk_src = SPI_CLK_SRC_DEFAULT, // different clock source can lead to different power consumption
        .spi_bus = spiHostDevice,           // SPI bus ID
        .flags{.with_dma = true},         // Using DMA can improve performance and help drive more LEDs
    };

    // LED Strip object handle
    ESP_ERROR_CHECK(led_strip_new_spi_device(&strip_config, &spi_config, &led_strip));
    // ESP_LOGI(TAG, "Created LED strip object with SPI backend");
}

esp_err_t ws2812::SetLedColor(LedColor_t &led)
{
    return SetLedColor(led.Index, led.R, led.G, led.B);
}

esp_err_t ws2812::SetLedColor(uint32_t ledIndex, uint32_t r, uint32_t g, uint32_t b)
{
    return led_strip_set_pixel(led_strip, ledIndex, r, g, b);
}

void ws2812::ResetLeds()
{
    for(int i = 0; i < numberOfLeds; i++)
    {
        leds[i].R = 0;
        leds[i].G = 0;
        leds[i].B = 0;
        leds[i].scaleFactor = 0;
        leds[i].isOn = false;
    }
}

/// @brief Applies the current settings stored in the led array to the led strip and then refreshes the strip.
/// @return 
esp_err_t ws2812::Update()
{
    for(int i = 0; i< numberOfLeds; i++)
    {
        SetLedColor(leds[i]);
    }

    return led_strip_refresh(led_strip);
}

esp_err_t ws2812::Clear()
{
    return led_strip_clear(led_strip);
}
