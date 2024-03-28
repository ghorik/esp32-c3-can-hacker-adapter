#include "config/led_config.h"

static void vTask_BLINK( void* pvParameters );

static volatile LED_State blink_state = LED_ON;

void BLINK_INIT( void )
{
    gpio_set_direction( GPIO_NUM_8, GPIO_MODE_OUTPUT );
    gpio_set_pull_mode( GPIO_NUM_8, GPIO_FLOATING );

    xTaskCreate(
        vTask_BLINK,
        "BLINK",
        2048,
        NULL,
        configTIMER_TASK_PRIORITY,
        NULL
    );
}

void vTask_BLINK( void* pvParameters )
{
    for ( ; ;  )
    {
        if ( blink_state == LED_ON )
        {
             gpio_set_level( GPIO_NUM_8, 0 );
             vTaskDelay( pdMS_TO_TICKS( 500 ) );
        }
        else if ( blink_state == LED_Blink )
        {
            gpio_set_level( GPIO_NUM_8, 1 );
            vTaskDelay( pdMS_TO_TICKS( 500 ) );
            gpio_set_level( GPIO_NUM_8, 0 );
            vTaskDelay( pdMS_TO_TICKS( 300 ) );
        }
        else
        {
            gpio_set_level( GPIO_NUM_8, 1 );
            vTaskDelay( pdMS_TO_TICKS( 500 ) );
        }
    }
}

void SET_LED_STATE( LED_State state )
{
    blink_state = state;
}