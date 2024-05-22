#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "Led_Light.h"

static const char *LED_TAG = "LED";

Led_Light::Led_Light(gpio_num_t pin): pin(pin)
{
    ESP_LOGI(LED_TAG, "Create LED on pin %d", pin);
    esp_rom_gpio_pad_select_gpio(pin);
    gpio_set_direction(pin, GPIO_MODE_OUTPUT);
};

Led_Light::~Led_Light()
{
};

void Led_Light::On()
{
    ESP_LOGI(LED_TAG, "Turning led with pin %d on.", pin);
    gpio_set_level(pin, 1);
};

void Led_Light::Off()
{
    ESP_LOGI(LED_TAG, "Turning led with pin %d off.", pin);
    gpio_set_level(pin, 0);
};

void Led_Light::blink()
{
    this->blink(1000,1000);
}

void Led_Light::blink(int timeOn, int timeOff)
{
    for(size_t i =0; i< 10; i++)
    {
        On();
        vTaskDelay(pdMS_TO_TICKS(timeOn));

        Off();
        vTaskDelay(pdMS_TO_TICKS(timeOff));
    }
};