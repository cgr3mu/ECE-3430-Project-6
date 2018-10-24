#include "serial_flash.h"

void InitializeSerialFlash()
{
    SET_CHIP_ENABLE_U3_AS_OUTPUT;
    DISABLE_FLASH_MEMORY_U3;
    ENABLE_U3_HOLD_AS_OUTPUT;
    DISABLE_U3_HOLD;
    SET_CHIP_ENABLE_U2_AS_OUTPUT;
    DISABLE_FLASH_MEMORY_U2;
    ENABLE_U2_HOLD_AS_OUTPUT;
    DISABLE_U2_HOLD;
    ENABLE_WRITE_PROTECT_AS_OUTPUT;
}

unsigned int ReadFlashMemoryID(unsigned char ComponentNumber)
{
   unsigned int ReturnValue = 0;
   unsigned char ID[2] = {0x00,0x00};

   //First enable appropriate device
   if(ComponentNumber == FLASH_MEMORY_U3)
       ENABLE_FLASH_MEMORY_U3;
   else
       ENABLE_FLASH_MEMORY_U2;

   //Now, send the command
   SPISendByte(READ_ID);

   //Send the address
   SPISendByte(0x00);
   SPISendByte(0x00);
   SPISendByte(0x00);

   //read the device ID
   ID[0] = SPIReceiveByte();
   ID[1] = SPIReceiveByte();

   //Finally disable appropriate device
   if(ComponentNumber == FLASH_MEMORY_U3)
          DISABLE_FLASH_MEMORY_U3;
      else
          DISABLE_FLASH_MEMORY_U2;

   ReturnValue = (((unsigned int)ID[0])<<8) + ((unsigned int)ID[1]) ;
   return ReturnValue;
}

unsigned char ReadFlashMemoryStatusRegister(unsigned char ComponentNumber)
{
	unsigned char RegisterValue = 0;

    //First enable appropriate device
    if(ComponentNumber == FLASH_MEMORY_U3)
        ENABLE_FLASH_MEMORY_U3;
    else
        ENABLE_FLASH_MEMORY_U2;

    //Now, send the command
    SPISendByte(RDSR);

    //read the device status
    RegisterValue = SPIReceiveByte();

    //Finally disable appropriate device
    if(ComponentNumber == FLASH_MEMORY_U3)
        DISABLE_FLASH_MEMORY_U3;
    else
        DISABLE_FLASH_MEMORY_U2;

	return RegisterValue;
}

void WriteFlashMemoryStatusRegister(unsigned char WriteValue,unsigned char ComponentNumber)
{
    DISABLE_WRITE_PROTECT;

    //First enable appropriate device
    if(ComponentNumber == FLASH_MEMORY_U3)
        ENABLE_FLASH_MEMORY_U3;
    else
        ENABLE_FLASH_MEMORY_U2;

    //Now, send the EWSR command to enable writing to the status register
    SPISendByte(EWSR);

    //Disable the appropriate device to actually cause the RDSR command to execute
    if(ComponentNumber == FLASH_MEMORY_U3)
        DISABLE_FLASH_MEMORY_U3;
    else
        DISABLE_FLASH_MEMORY_U2;

    //re-enable appropriate device
    if(ComponentNumber == FLASH_MEMORY_U3)
        ENABLE_FLASH_MEMORY_U3;
    else
        ENABLE_FLASH_MEMORY_U2;

    //Send WRSR instruction followed by the value to be written
    SPISendByte(WRSR);
    SPISendByte(WriteValue);

    //Disable the appropriate device
    if(ComponentNumber == FLASH_MEMORY_U3)
        DISABLE_FLASH_MEMORY_U3;
    else
        DISABLE_FLASH_MEMORY_U2;

    ENABLE_WRITE_PROTECT;

}

//readmode is used to enable high speed read, but we can't use that with the MSP
void ReadFlashMemory(unsigned long StartAddress, unsigned char* DataValuesArray,
unsigned int NumberOfDataValues, unsigned char ComponentNumber, unsigned char ReadMode)
{
    //First enable appropriate device
    if(ComponentNumber == FLASH_MEMORY_U3)
        ENABLE_FLASH_MEMORY_U3;
    else
        ENABLE_FLASH_MEMORY_U2;

    //check which read mode we should be using
    if(ReadMode)
        SPISendByte(READ);
    else
        SPISendByte(HIGH_SPEED_READ);

    //rip the Start Address into three bytes, send each byte to the flash
    unsigned char firstByte =(unsigned char)( (StartAddress & 0xFF0000) >> 16);
    unsigned char secondByte =(unsigned char)( (StartAddress & 0x00FF00) >> 8);
    unsigned char thirdByte =(unsigned char)( (StartAddress & 0x0000FF));
    SPISendByte(firstByte);
    SPISendByte(secondByte);
    SPISendByte(thirdByte);

    int x;
    //for each data value we should be reading
    for(x = 0; x<NumberOfDataValues; x++)
    {
        DataValuesArray[x] = SPIReceiveByte();
    }

    if(ComponentNumber == FLASH_MEMORY_U3)
        DISABLE_FLASH_MEMORY_U3;
    else
        DISABLE_FLASH_MEMORY_U2;

    return;
}


void ByteProgramFlashMemory(unsigned long MemoryAddress, unsigned char WriteValue, 
unsigned char ComponentNumber)
{
    //Enable the flash
    if(ComponentNumber == FLASH_MEMORY_U3)
        ENABLE_FLASH_MEMORY_U3;
    else
        ENABLE_FLASH_MEMORY_U2;

    //Save the old status of Flash to restore it later
    unsigned char oldStatus =  ReadFlashMemoryStatusRegister(ComponentNumber);

    SetBlockProtection(NONE,ComponentNumber);
    DISABLE_WRITE_PROTECT;


    //send instruction
    SPISendByte(BYTE_PROGRAM);

    //send address to program
    unsigned char firstByte =(unsigned char)( (MemoryAddress & 0xFF0000) >> 16);
    unsigned char secondByte =(unsigned char)( (MemoryAddress & 0x00FF00) >> 8);
    unsigned char thirdByte =(unsigned char)( (MemoryAddress & 0x0000FF));
    SPISendByte(firstByte);
    SPISendByte(secondByte);
    SPISendByte(thirdByte);

    //Send the byte to be written to the flash.
    SPISendByte(WriteValue);

    //Disable the appropriate device
    if(ComponentNumber == FLASH_MEMORY_U3)
        DISABLE_FLASH_MEMORY_U3;
    else
        DISABLE_FLASH_MEMORY_U2;

    //wait until flash is done programming itself
    while(FlashMemoryBusy(ComponentNumber));

    SetBlockProtection(oldStatus,ComponentNumber);
    ENABLE_WRITE_PROTECT;

    return;

}

//Connor Roos
void AAIProgramFlashMemory(unsigned long StartAddress, unsigned char* DataValuesArray,
unsigned int NumberOfDataValues, unsigned char ComponentNumber)
{
    DISABLE_WRITE_PROTECT;

    //Enable the appropriate device
    if(ComponentNumber == FLASH_MEMORY_U3)
        ENABLE_FLASH_MEMORY_U3;
    else
        ENABLE_FLASH_MEMORY_U2;

    SPISendByte(WREN);
    SPISendByte(AAI_PROGRAM);

    unsigned char Address1 = (unsigned char) (StartAddress >> 16);
    unsigned char Address2 = (unsigned char) (StartAddress >> 8);
    unsigned char Address3 = (unsigned char) (StartAddress);

    SPISendByte(Address1);
    SPISendByte(Address2);
    SPISendByte(Address3);

    int i = 0;
    for(i = 0; i <NumberOfDataValues; i ++)
        SPISendByte(DataValuesArray[i]);

    //Disable the appropriate device
    if(ComponentNumber == FLASH_MEMORY_U3)
        DISABLE_FLASH_MEMORY_U3;
    else
        DISABLE_FLASH_MEMORY_U2;

    ENABLE_WRITE_PROTECT;
}

void ChipEraseFlashMemory(unsigned char ComponentNumber)
{
    //Enable the flash
    if(ComponentNumber == FLASH_MEMORY_U3)
        ENABLE_FLASH_MEMORY_U3;
    else
        ENABLE_FLASH_MEMORY_U2;

    //Save the old status of Flash to restore it later
    unsigned char oldStatus =  ReadFlashMemoryStatusRegister(ComponentNumber);

    SetBlockProtection(NONE,ComponentNumber);
    DISABLE_WRITE_PROTECT;

    SPISendByte(CHIP_ERASE);

    //Disable the appropriate device
    if(ComponentNumber == FLASH_MEMORY_U3)
        DISABLE_FLASH_MEMORY_U3;
    else
        DISABLE_FLASH_MEMORY_U2;

    //wait until flash is done programming itself
    while(FlashMemoryBusy(ComponentNumber));

    SetBlockProtection(oldStatus,ComponentNumber);
    ENABLE_WRITE_PROTECT;
}

void SectorBlockEraseFlashMemory(unsigned long StartAddress, unsigned char ComponentNumber, 
unsigned char EraseMode)
{
    DISABLE_WRITE_PROTECT;

    //Enable the appropriate device
    if(ComponentNumber == FLASH_MEMORY_U3)
        ENABLE_FLASH_MEMORY_U3;
    else
        ENABLE_FLASH_MEMORY_U2;
    
    SPISendByte(WREN);

    SPISendByte(SECTOR_ERASE);

    unsigned char Address1 = (unsigned char) (StartAddress >> 16);
    unsigned char Address2 = (unsigned char) (StartAddress >> 8);
    unsigned char Address3 = (unsigned char) (StartAddress);

    SPISendByte(Address1);
    SPISendByte(Address2);
    SPISendByte(Address3);

    //Disable the appropriate device
    if(ComponentNumber == FLASH_MEMORY_U3)
        DISABLE_FLASH_MEMORY_U3;
    else
        DISABLE_FLASH_MEMORY_U2;

    ENABLE_WRITE_PROTECT;

    //wait until flash is done programming itself
    while(FlashMemoryBusy(ComponentNumber));
}

void SetBlockProtection(unsigned char ProtectionLevel, unsigned char ComponentNumber)
{
    unsigned char sendInstruction = (ProtectionLevel << 2);
    WriteFlashMemoryStatusRegister(sendInstruction,ComponentNumber);

}

unsigned char FlashMemoryBusy(unsigned char ComponentNumber)
{
    //receive status from the component
    unsigned char status = ReadFlashMemoryStatusRegister(ComponentNumber);
    //logical and it with 0x01 to just get the last bit
    const unsigned char Busy = status & 0x01;


    return Busy;
}
