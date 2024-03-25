
#ifndef __CAN_SNIFFER_H
#define __CAN_SNIFFER_H

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "driver/twai.h"
#include "config/serial_config.h"

void CAN_SNIFFER_INIT( void );

void CAN_SNIFFER_START( void );
void CAN_SNIFFER_STOP( void );

#endif