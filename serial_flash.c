#include "serial_flash.h"

void InitializeSerialFlash()
{

}

unsigned int ReadFlashMemoryID(unsigned char ComponentNumber)
{
   unsigned int ReturnValue = 0;


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
