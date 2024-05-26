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

    double angle = 0;
    
    servo1->Update(angle);

    for(;;)
    {
        servo1->Update(angle);

        vTaskDelay(pdMS_TO_TICKS(10));

        printf("\rAngle: %lf", angle);

        angle = servo1->CalculateAngle(angle);
    }
} 