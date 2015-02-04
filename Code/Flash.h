//-----------------------------------------------------------------------------
// Global.h
//-----------------------------------------------------------------------------


#ifdef	__FLASH__

#define FLASH_PAGESIZE 512
#define FLASH_TEMP 0x0F800L
#define FLASH_LAST 0x0FA00L

void FLASH_ByteWrite (WORD addr, BYTE byte);
BYTE FLASH_ByteRead (WORD addr);
void FLASH_PageErase (WORD addr);

BYTE * FLASH_Read (BYTE *dest, WORD src, unsigned int numbytes);


#else
extern void FLASH_ByteWrite (WORD addr, BYTE byte);
extern BYTE FLASH_ByteRead (WORD addr);
extern void FLASH_PageErase (WORD addr);

extern BYTE * FLASH_Read (BYTE *dest, WORD src, unsigned int numbytes);

#endif

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------