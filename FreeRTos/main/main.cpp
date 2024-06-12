#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include <freertos/task.h>
#include "esp_timer.h"
#include "esp_log.h"

const char *TAG = "Main";

void vTaskCode( void * pvParameters );

extern "C" void app_main(void)
{
    // BaseType_t xReturned;
    // TaskHandle_t xHandle = NULL;
 
    // xReturned = xTaskCreate(
    //                 vTaskCode,       /* Function that implements the task. */
    //                 "Task 1",          /* Text name for the task. */
    //                 6000,      /* Stack size in words, not bytes. */
    //                 ( void * ) 1,    /* Parameter passed into the task. */
    //                 tskIDLE_PRIORITY,/* Priority at which the task is created. */
    //                 &xHandle );      /* Used to pass out the created task's handle. */

    // if( xReturned == pdPASS )
    // {
    //     /* The task was created.  Use the task's handle to delete the task. */
    //     vTaskDelete( xHandle );
    // }

    // vTaskStartScheduler();

    // for(;;){} // Should not run this, the function has an eternal loop
}

void vTaskCode( void * pvParameters )
{
    for(;;)
    {
        vTaskDelay(pdMS_TO_TICKS(5000));
        ESP_LOGI("Task", "Inside task code function.");
    }
};