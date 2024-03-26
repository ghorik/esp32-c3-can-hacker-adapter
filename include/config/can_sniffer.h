
#ifndef __CAN_SNIFFER_H
#define __CAN_SNIFFER_H

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <HexChar.h>
#include "driver/twai.h"
#include "config/serial_config.h"
#include "config/led_strobe_config.h"

void CAN_SNIFFER_INIT( void );

void CAN_SNIFFER_START( void );
void CAN_SNIFFER_STOP( void );

void SET_CAN_SNIFFER_TIMESTAMP_STATE( bool state );

#endif