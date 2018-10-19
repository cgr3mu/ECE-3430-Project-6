#include "spi.h"
#define MSB_MASK 0x80

void InitializeSPI()
{
    TURN_OFF_SCK;
    TURN_OFF_MOSI;
    SET_MISO_AS_AN_INPUT;
    SET_MOSI_AS_AN_OUTPUT;
    SET_SCK_AS_AN_OUTPUT;
}

void SPISendByte(unsigned char SendValue)
{
    unsigned char SendValueCopy = SendValue;
    int x;
    for(x = 0 ; x<8; x++)
    {
        if(SendValueCopy & MSB_MASK)
            TURN_ON_MOSI;
        else
            TURN_OFF_MOSI;
        SendValueCopy = SendValueCopy << 1;
        TOGGLE_SCK;
        TOGGLE_SCK;
    }

}

unsigned char SPIReceiveByte()
{
	unsigned char ReceiveValue = 0;
	int x;
    for(x = 0 ; x<8; x++)
    {
        ReceiveValue = ReceiveValue << 1;
        ReceiveValue |= READ_MISO_PIN;
        TOGGLE_SCK;
        TOGGLE_SCK;
    }

	return ReceiveValue;
}

