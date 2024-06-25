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
#include <LCD1602.h>

// Led strip
#define RGB_LED_PIN GPIO_NUM_22
#define NUMBER_OF_LEDS 8
#define DEFAULT_LED_BRIGHTNESS 10

// Ultrasonic
#define TRIGGER_PIN GPIO_NUM_19
#define ECHO_PIN GPIO_NUM_18
#define MAX_DISTANCE_CM 80 // 80 cm max ( 5 m max if changed )
#define RANGE_STEP MAX_DISTANCE_CM / 8

#define ACC_SENSITIVITY ACCE_FS_2G
#define GYRO_SENSITIVITY GYRO_FS_250DPS

// Joystick
#define JOYSTICK_BUTTON_PIN GPIO_NUM_15
#define JOYSTICK_X_AXIS_CHANNEL ADC1_CHANNEL_3
#define JOYSTICK_Y_AXIS_CHANNEL ADC1_CHANNEL_0

// Servo motor
#define SERVO_PIN GPIO_NUM_2
#define SERVO_LEDC_CHANNEL LEDC_CHANNEL_0

// I2C Bus used for the Lcd
#define I2C_SDA_PIN GPIO_NUM_13
#define I2C_SCL_PIN GPIO_NUM_14
#define I2C_FREQ_HZ 100000 // Standard-mode: 100KHz, Fast-mode: 400KHz. Should be max fast-mode if LCD isn't used otherwise 100 KHz
#define I2C_PORT I2C_NUM_0

const char *TAG = "Main";

ws2812 *led_strip;
ultrasonic_sensor_t uc_sensor;
Joystick *joystick;
Servo *servo;
LedManager *ledManager;
LCD1602 *lcd;

void measure_task(void *pvParameters);
void joystick_task(void *ptr);
void onJoyButtonPressed(void *ptr);
void RegisterComponents();

double angle = 0;
uint32_t distance = 0;
std::string displayText = "";
int centerValue = 3045;

extern "C" void app_main(void)
{
    i2c_config_t i2cConfig;

    i2cConfig.mode = I2C_MODE_MASTER;
    i2cConfig.scl_io_num = I2C_SCL_PIN;
    i2cConfig.sda_io_num = I2C_SDA_PIN;
    i2cConfig.scl_pullup_en = GPIO_PULLUP_ENABLE;
    i2cConfig.sda_pullup_en = GPIO_PULLUP_ENABLE;
    i2cConfig.master.clk_speed = I2C_FREQ_HZ;
    //i2cConfig.clk_flags = 0;

    ESP_ERROR_CHECK(i2c_param_config(I2C_PORT, &i2cConfig));
    ESP_ERROR_CHECK(i2c_driver_install(I2C_PORT, i2cConfig.mode, 0, 0, 0));

    RegisterComponents();

    xTaskCreate(measure_task, "ultrasonic_range", configMINIMAL_STACK_SIZE * 3, NULL, 5, NULL);
    xTaskCreate(joystick_task, "JoystickEvent", configMINIMAL_STACK_SIZE * 3, &angle, tskIDLE_PRIORITY, NULL);

    for(;;)
    {
        servo->Update(angle);
        ESP_ERROR_CHECK(led_strip->Update());

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void RegisterComponents()
{
    joystick = new Joystick(JOYSTICK_BUTTON_PIN, JOYSTICK_X_AXIS_CHANNEL, JOYSTICK_Y_AXIS_CHANNEL);
    joystick->SetOnPressed(onJoyButtonPressed, (void *)joystick);

    led_strip = new ws2812(RGB_LED_PIN, NUMBER_OF_LEDS, LED_PIXEL_FORMAT_GRB, LED_MODEL_WS2812, SPI2_HOST);
    led_strip->configure_led();

    uc_sensor.echo_pin = ECHO_PIN;
    uc_sensor.trigger_pin = TRIGGER_PIN;

    servo = new Servo(SERVO_PIN, SERVO_LEDC_CHANNEL);
    servo->Update(angle);

    ledManager = new LedManager(DEFAULT_LED_BRIGHTNESS, NUMBER_OF_LEDS, MAX_DISTANCE_CM, RANGE_STEP);

    lcd = new LCD1602(I2C_SCL_PIN, I2C_SDA_PIN, I2C_PORT);
    lcd->lcd_init();
}

void onJoyButtonPressed(void *ptr)
{
    printf("Joystick button was pressed.\n");

    displayText = "Range: ";
    displayText += std::to_string(distance); 
    displayText += " cm";

    lcd->ClearScreen();
    lcd->lcd_print(displayText.c_str());
}

void joystick_task(void *ptr)
{
    int centerValue = 3045;

    for(;;)
    {
        joystick->Update();

        if(joystick->GetPosition().x < centerValue - 30)
        {
            angle += 1.0;

            if(angle >= 180)
            {
                angle = 180;
            }

            printf("Pressed to the right, angle:  %f \n", angle);
        }
        else if(joystick->GetPosition().x > centerValue + 30)
        {
            angle -= 1.0;

            if(angle <= 0)
            {
                angle = 0;
            }

            printf("Pressed to the left, angle:  %f \n", angle);
        }

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void measure_task(void *pvParameters)
{
    ultrasonic_init(&uc_sensor);


    while (true)
    {
        esp_err_t res = ultrasonic_measure_cm(&uc_sensor, MAX_DISTANCE_CM, &distance);

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
