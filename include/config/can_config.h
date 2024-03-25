
#ifndef __CAN_CONFIG_H
#define __CAN_CONFIG_H

#include "driver/twai.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "esp_err.h"

namespace CAN_INTERFACE
{

    typedef enum
    {
        STATE_RESET = 0,
        STATE_INIT,
        STATE_OPENED
    }
    STATE_Enum;

    typedef enum
    {
        MODE_READ_WRITE = 0,
        MODE_READ_ONLY
    }
    MODE_Enum;

    typedef enum
    {
        MODE_TIMESTAMP_DISABLE = 0,
        MODE_TIMESTAMP_ENABLE
    }
    MODE_TIMESTAMP_Enum;

    typedef enum
    {
        SPEED_10Kbps = 0,
        SPEED_20Kbps,
        SPEED_50Kbps,
        SPEED_100Kbps,
        SPEED_125Kbps,
        SPEED_250Kbps,
        SPEED_500Kbps,
        SPEED_800Kbps,
        SPEED_1Mbps,
        SPEED_COUNT
    }
    SPEED_Enum;

    //void CAN_INIT( void );

    STATE_Enum GET_STATE( void );
    void SET_STATE( const STATE_Enum state );
    esp_err_t RESET_CAN( void );
    esp_err_t SET_SPEED( const SPEED_Enum speed );
    esp_err_t SET_MODE_TIMESTAMP( bool state );
    esp_err_t CAN_OPEN( const MODE_Enum mode );
    esp_err_t SEND_RTR( uint32_t id, uint8_t len );
    esp_err_t SEND_EX_RTR( uint32_t id, uint8_t len );
    esp_err_t SEND_PDO( uint32_t id, uint8_t len, uint8_t* data );
    esp_err_t SEND_EX_PDO( uint32_t id, uint8_t len, uint8_t* data );
}

#endif