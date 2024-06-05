#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "esp_log.h"
#include "esp_sleep.h"
#include "driver/gpio.h"
#include <driver/rtc_io.h>
#include "nvs_flash.h"

const char *TAG = "MAIN";
const char *ITHS_SLEEP_NAMESPACE = "iths sleep";

int32_t counter = 0;
RTC_DATA_ATTR esp_sleep_wakeup_cause_t wakeUpCause;

#define SLEEP_WAKEUP_TIME 2000 * 1000
#define WAKEUP_BUTTON_PIN GPIO_NUM_25

void handleWakeupCause();

nvs_handle_t storageHandle;

bool isRunning = true;

extern "C" void app_main(void)
{
    ESP_LOGI(TAG,"app_main start.");

    esp_err_t err = nvs_flash_init();

    // Check if there is available memory or if the data/instructions has changed
    if(err == ESP_ERR_NVS_NO_FREE_PAGES ||err == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        nvs_flash_erase();
        err = nvs_flash_init();
    }
    
    ESP_ERROR_CHECK(err);

    // Will probably give and error if not set to READWRITE
    err = nvs_open(ITHS_SLEEP_NAMESPACE, NVS_READWRITE, &storageHandle);

    if(err != ESP_OK)
    {
        ESP_ERROR_CHECK_WITHOUT_ABORT(err);
    }
    else
    {
        err = nvs_get_i32(storageHandle, "counter", &counter);

        if(err != ESP_OK)
        {
            if(err == ESP_ERR_NVS_KEYS_NOT_INITIALIZED)
            {
                
            }else
            {
                ESP_ERROR_CHECK_WITHOUT_ABORT(err);
            }
        }

        nvs_close(storageHandle);
    }

    handleWakeupCause();

    esp_sleep_enable_timer_wakeup(SLEEP_WAKEUP_TIME);

    gpio_config_t wakeupConfig;
    wakeupConfig.pin_bit_mask = 1ULL << WAKEUP_BUTTON_PIN;
    wakeupConfig.mode = GPIO_MODE_INPUT;
    wakeupConfig.pull_down_en = GPIO_PULLDOWN_DISABLE;
    wakeupConfig.pull_up_en = GPIO_PULLUP_DISABLE;
    wakeupConfig.intr_type = GPIO_INTR_DISABLE;

    ESP_ERROR_CHECK(gpio_config(&wakeupConfig));
    ESP_ERROR_CHECK(gpio_wakeup_enable(WAKEUP_BUTTON_PIN, GPIO_INTR_HIGH_LEVEL));
    
    esp_sleep_enable_gpio_wakeup();

    // while(true)
    // {
    //     ESP_LOGI(TAG,"Counter: %d", counter++);
    //     vTaskDelay(50);

    //     if(counter % 20 == 0)
    //     {
    //         esp_deep_sleep_start();
    //     }

    //     ESP_ERROR_CHECK(esp_light_sleep_start());

    //     HandleWakeupCall();

    //     vTaskDelay(pdMS_TO_TICKS(1000));
    // }
}

void handleWakeupCause()
{
    wakeUpCause = esp_sleep_get_wakeup_cause();

    switch(wakeUpCause)
    {
        case ESP_SLEEP_WAKEUP_TIMER:
        {
            ESP_LOGI(TAG,"Woke up at counter %ld from TIMER.", counter);
            break;
        }
        case ESP_SLEEP_WAKEUP_GPIO:
        {
            ESP_LOGI(TAG,"Woke up at counter %ld from GPIO.", counter);
            break;
        }
        default:
        {
            ESP_LOGI(TAG,"Woke up at counter %ld from unkown cause: %d",counter, wakeUpCause);
            break;
        }
    }
}