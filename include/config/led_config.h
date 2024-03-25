
#ifndef __LED_CONFIG_H
#define __LED_CONFIG_H

#include "driver/gpio.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

typedef enum
{
    LED_OFF = 0,
    LED_ON,
    LED_Blink
}
LED_State;

void BLINK_INIT( void );

void SET_LED_STATE( LED_State state );

#endif
