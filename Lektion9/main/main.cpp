#include <stdio.h>
#include <iostream>
#include "freertos/FreeRTOS.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "driver/adc.h"
#include <driver/ledc.h>
#include "Servo.h"

// 0.5 ms = 0 grader
// 2.5 ms = 180 grader
// 50 Hz
// 20 ms tidsperiod
// 0.5 / 20 = 0.025 // 2.5%
// 2.5 / 20 = 0.125 // 12.5%

// duty på 2.5% = 0 grader
// duty på 12.5% = 180 grader

// 12  bit = 4095
// 2.5% 4095 ca 102.375
// 12.5% 4095 ca 511.875

#define SERVO_1_PIN GPIO_NUM_27

#define SERVO_1_CHANNEL LEDC_CHANNEL_0

// uint32_t calculateDuty(double degrees);

static const char *TAG = "MAIN";

Servo *servo1;

/* Allow resolution of undecorated (a.k.a. not mangled), C-style references.  */
extern "C" 
{
    void app_main(void);
}

void app_main(void) 
{
    servo1 = new Servo(SERVO_1_PIN, SERVO_1_CHANNEL);

    uint32_t duty = 0;
    double angle = 0;
    bool direction = true;
    
    // servo1->goToAngle(0);

    // for(;;)
    // {
    //     // duty = calculateDuty(angle);

    //     // ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 0b111111);
    //     // duty += 10;
    //     // duty = duty % 0b111111111111;

    //     // ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);

    //     vTaskDelay(pdMS_TO_TICKS(100));
    //     std::cout << "Duty %lu" << duty << " Angle %lf" << angle;
    //     // // ESP_LOGI(TAG,"Duty %lu Angle %lf", duty, angle);

    //     if(direction)
    //     {
    //         angle += 0.5;

    //         if(angle > 180)
    //         {
    //             direction = false;
    //         }
    //     }
    //     else
    //     {
    //         angle -= 0.5;

    //         if(angle <= 0)
    //         {
    //             direction = true;
    //         }
    //     }

    //     servo1->goToAngle(angle);
    // }
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