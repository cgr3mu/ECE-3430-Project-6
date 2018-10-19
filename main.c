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
    volatile unsigned int ID_U3;
    volatile unsigned int ID_U2;

    // Stop the watchdog timer, and configure the clock module.
	WDTCTL = WDTPW + WDTHOLD;
    ConfigureClockModule();

    // Initialize port pins.
	InitializeLEDPortPins();
    InitializeSPI();
    InitializeSerialFlash();

	// Insert code to perform tests of SPI and/or serial Flash functionality.
    while(TRUE){
        ID_U3 = ReadFlashMemoryID(FLASH_MEMORY_U3);
        ID_U2 = ReadFlashMemoryID(FLASH_MEMORY_U2);
        _delay_cycles(1000000);
    }
}

void ConfigureClockModule()
{
	// Configure Digitally Controlled Oscillator (DCO) using factory calibrations
	DCOCTL  = CALDCO_16MHZ;
	BCSCTL1 = CALBC1_16MHZ;
}
