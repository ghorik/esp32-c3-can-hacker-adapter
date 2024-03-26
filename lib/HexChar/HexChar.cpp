
#include "HexChar.h"

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

uint8_t HEX_TO_NIBBLE( char hex )
{
    uint8_t res = 0;
    if ( hex >= '0' && hex <= '9' )
    {
        res = hex - '0';
    }
    else if ( hex >= 'A' && hex <= 'F' )
    {
        res = 10 + hex - 'A';
    }
    else if ( hex >= 'a' && hex <= 'f' )
    {
        res = 10 + hex - 'a';
    }

    return res;
}