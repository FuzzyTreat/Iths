#include <stdio.h>
#include <iostream>
#include "freertos/FreeRTOS.h"
#include "driver/gpio.h"
#include "driver/adc.h"
#include <driver/ledc.h>
#include "esp_log.h"
#include "Servo.h"

#define SERVO_1_PIN GPIO_NUM_27
#define SERVO_1_CHANNEL LEDC_CHANNEL_0

static const char *TAG = "MAIN";

Servo *servo1;

/* Allow resolution of undecorated (a.k.a. not mangled), C-style references.  */
extern "C" void app_main(void) 
{
    servo1 = new Servo(SERVO_1_PIN, SERVO_1_CHANNEL);

    // uint32_t duty = 0;
    double angle = 0;
    // bool direction = true;
    
    servo1->Update(0);

    for(;;)
    {
        servo1->Update(angle);

        //     duty = calculateDuty(angle);

        //     ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 0b111111);
        //     duty += 10;
        //     duty = duty % 0b111111111111;

        //     ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);

        vTaskDelay(pdMS_TO_TICKS(10));
        ESP_LOGI(TAG,"Angle %lf", angle);

        angle = servo1->CalculateAngle(angle);
    }
} 

uint32_t calculateDuty(double degrees)
{
    if(degrees >= 180)
    {
        return DEGREE_180_DUTY;
    }
    else if(degrees <= 0)
    {
        return DEGREE_0_DUTY;
    }

    return (degrees * (((double)DEGREE_180_DUTY - (double)DEGREE_0_DUTY) / (double)180.0)) + (double)DEGREE_0_DUTY;
}