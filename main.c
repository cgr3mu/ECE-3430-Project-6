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
        SetBlockProtection(NONE, FLASH_MEMORY_U3);
        DISABLE_WRITE_PROTECT;
        ChipEraseFlashMemory(FLASH_MEMORY_U3);
        ReadFlashMemory(0x5555, Array, 2, FLASH_MEMORY_U3, 0);
        x = Array[0];
        y = Array[1];
        ByteProgramFlashMemory(0x5555, 0xBB, FLASH_MEMORY_U3);
        ByteProgramFlashMemory(0x5556, 0xCC, FLASH_MEMORY_U3);
        _delay_cycles(1000000);
        ReadFlashMemory(0x5555, Array, 2, FLASH_MEMORY_U3, 0);
        x = Array[0];
        y = Array[1];
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
