//-----------------------------------------------------------------------------
// Flash.c
//-----------------------------------------------------------------------------

#define __FLASH__

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include "Include.h"

void FLASH_ByteWrite (WORD addr, BYTE byte)
{
	bit EA_SAVE = EA;
	WORD xdata * data pwrite;
	
	pwrite = (WORD xdata *)addr;
	
	EA = 0;  // Disable interrupt
	
	// Enable VDD monitor
	VDM0CN |= _BIT7;
	RSTSRC |= _BIT1;
	
	FLKEY = 0xA5;
	FLKEY = 0xF1;
	PSCTL |= _BIT0;	// PSWE = 1
	
	*pwrite = byte;
	
	PSCTL &= _BIT0;	// PSWE = 0
	
	// Disable VDD monitor
	VDM0CN &= ~_BIT7;
	RSTSRC &= _BIT1;
	
	EA = EA_SAVE;	// restore interrupt
}
	
BYTE FLASH_ByteRead (WORD addr)
{
	bit EA_SAVE = EA;                   // Preserve EA
   WORD code * data pread;             // FLASH read pointer
   BYTE byte;

   EA = 0;                             // Disable interrupts

   pread = (WORD code *) addr;

   byte = *pread;                      // Read the byte

   EA = EA_SAVE;                       // Restore interrupts

   return byte;
}

void FLASH_PageErase (WORD addr)
{
	bit EA_SAVE = EA;
	WORD xdata * data pwrite;
	
	pwrite = (WORD xdata *)addr;
	
	EA = 0;  // Disable interrupt
	
	// Enable VDD monitor
	VDM0CN |= _BIT7;
	RSTSRC |= _BIT1;
	
	FLKEY = 0xA5;
	FLKEY = 0xF1;
	PSCTL |= (_BIT0 | _BIT1);	// PSWE = 1; PSEE = 1
	
	*pwrite = 0;
	
	PSCTL &= (_BIT0 | _BIT1);	// PSWE = 0; PSEE =0
	
	// Disable VDD monitor
	VDM0CN &= ~_BIT7;
	RSTSRC &= _BIT1;
	
	EA = EA_SAVE;	// restore interrupt	
}

BYTE * FLASH_Read (BYTE *dest, WORD src, unsigned int numbytes)
{
	WORD i;
	
	for (i = 0; i < numbytes; i++)
	{
		*dest++ = FLASH_ByteRead(src+i);
	}
	
	return dest;
}

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------