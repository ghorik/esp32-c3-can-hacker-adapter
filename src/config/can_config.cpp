
#include "config/can_config.h"
#include "config/can_sniffer.h"
#include "project_config.h"
#include <string.h>

static CAN_INTERFACE::STATE_Enum can_state = CAN_INTERFACE::STATE_RESET;
static CAN_INTERFACE::SPEED_Enum can_speed = CAN_INTERFACE::SPEED_10Kbps;

static twai_timing_config_t can_timings[ CAN_INTERFACE::SPEED_COUNT ] = {
        TWAI_TIMING_CONFIG_10KBITS(),
        TWAI_TIMING_CONFIG_20KBITS(),
        TWAI_TIMING_CONFIG_50KBITS(),
        TWAI_TIMING_CONFIG_100KBITS(),
        TWAI_TIMING_CONFIG_125KBITS(),
        TWAI_TIMING_CONFIG_250KBITS(),
        TWAI_TIMING_CONFIG_500KBITS(),
        TWAI_TIMING_CONFIG_800KBITS(),
        TWAI_TIMING_CONFIG_1MBITS()
    };

CAN_INTERFACE::STATE_Enum CAN_INTERFACE::GET_STATE( void )
{
    return can_state;
}

void CAN_INTERFACE::SET_STATE( const STATE_Enum state )
{
    can_state = state;
}

esp_err_t CAN_INTERFACE::RESET_CAN( void )
{
    if ( can_state == STATE_RESET )
    {
        return ESP_FAIL; 
    }

    can_state = STATE_RESET;

    CAN_SNIFFER_STOP();

    if ( twai_stop() == ESP_FAIL )
    {
        return ESP_FAIL;
    }

    if ( twai_driver_uninstall() == ESP_FAIL )
    {
        return ESP_FAIL;
    }
    
    return ESP_OK;
}

esp_err_t CAN_INTERFACE::SET_SPEED( const SPEED_Enum speed )
{
    if ( GET_STATE() == STATE_OPENED )
    {
        return ESP_FAIL;
    }

    can_speed = speed;

    can_state = STATE_INIT;
    
    return ESP_OK;
}

esp_err_t CAN_INTERFACE::SET_MODE_TIMESTAMP( bool state )
{
    if ( GET_STATE() == STATE_OPENED )
    {
       return ESP_FAIL;
    }

    SET_CAN_SNIFFER_TIMESTAMP_STATE( state );
    return ESP_OK;
}

esp_err_t CAN_INTERFACE::CAN_OPEN( const MODE_Enum mode )
{
    if ( GET_STATE() != STATE_INIT )
    {
        return ESP_FAIL;
    }

    twai_mode_t can_mode;
    if ( mode == MODE_READ_ONLY )
    {
        can_mode = TWAI_MODE_LISTEN_ONLY;
    }
    {
        can_mode = TWAI_MODE_NORMAL;
    }

    twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT( CAN_CTX_PIN, CAN_CRX_PIN, can_mode );
    twai_timing_config_t t_config;
    memcpy( &t_config, &can_timings[ can_speed ], sizeof( twai_timing_config_t ) );

    twai_filter_config_t f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();

    if ( twai_driver_install( &g_config, &t_config, &f_config ) == ESP_FAIL ) 
    {
        return ESP_FAIL;
    }

    if ( twai_start() == ESP_FAIL )
    {
        return ESP_FAIL;
    } 


    CAN_SNIFFER_START();
    can_state = STATE_OPENED;
    return ESP_OK;
}

esp_err_t CAN_INTERFACE::SEND_MSG( twai_message_t* message )
{
    if ( GET_STATE() != STATE_OPENED )
    {
        return ESP_FAIL;
    }

    return twai_transmit( message, pdMS_TO_TICKS( 1000 ) );
}
