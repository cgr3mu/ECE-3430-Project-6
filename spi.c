#include "spi.h"
#define MSB 0x80
#define READ_PORT_PIN(PORT,BIT,PIN) ((PORT & BIT) >> PIN) // Returns port pin value of 0 or 1.

void InitializeSPI()
{
    SET_MISO_AS_AN_INPUT;

    TURN_OFF_MOSI;
    SET_MOSI_AS_AN_OUTPUT;

    TURN_OFF_SCK;
    SET_SCK_AS_AN_OUTPUT;
}

void SPISendByte(unsigned char SendValue)
{
    unsigned int localSendValue = SendValue;
    int isMSB, i;
    for(i = 0; i<8; i++){
        isMSB = localSendValue & MSB;
        if(isMSB){
            TURN_ON_MOSI;
        }
        else{
            TURN_OFF_MOSI;
        }
        localSendValue = localSendValue << 1;
        TOGGLE_SCK;
        TOGGLE_SCK;
    }
}

unsigned char SPIReceiveByte()
{
	unsigned char ReceiveValue = 0;
	int i = 0;
	 for(i = 0; i<8; i++){
	        ReceiveValue = ReceiveValue << 1;
	        ReceiveValue |= READ_PORT_PIN(USCIB0_MISO_PORT, USCIB0_MISO_BIT, USCIB0_MISO_PIN);
            TOGGLE_SCK;
            TOGGLE_SCK;
	        }
	return ReceiveValue;
}

