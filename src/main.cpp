
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "config/led_config.ccp"
#include "config/can_config.h"
#include "config/serial_config.h"

extern "C" void app_main() 
{
    vTaskDelay( pdMS_TO_TICKS( 5000 ) );
    printf( "Start CAN Hacker\n" );

    BLINK_INIT();
    //CAN_INIT();
    SERIAL_INIT();

    SET_LED_STATE( LED_Blink );

    vTaskSuspend( NULL );

    while( 1 )
    {
        //vTaskDelay( pdMS_TO_TICKS( 100 ) );
    }
}



