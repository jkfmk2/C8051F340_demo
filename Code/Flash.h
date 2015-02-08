//-----------------------------------------------------------------------------
// Global.h
//-----------------------------------------------------------------------------


#ifdef	__FLASH__

#define FLASH_PAGESIZE 512
#define FLASH_TEMP 0x0F800L
#define FLASH_LAST 0x0FA00L

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------
void FLASH_ByteWrite (FLADDR addr, BYTE byte);
BYTE FLASH_ByteRead (FLADDR addr);
void FLASH_PageErase (FLADDR addr);

BYTE * FLASH_Read (BYTE *dest, FLADDR src, unsigned int numbytes);
void FLASH_Write (FLADDR dest, BYTE *src, unsigned int numbytes);
void FLASH_Copy (FLADDR dest, FLADDR src, unsigned numbytes);
void FLASH_Fill (FLADDR addr, DWORD length, BYTE fill);
void FLASH_Clear (FLADDR dest, unsigned numbytes);
void FLASH_Update (FLADDR dest, BYTE *src, unsigned numbytes);

#else
//-----------------------------------------------------------------------------
// Extern Global Variables
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Extern Global Functions
//-----------------------------------------------------------------------------
extern void FLASH_ByteWrite (FLADDR addr, BYTE byte);
extern BYTE FLASH_ByteRead (FLADDR addr);
extern void FLASH_PageErase (FLADDR addr);

extern BYTE * FLASH_Read (BYTE *dest, FLADDR src, unsigned int numbytes);
extern void FLASH_Write (FLADDR dest, BYTE *src, unsigned int numbytes);
extern void FLASH_Copy (FLADDR dest, FLADDR src, unsigned numbytes);
extern void FLASH_Fill (FLADDR addr, DWORD length, BYTE fill);
extern void FLASH_Clear (FLADDR dest, unsigned numbytes);
extern void FLASH_Update (FLADDR dest, BYTE *src, unsigned numbytes);
#endif

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------