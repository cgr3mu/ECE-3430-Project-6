#include <msp430.h>
#include "LED.h"
#include "spi.h"
#include "serial_flash.h"

// Local prototypes
void ConfigureClockModule();

// Use for read/write flash memory tests.
#define TRUE 1
#define FALSE 0

void main(void)
{
    // Stop the watchdog timer, and configure the clock module.
	WDTCTL = WDTPW + WDTHOLD;
    ConfigureClockModule();

    // Initialize port pins.
	InitializeLEDPortPins();
    InitializeSPI();
    InitializeSerialFlash();

    volatile unsigned char STAT_U3;
    volatile unsigned char STAT_U2;


    unsigned char Array[2];
    volatile unsigned char x, y;

    //volatile unsigned char ST = ReadFlashMemoryStatusRegister(FLASH_MEMORY_U3);

    while (TRUE) {
        SetBlockProtection(NONE, FLASH_MEMORY_U2);
        DISABLE_WRITE_PROTECT;
        ByteProgramFlashMemory(0x5000, 0xAA, FLASH_MEMORY_U2);
        ByteProgramFlashMemory(0x5001, 0xAA, FLASH_MEMORY_U2);
        ByteProgramFlashMemory(0x6000, 0xAA, FLASH_MEMORY_U2);

        ReadFlashMemory(0x5000, Array, 1, FLASH_MEMORY_U2, 0);
        x = Array[0];
        _delay_cycles(1000000);
        ReadFlashMemory(0x6000, Array, 1, FLASH_MEMORY_U2, 0);
        x = Array[0];
        _delay_cycles(1000000);

        SectorBlockEraseFlashMemory(0x5000, FLASH_MEMORY_U2,0);

        ReadFlashMemory(0x5000, Array, 1, FLASH_MEMORY_U2, 0);
        x = Array[0];
        _delay_cycles(1000000);
        ReadFlashMemory(0x6000, Array, 1, FLASH_MEMORY_U2, 0);
        x = Array[0];
        _delay_cycles(1000000);
    }
	// Insert code to perform tests of SPI and/or serial Flash functionality.
}

void ConfigureClockModule()
{
	// Configure Digitally Controlled Oscillator (DCO) using factory calibrations
	DCOCTL  = CALDCO_16MHZ;
	BCSCTL1 = CALBC1_16MHZ;
}
