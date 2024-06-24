#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "led_strip.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_err.h"
#include <ws2812.h>
#include <ultrasonic.h>
#include <Joystick.h>
#include <Servo.h>
#include <LedManager.h>

// Led strip
#define RGB_LED_PIN GPIO_NUM_5
#define NUMBER_OF_LEDS 8
#define DEFAULT_LED_BRIGHTNESS 10

// Ultrasonic
#define TRIGGER_PIN GPIO_NUM_19
#define ECHO_PIN GPIO_NUM_18
#define MAX_DISTANCE_CM 80 // 80 cm max ( 5 m max if changed )
#define RANGE_STEP MAX_DISTANCE_CM / 8

// Joystick
#define JOYSTICK_BUTTON_PIN GPIO_NUM_15
#define JOYSTICK_X_AXIS_CHANNEL ADC1_CHANNEL_3
#define JOYSTICK_Y_AXIS_CHANNEL ADC1_CHANNEL_0

// Servo motor
#define SERVO_PIN GPIO_NUM_15
#define SERVO_LEDC_CHANNEL LEDC_CHANNEL_0

const char *TAG = "Main";

ws2812 *led_strip;
ultrasonic_sensor_t sensor;
Joystick *joystick;
Servo *servo;
LedManager *ledManager;

// void GetLedColor(LedColor_t &led);
// void SetRangeLed(uint32_t range);
void measure_task(void *pvParameters);
void joystick_task(void *ptr);
void onButtonPressed(void *ptr);

extern "C" void app_main(void)
{
    led_strip = new ws2812(RGB_LED_PIN, NUMBER_OF_LEDS, LED_PIXEL_FORMAT_GRB, LED_MODEL_WS2812, SPI2_HOST);
    led_strip->configure_led();

    sensor.echo_pin = ECHO_PIN;
    sensor.trigger_pin = TRIGGER_PIN;

    joystick = new Joystick(JOYSTICK_BUTTON_PIN, JOYSTICK_X_AXIS_CHANNEL, JOYSTICK_Y_AXIS_CHANNEL);
    joystick->SetOnPressed(onButtonPressed, joystick);

    // servo = new Servo(SERVO_PIN, SERVO_LEDC_CHANNEL);

    // ledManager = new LedManager(DEFAULT_LED_BRIGHTNESS, NUMBER_OF_LEDS, MAX_DISTANCE_CM, RANGE_STEP);

    // xTaskCreate(measure_task, "ultrasonic_range", configMINIMAL_STACK_SIZE * 3, NULL, 5, NULL);
    xTaskCreate(joystick_task, "JoystickEvent", configMINIMAL_STACK_SIZE * 3, NULL, tskIDLE_PRIORITY, NULL);

    for(;;)
    {
        //servo->Update();
        //ESP_ERROR_CHECK(led_strip->Update());
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void onButtonPressed(void *ptr)
{
    Button button = *((Button *)ptr);
    printf("Joy X: %d Joy Y: %d    \n", joystick->GetPosition().x, joystick->GetPosition().y);
}

void joystick_task(void *ptr)
{
    for(;;)
    {
        joystick->Update();
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void measure_task(void *pvParameters)
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
            ledManager->SetRangeLed(led_strip, distance);

            printf("\rDistance is: %lu cm ", distance);
            fflush(stdout);
        }

        vTaskDelay(pdMS_TO_TICKS(250));
    }
}
