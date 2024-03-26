
#ifndef __LED_STROBE_CONFIG_H
#define __LED_STROBE_CONFIG_H

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "driver/gpio.h"

#define TIME_LED_STROBE_MS                 10

void LED_STROBE_INIT( void );

void LED_STROBE( void );

#endif