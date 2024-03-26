
#ifndef __SERIAL_CONFIG_H
#define __SERIAL_CONFIG_H

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "driver/usb_serial_jtag.h"
#include "config/led_strobe_config.h"
#include <HexChar.h>

void SERIAL_INIT( void );

#endif