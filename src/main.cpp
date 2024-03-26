
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

//#include "config/led_config.ccp"
#include "config/led_strobe_config.h"
#include "config/can_sniffer.h"
#include "config/serial_config.h"

extern "C" void app_main() 
{
    vTaskDelay( pdMS_TO_TICKS( 2000 ) );
    printf( "Start CAN Hacker\n" );

    LED_STROBE_INIT();
    CAN_SNIFFER_INIT();
    SERIAL_INIT();

    LED_STROBE();

    vTaskSuspend( NULL );

    while( 1 );
}



