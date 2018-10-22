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


    unsigned char * Array = {0};
    volatile unsigned char x;

    while (TRUE) {
        ReadFlashMemory(0x08000, Array,2, FLASH_MEMORY_U3, 0);
        x = Array[0];
        ChipEraseFlashMemory(FLASH_MEMORY_U3);
        ReadFlashMemory(0x08000, Array,2, FLASH_MEMORY_U3, 0);
        x = Array[0];
        ByteProgramFlashMemory(0x08000, 0xBC, FLASH_MEMORY_U3);

    }
	// Insert code to perform tests of SPI and/or serial Flash functionality.
}

void ConfigureClockModule()
{
	// Configure Digitally Controlled Oscillator (DCO) using factory calibrations
	DCOCTL  = CALDCO_16MHZ;
	BCSCTL1 = CALBC1_16MHZ;
}
