
#ifndef __HEX_CHAR_H
#define __HEX_CHAR_H

#include <esp_types.h>

char NIBBLE_TO_HEX( uint8_t nibble );
uint8_t HEX_TO_NIBBLE( char hex );

#endif