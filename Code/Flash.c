//-----------------------------------------------------------------------------
// Flash.c
//-----------------------------------------------------------------------------

#define __FLASH__

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include "Include.h"

void FLASH_ByteWrite (FLADDR addr, BYTE byte)
{
	bit EA_SAVE = EA;
	BYTE xdata * data pwrite;
	
	pwrite = (BYTE xdata *)addr;
	
	EA = 0;  // Disable interrupt
	
	// Enable VDD monitor
	VDM0CN |= _BIT7;
	RSTSRC |= _BIT1;
	
	FLKEY = 0xA5;
	FLKEY = 0xF1;
	PSCTL |= _BIT0;	// PSWE = 1
	
	*pwrite = byte;
	
	PSCTL &= ~_BIT0;	// PSWE = 0
	
	// Disable VDD monitor
	VDM0CN &= ~_BIT7;
	RSTSRC &= ~_BIT1;
	
	EA = EA_SAVE;	// restore interrupt
}
	
BYTE FLASH_ByteRead (FLADDR addr)
{
	bit EA_SAVE = EA;                   // Preserve EA
   BYTE code * data pread;             // FLASH read pointer
   BYTE byte;

   EA = 0;                             // Disable interrupts

   pread = (BYTE code *) addr;

   byte = *pread;                      // Read the byte

   EA = EA_SAVE;                       // Restore interrupts

   return byte;	
}

void FLASH_PageErase (FLADDR addr)
{
	bit EA_SAVE = EA;
	BYTE xdata * data pwrite;
	
	pwrite = (BYTE xdata *)addr;
	
	EA = 0;  // Disable interrupt
	
	// Enable VDD monitor
	VDM0CN |= _BIT7;
	RSTSRC |= _BIT1;
	
	FLKEY = 0xA5;
	FLKEY = 0xF1;
	PSCTL |= (_BIT0 | _BIT1);	// PSWE = 1; PSEE = 1
	
	*pwrite = 0;
	
	PSCTL &= ~(_BIT0 | _BIT1);	// PSWE = 0; PSEE =0
	
	// Disable VDD monitor
	VDM0CN &= ~_BIT7;
	RSTSRC &= ~_BIT1;
	
	EA = EA_SAVE;	// restore interrupt	
}

BYTE * FLASH_Read (BYTE *dest, FLADDR src, unsigned int numbytes)
{
	FLADDR i;
	
	for (i = 0; i < numbytes; i++)
	{
		*dest++ = FLASH_ByteRead(src+i);
	}
	
	return dest;
}

void FLASH_Write (FLADDR dest, BYTE *src, unsigned int numbytes)
{
   FLADDR i;

   for (i = dest; i < dest+numbytes; i++) {
      FLASH_ByteWrite (i, *src++);
   }
}

void FLASH_Copy (FLADDR dest, FLADDR src, unsigned numbytes)
{
   FLADDR i;

   for (i = 0; i < numbytes; i++) {

      FLASH_ByteWrite ((FLADDR) dest+i, FLASH_ByteRead((FLADDR) src+i));
   }
}

void FLASH_Fill (FLADDR addr, DWORD length, BYTE fill)
{
   FLADDR i;

   for (i = 0; i < length; i++) {
      FLASH_ByteWrite (addr+i, fill);
   }
}

void FLASH_Clear (FLADDR dest, unsigned numbytes)
{
   FLADDR dest_1_page_start;           // First address in 1st page
                                       // containing <dest>
   FLADDR dest_1_page_end;             // Last address in 1st page
                                       // containing <dest>
   FLADDR dest_2_page_start;           // First address in 2nd page
                                       // containing <dest>
   FLADDR dest_2_page_end;             // Last address in 2nd page
                                       // containing <dest>
   unsigned numbytes_remainder;        // When crossing page boundary,
                                       // number of <src> bytes on 2nd page
   unsigned FLASH_pagesize;            // Size of FLASH page to update

   FLADDR  wptr;                       // Write address
   FLADDR  rptr;                       // Read address

   unsigned length;

   FLASH_pagesize = FLASH_PAGESIZE;

   dest_1_page_start = dest & ~(FLASH_pagesize - 1);
   dest_1_page_end = dest_1_page_start + FLASH_pagesize - 1;
   dest_2_page_start = (dest + numbytes)  & ~(FLASH_pagesize - 1);
   dest_2_page_end = dest_2_page_start + FLASH_pagesize - 1;

   if (dest_1_page_end == dest_2_page_end) {

      // 1. Erase Scratch page
      FLASH_PageErase (FLASH_TEMP);

      // 2. Copy bytes from first byte of dest page to dest-1 to Scratch page

      wptr = FLASH_TEMP;
      rptr = dest_1_page_start;
      length = dest - dest_1_page_start;
      FLASH_Copy (wptr, rptr, length);

      // 3. Copy from (dest+numbytes) to dest_page_end to Scratch page

      wptr = FLASH_TEMP + dest - dest_1_page_start + numbytes;
      rptr = dest + numbytes;
      length = dest_1_page_end - dest - numbytes + 1;
      FLASH_Copy (wptr, rptr, length);

      // 4. Erase destination page
      FLASH_PageErase (dest_1_page_start);

      // 5. Copy Scratch page to destination page
      wptr = dest_1_page_start;
      rptr = FLASH_TEMP;
      length = FLASH_pagesize;
      FLASH_Copy (wptr, rptr, length);

   } else {                            // Value crosses page boundary
      // 1. Erase Scratch page
      FLASH_PageErase (FLASH_TEMP);

      // 2. Copy bytes from first byte of dest page to dest-1 
	  //    to Scratch page

      wptr = FLASH_TEMP;
      rptr = dest_1_page_start;
      length = dest - dest_1_page_start;
      FLASH_Copy (wptr, rptr, length);

      // 3. Erase destination page 1
      FLASH_PageErase (dest_1_page_start);

      // 4. Copy Scratch page to destination page 1
      wptr = dest_1_page_start;
      rptr = FLASH_TEMP;
      length = FLASH_pagesize;
      FLASH_Copy (wptr, rptr, length);

      // now handle 2nd page

      // 5. Erase Scratch page
      FLASH_PageErase (FLASH_TEMP);

      // 6. Copy bytes from numbytes remaining to dest-2_page_end 
	  //    to Scratch page

      numbytes_remainder = numbytes - (dest_1_page_end - dest + 1);
      wptr = FLASH_TEMP + numbytes_remainder;
      rptr = dest_2_page_start + numbytes_remainder;
      length = FLASH_pagesize - numbytes_remainder;
      FLASH_Copy (wptr, rptr, length);

      // 7. Erase destination page 2
      FLASH_PageErase (dest_2_page_start);

      // 8. Copy Scratch page to destination page 2
      wptr = dest_2_page_start;
      rptr = FLASH_TEMP;
      length = FLASH_pagesize;
      FLASH_Copy (wptr, rptr, length);
   }
}

void FLASH_Update (FLADDR dest, BYTE *src, unsigned numbytes)
{
   // 1. Erase <numbytes> starting from <dest>
   FLASH_Clear (dest, numbytes);

   // 2. Write <numbytes> from <src> to <dest>
   FLASH_Write (dest, src, numbytes);
}

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------