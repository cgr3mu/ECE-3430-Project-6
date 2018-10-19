#include "serial_flash.h"

void InitializeSerialFlash()
{
    SET_U3_AS_AN_OUTPUT;
    DISABLE_FLASH_MEMORY_U3;

    SET_U2_AS_AN_OUTPUT;
    DISABLE_FLASH_MEMORY_U2;

    SET_HOLD_U3_AS_AN_OUTPUT;
    TURN_OFF_HOLD_U3;

    SET_HOLD_U2_AS_AN_OUTPUT;
    TURN_OFF_HOLD_U2;

    SET_WRITE_PROTECT;
    TURN_OFF_WRITE_PROTECT;
}

unsigned int ReadFlashMemoryID(unsigned char ComponentNumber)
{
   unsigned int ReturnValue = 0;
   unsigned char ID[2] = {0x00, 0x00};
   // First enable the appropriate device
   // \ and # indicate active low
   if(ComponentNumber == FLASH_MEMORY_U2){
       ENABLE_FLASH_MEMORY_U2;
   }
   else{
       ENABLE_FLASH_MEMORY_U3;
   }
   // Now, send the command.
   SPISendByte(READ_ID);

  //Then, send the address
   SPISendByte(0x00);SPISendByte(0x00);SPISendByte(0x00);

   // Now receive the device ID
   ID[0] = SPIReceiveByte();
   ID[1] = SPIReceiveByte();

   // Finally disable the appropriate device
   if(ComponentNumber == FLASH_MEMORY_U2){
       DISABLE_FLASH_MEMORY_U2;
   }
   else{
       DISABLE_FLASH_MEMORY_U3;
   }
   ReturnValue = ((unsigned int)ID[0]<<8) + (unsigned int)ID[1];
   return ReturnValue;
}

unsigned char ReadFlashMemoryStatusRegister(unsigned char ComponentNumber)
{
	unsigned char RegisterValue = 0;


	return RegisterValue;
}

void WriteFlashMemoryStatusRegister(unsigned char WriteValue,unsigned char ComponentNumber)
{

}

void ReadFlashMemory(unsigned long StartAddress, unsigned char* DataValuesArray,
unsigned int NumberOfDataValues, unsigned char ComponentNumber, unsigned char ReadMode)
{

}

void ByteProgramFlashMemory(unsigned long MemoryAddress, unsigned char WriteValue, 
unsigned char ComponentNumber)
{

}

void AAIProgramFlashMemory(unsigned long StartAddress, unsigned char* DataValuesArray,
unsigned int NumberOfDataValues, unsigned char ComponentNumber)
{

}

void ChipEraseFlashMemory(unsigned char ComponentNumber)
{

}

void SectorBlockEraseFlashMemory(unsigned long StartAddress, unsigned char ComponentNumber, 
unsigned char EraseMode)
{

}

void SetBlockProtection(unsigned char ProtectionLevel, unsigned char ComponentNumber)
{

}

unsigned char FlashMemoryBusy(unsigned char ComponentNumber)
{
    unsigned char Busy = 0;


    return Busy;
}
