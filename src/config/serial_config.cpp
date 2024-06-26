
#include "config/serial_config.h"
#include "config/can_config.h"
#include "config/led_config.h"
#include "driver/twai.h"
#include "version.h"
#include <string.h>

#define MAX_LEN_COMMAND             128
static int len_command = 0;
static char command[ MAX_LEN_COMMAND ];

static const char ANSWER_OK = '\r';
static const char ANSWER_ERROR = '\a';

static void vTask_SERIAL( void* pvParameters );
static void PARSE_COMMAND( char* command );

static void SEND_PCB_VERSION( void );
static void SEND_FIRMVARE_VERSION( void );
static void SEND_SERIAL_NUMBER( void );
static void RESET_CAN( void );
static void SET_CAN_SPEED( char num_speed );
static void SET_CAN_TIMESTAMP( char state );
static void CAN_OPEN_READ_ONLY( void );
static void CAN_OPEN_READ_WRITE( void );
static void CAN_SEND_MSG( char* command, bool extd, bool rtr );


static esp_err_t READ_ID( char* command, uint32_t* id, bool extd );
static esp_err_t READ_LEN( char* command, uint8_t* len );
static esp_err_t READ_DATA( char* command, const uint8_t len, uint8_t* data );

void SERIAL_INIT( void )
{
    usb_serial_jtag_driver_config_t serial_config = {
        .tx_buffer_size = 256,
        .rx_buffer_size = 256
    };
    if ( usb_serial_jtag_driver_install( &serial_config ) == ESP_FAIL )
    {
        printf( "Не удалось инициализировать драйвер usb_serial_jtag\r" );
        return;
    }
    xTaskCreate(
        vTask_SERIAL,
        "SERIAL",
        4096,
        NULL,
        configTIMER_TASK_PRIORITY,
        NULL
    );
}

void vTask_SERIAL( void* pvParameters )
{
    for ( ; ; )
    {
        char buffer[ 64 ];
        int count = usb_serial_jtag_read_bytes( buffer, 64, 0 );
        if ( count == 0 )
        {
            vTaskDelay( 1 );
            continue;
        }

        //usb_serial_jtag_write_bytes( buffer, count, portMAX_DELAY );
        for ( int i = 0; i < count; i++ )
        {
            char ch =  buffer[ i ];
            switch ( ch )
            {
                case '\r':
                case '\n':
                    if ( len_command == 0 )
                    {
                        break;
                    }
                    command[ len_command ] = '\0';
                    PARSE_COMMAND( command );
                    len_command = 0;
                    break;
            
                default:
                    if ( len_command < MAX_LEN_COMMAND )
                    {
                        command[ len_command ] = ch;
                        ++len_command;
                    }
                    break;
            }
        }
    }
}

void PARSE_COMMAND( char* command )
{   
    LED_STROBE();
    switch ( command[ 0 ] )
    {
    case 'V':   //Чтение версии программного обеспечения и платы
        SEND_PCB_VERSION();
        break;
    case 'v':   //Чтение подробной версии программного обеспечения
        SEND_FIRMVARE_VERSION();
        break;
    case 'N':   //Серийный номер устройства
        SEND_SERIAL_NUMBER();
        break;
    case 'C':   //Переводит CAN в режим сброса
        RESET_CAN();
        break;
    case 'S':   //Установка скорости CAN
        SET_CAN_SPEED( command[ 1 ] );
        break;
    case 'Z':   //Включает выключает метки времени вполучаемых сообщениях
        SET_CAN_TIMESTAMP( command[ 1 ] );
        break;
    case 'M':   //Устанавливает код фильтра (Доработать)
        usb_serial_jtag_write_bytes( &ANSWER_ERROR, 1, portMAX_DELAY );
        break;
    case 'm':   //Устанавливает маску фильтра (Доработать)
        usb_serial_jtag_write_bytes( &ANSWER_ERROR, 1, portMAX_DELAY );
        break; 
    case 'L':   //Запускает CAN в режиме только для чтения
        CAN_OPEN_READ_ONLY();
        break;
    case 'O':   //Запускает CAN
        CAN_OPEN_READ_WRITE();
        break;
    case 'r':   //Передача стандартного RTR фрейма
        CAN_SEND_MSG( command + 1, false, true );
        break;
    case 'R':   //Передача расширенного RTR фрейма
        CAN_SEND_MSG( command + 1, true, true );
        break;
    case 't':   //Передача стандартного CAN фрейма
        CAN_SEND_MSG( command + 1, false, false );
        break;
    case 'T':   //Передача расширенного CAN фрейма
        CAN_SEND_MSG( command + 1, true, false );
        break;
    default:
        break;
    }
}

void SEND_PCB_VERSION( void )
{
    usb_serial_jtag_write_bytes( "V", 1, portMAX_DELAY );
    usb_serial_jtag_write_bytes( PCB_VERSION, sizeof( PCB_VERSION ) - 1, portMAX_DELAY );
    usb_serial_jtag_write_bytes( FIRMWARE_VERSION_MA, sizeof( FIRMWARE_VERSION_MA ) - 1, portMAX_DELAY );
    usb_serial_jtag_write_bytes( "\r", 1, portMAX_DELAY ); 

}

void SEND_FIRMVARE_VERSION( void )
{
    usb_serial_jtag_write_bytes( "v", 1, portMAX_DELAY );
    usb_serial_jtag_write_bytes( FIRMWARE_VERSION_MA, sizeof( FIRMWARE_VERSION_MA ) - 1, portMAX_DELAY );
    usb_serial_jtag_write_bytes( FIRMWARE_VERSION_MI, sizeof( FIRMWARE_VERSION_MI ) - 1, portMAX_DELAY );
    usb_serial_jtag_write_bytes( "\r", 1, portMAX_DELAY ); 
}

void SEND_SERIAL_NUMBER( void )
{
    usb_serial_jtag_write_bytes( "N", 1, portMAX_DELAY );
    usb_serial_jtag_write_bytes( SERIAL_NUMBER, sizeof( SERIAL_NUMBER ) - 1, portMAX_DELAY );
    usb_serial_jtag_write_bytes( "\r", 1, portMAX_DELAY ); 
}

void RESET_CAN( void )
{
    if ( CAN_INTERFACE::RESET_CAN() == ESP_FAIL )
    {
        usb_serial_jtag_write_bytes( &ANSWER_ERROR, 1, portMAX_DELAY );
        return;
    }
    usb_serial_jtag_write_bytes( &ANSWER_OK, 1, portMAX_DELAY );
}

void SET_CAN_SPEED( char num_speed )
{    
    if (  num_speed < '0' && num_speed > '8' )
    {
        usb_serial_jtag_write_bytes( &ANSWER_ERROR, 1, portMAX_DELAY );
        return;
    }

    CAN_INTERFACE::SPEED_Enum speed = (CAN_INTERFACE::SPEED_Enum)( num_speed - '0' );

    if ( CAN_INTERFACE::SET_SPEED( speed ) == ESP_FAIL )
    {
        usb_serial_jtag_write_bytes( &ANSWER_ERROR, 1, portMAX_DELAY );
        return;
    }

    usb_serial_jtag_write_bytes( &ANSWER_OK, 1, portMAX_DELAY );
}

void SET_CAN_TIMESTAMP( char num_state )
{
    bool state;
    if ( num_state == '0' )
    {
        state = false;
    }
    else if ( num_state == '1' )
    {
        state = true;
    }
    else
    {
        usb_serial_jtag_write_bytes( &ANSWER_ERROR, 1, portMAX_DELAY );
        return;
    }

    if ( CAN_INTERFACE::SET_MODE_TIMESTAMP( state ) == ESP_FAIL )
    {
        usb_serial_jtag_write_bytes( &ANSWER_ERROR, 1, portMAX_DELAY );
        return;
    }
    usb_serial_jtag_write_bytes( &ANSWER_OK, 1, portMAX_DELAY );
}

void CAN_OPEN_READ_ONLY( void )
{
    if ( CAN_INTERFACE::CAN_OPEN( CAN_INTERFACE::MODE_READ_ONLY ) == ESP_FAIL )
    {
        usb_serial_jtag_write_bytes( &ANSWER_ERROR, 1, portMAX_DELAY );
        return;
    }
    usb_serial_jtag_write_bytes( &ANSWER_OK, 1, portMAX_DELAY );
}

void CAN_OPEN_READ_WRITE( void )
{
    if ( CAN_INTERFACE::CAN_OPEN( CAN_INTERFACE::MODE_READ_WRITE )  == ESP_FAIL )
    {
        usb_serial_jtag_write_bytes( &ANSWER_ERROR, 1, portMAX_DELAY );
        return;
    }
    usb_serial_jtag_write_bytes( &ANSWER_OK, 1, portMAX_DELAY );
}

void CAN_SEND_MSG( char* command, bool extd, bool rtr )
{
    twai_message_t message;
    message.extd = extd;
    message.rtr = rtr;

    message.identifier = 0;
    if ( READ_ID( command, &message.identifier, extd ) == ESP_FAIL )
    {
        usb_serial_jtag_write_bytes( &ANSWER_ERROR, 1, portMAX_DELAY );
        return;
    }
    command += extd ? 8 : 3;

    message.data_length_code = 0;
    if ( READ_LEN( command, &message.data_length_code ) == ESP_FAIL )
    {
        usb_serial_jtag_write_bytes( &ANSWER_ERROR, 1, portMAX_DELAY );
        return;
    }
    command++;

    if ( !rtr )
    {
         if ( READ_DATA( command, message.data_length_code, message.data ) == ESP_FAIL )
        {
            usb_serial_jtag_write_bytes( &ANSWER_ERROR, 1, portMAX_DELAY );
            return;
        }
    }

    if ( CAN_INTERFACE::SEND_MSG( &message ) == ESP_FAIL )
    {
        usb_serial_jtag_write_bytes( &ANSWER_ERROR, 1, portMAX_DELAY );
        return;
    }

    usb_serial_jtag_write_bytes( &ANSWER_OK, 1, portMAX_DELAY );
}

esp_err_t READ_ID( char* command, uint32_t* id, bool extd )
{
    int iterate;
    if ( extd )
    {
        iterate = 8;
    }
    else
    {
        iterate = 3;
    }
    for ( int i = 0; i < iterate; i++ )
    {
         if ( command[ i ] == '\0' )
        {
            return ESP_FAIL;
        }
        *id <<= 4;
        *id += HEX_TO_NIBBLE( command[ i ] );
    }

    return ESP_OK;
}

esp_err_t READ_LEN( char* command, uint8_t* len )
{
    if (  command[ 0 ] < '0' && command[ 0 ] > '8' )
    {
        return ESP_FAIL;
    }
    *len = command[ 0 ] - '0';
    
    return ESP_OK;
}

esp_err_t READ_DATA( char* command, const uint8_t len, uint8_t* data )
{
    for ( int i = 0; i < len; i++ )
    {
        if ( command[ 0 ] == '\0' || command[ 1 ] == '\0' )
        {
            return ESP_FAIL;
        }

        data[ i ] = HEX_TO_NIBBLE( command[ 0 ] );
        data[ i ] <<= 4;
        data[ i ] += HEX_TO_NIBBLE( command[ 1 ] );
        command += 2;
    }

    return ESP_OK;
}
