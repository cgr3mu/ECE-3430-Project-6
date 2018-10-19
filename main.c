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
    unsigned char sendInstruction = (FULL << 2) + (1 << 7);

    while (TRUE) {

    }
	// Insert code to perform tests of SPI and/or serial Flash functionality.
}

void ConfigureClockModule()
{
	// Configure Digitally Controlled Oscillator (DCO) using factory calibrations
	DCOCTL  = CALDCO_16MHZ;
	BCSCTL1 = CALBC1_16MHZ;
}
