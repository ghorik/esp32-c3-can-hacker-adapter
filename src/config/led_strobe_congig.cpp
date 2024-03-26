
#include "config/led_strobe_config.h"

static TickType_t time_start_strobe;

static void vTask_STROBE( void* pvParameters );

void LED_STROBE_INIT( void )
{
    gpio_set_direction( GPIO_NUM_8, GPIO_MODE_OUTPUT );
    gpio_set_pull_mode( GPIO_NUM_8, GPIO_FLOATING );

    xTaskCreate(
        vTask_STROBE,
        "STROBE",
        2048,
        NULL,
        configTIMER_TASK_PRIORITY,
        NULL
    );
}

void vTask_STROBE( void* pvParameters )
{
    for ( ; ; )
    {
        vTaskDelay( 1 );
        if ( ( xTaskGetTickCount() - time_start_strobe ) > TIME_LED_STROBE_MS )
        {
            gpio_set_level( GPIO_NUM_8, 1 );
        }
        else
        {
            gpio_set_level( GPIO_NUM_8, 0 );
        }
    }
}

void LED_STROBE( void )
{
    vTaskSuspendAll();
    time_start_strobe = xTaskGetTickCount();
    xTaskResumeAll();
}