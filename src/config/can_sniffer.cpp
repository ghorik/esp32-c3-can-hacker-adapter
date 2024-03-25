
#include "config/can_sniffer.h"

static TaskHandle_t TaskHandle_CAN_SNIFFER = NULL;

static void vTask_CAN_SNIFFER( void* pvParameters );
static void PDO_TO_SERIAL( const twai_message_t* message );
static void APPEND_STD_ID_TO_BUFFER( int value, char* buffer );
static void APPEND_EXT_ID_TO_BUFFER( int value, char* buffer );
static void APPEND_DATA_TO_BUFFER( uint8_t data, char* buffer );
static char NIBBLE_TO_HEX( uint8_t nibble );

void CAN_SNIFFER_INIT( void )
{
    xTaskCreate(
        vTask_CAN_SNIFFER,
        "SERIAL",
        4096,
        NULL,
        configTIMER_TASK_PRIORITY,
        &TaskHandle_CAN_SNIFFER
    );
    vTaskSuspend( TaskHandle_CAN_SNIFFER );
}

void CAN_SNIFFER_START( void )
{
    vTaskResume( TaskHandle_CAN_SNIFFER );
}

void CAN_SNIFFER_STOP( void )
{
    vTaskSuspend( TaskHandle_CAN_SNIFFER );
}

void vTask_CAN_SNIFFER( void* pvParameters )
{
    for ( ; ; )
    {
        twai_message_t message;
        if ( twai_receive( &message, pdMS_TO_TICKS( portMAX_DELAY ) ) == ESP_OK)
        {
            PDO_TO_SERIAL( &message );
        }
    }
}

void PDO_TO_SERIAL( const twai_message_t* message )
{
    int len_buffer = 0;
    char buffer[ 21 ];

    if ( message->extd )
    {
        if ( message->rtr )
        {
            buffer[ len_buffer ] = 'R';
        }
        else
        {
            buffer[ len_buffer ] = 'T';
        }
        len_buffer++;

        APPEND_EXT_ID_TO_BUFFER( message->identifier, &buffer[ len_buffer ] );
        len_buffer += 8;
    }
    else
    {
        if ( message->rtr )
        {
            buffer[ len_buffer ] = 'R';
        }
        else
        {
            buffer[ len_buffer ] = 't';
        }
        len_buffer++;

        APPEND_STD_ID_TO_BUFFER( message->identifier, &buffer[ len_buffer ] );
        len_buffer += 3;
    }

    buffer[ len_buffer ] = NIBBLE_TO_HEX( message->data_length_code );
    len_buffer++;

    for ( int i = 0; i < message->data_length_code; i++ )
    {
        APPEND_DATA_TO_BUFFER( message->data[ i ], &buffer[ len_buffer ] );
        len_buffer += 2;
    }

    buffer[ len_buffer ] = '\r';
    len_buffer++;

    usb_serial_jtag_write_bytes( buffer, len_buffer, portMAX_DELAY );
}

void APPEND_STD_ID_TO_BUFFER( int value, char* buffer )
{
    for( int i = 0; i < 3; i++ )
    {
        buffer[ 2 - i ] = NIBBLE_TO_HEX( (uint8_t)(value >> ( 4 * i ) ) & 0x0F );
    }
}

void APPEND_EXT_ID_TO_BUFFER( int value, char* buffer )
{
    for( int i = 0; i < 8; i++ )
    {
        buffer[ 7 - i ] = NIBBLE_TO_HEX( (uint8_t)(value >> ( 4 * i ) ) & 0x0F );
    }
}

void APPEND_DATA_TO_BUFFER( uint8_t data, char* buffer )
{
    for ( int i = 0; i < 2; i++ )
    {
        buffer[ 1 - i ] = NIBBLE_TO_HEX( (uint8_t)(data >> ( 4 * i ) ) & 0x0F );
    }
}

char NIBBLE_TO_HEX( uint8_t nibble )
{
    char ret;
    if ( nibble <= 9 )
    {
        ret = nibble + '0';
    }
    else
    {
        ret = nibble + 'A' - 10;
    }
    return ret;
}
