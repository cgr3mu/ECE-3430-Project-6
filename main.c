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


    unsigned char Array[1];
    volatile unsigned char x;

    //volatile unsigned char ST = ReadFlashMemoryStatusRegister(FLASH_MEMORY_U3);

    while (TRUE) {
        ReadFlashMemory(0x05555, Array,1, FLASH_MEMORY_U3, 0);
        x = Array[0];

        //Save the old status of Flash to restore it later
        unsigned char oldStatus =  ReadFlashMemoryStatusRegister(FLASH_MEMORY_U3);

        //save the old level of protection to restore it after this
        SetBlockProtection(NONE,FLASH_MEMORY_U3);
        DISABLE_WRITE_PROTECT;

        ChipEraseFlashMemory(FLASH_MEMORY_U3);


        //restore old level of protection
        SetBlockProtection(oldStatus,FLASH_MEMORY_U3);
        ENABLE_WRITE_PROTECT;

        ReadFlashMemory(0x05555, Array,1, FLASH_MEMORY_U3, 0);
        x = Array[0];


        //Save the old status of Flash to restore it later
        oldStatus =  ReadFlashMemoryStatusRegister(FLASH_MEMORY_U3);

        SetBlockProtection(NONE,FLASH_MEMORY_U3);
        DISABLE_WRITE_PROTECT;


        ByteProgramFlashMemory(0x05555, 0xBC, FLASH_MEMORY_U3);


        SetBlockProtection(oldStatus,FLASH_MEMORY_U3);
        ENABLE_WRITE_PROTECT;

    }
	// Insert code to perform tests of SPI and/or serial Flash functionality.
}

void ConfigureClockModule()
{
	// Configure Digitally Controlled Oscillator (DCO) using factory calibrations
	DCOCTL  = CALDCO_16MHZ;
	BCSCTL1 = CALBC1_16MHZ;
}
