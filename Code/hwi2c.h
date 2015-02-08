//-----------------------------------------------------------------------------
// hwi2c.h
// 1. Use timer0 overflow as I2C clock source
// 2. Use timer3 as timeout counter
//-----------------------------------------------------------------------------

sbit SDA = P0^0;                       // SMBus on P0.0
sbit SCL = P0^1;                       // and P0.1
#define SMB_FREQUENCY	50000

#define TIMER0_RELOAD			-(SYSCLK / SMB_FREQUENCY / 3)
#define TIMER0_RELOAD_HIGH		((TIMER0_RELOAD & 0xFF00) >> 8)
#define TIMER0_RELOAD_LOW		(TIMER0_RELOAD & 0x00FF)

// Status vector - top 4 bits only
#define  SMB_MTSTA      0xE0           // (MT) start transmitted
#define  SMB_MTDB       0xC0           // (MT) data byte transmitted
#define  SMB_MRDB       0x80           // (MR) data byte received
// End status vector definition

#define  WRITE          0x00           // SMBus WRITE command
#define  READ           0x01           // SMBus READ command

#ifdef	__HWI2C__

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------
unsigned char* pSMB_DATA_IN;           // Global pointer for SMBus data
                                       // All receive data is written here

unsigned char* pSMB_DATA_OUT;          // Global pointer for SMBus data.
                                       // All transmit data is read from here

unsigned char SMB_DATA_LEN;            // Global holder for number of bytes
                                       // to send or receive in the current
                                       // SMBus transfer.

unsigned char WORD_ADDR;               // Global holder for the EEPROM word
                                       // address that will be accessed in
                                       // the next transfer

unsigned char DEVICE_ADDR;                  // Target SMBus slave address

bit SMB_BUSY = 0;                      // Software flag to indicate when the
                                       // EEPROM_ByteRead() or
                                       // EEPROM_ByteWrite()
                                       // functions have claimed the SMBus

bit SMB_RW;                            // Software flag to indicate the
                                       // direction of the current transfer

bit SMB_SENDWORDADDR;                  // When set, this flag causes the ISR
                                       // to send the 8-bit <WORD_ADDR>
                                       // after sending the slave address.


bit SMB_RANDOMREAD;                    // When set, this flag causes the ISR
                                       // to send a START signal after sending
                                       // the word address.
                                       // For the 24LC02B EEPROM, a random read
                                       // (a read from a particular address in
                                       // memory) starts as a write then
                                       // changes to a read after the repeated
                                       // start is sent. The ISR handles this
                                       // switchover if the <SMB_RANDOMREAD>
                                       // bit is set.

bit SMB_ACKPOLL;                       // When set, this flag causes the ISR
                                       // to send a repeated START until the
                                       // slave has acknowledged its address

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------
void HWI2CWrite(unsigned char device, unsigned char addr, unsigned char *pByteArray, unsigned char length);
void HWI2CRead(unsigned char device, unsigned char addr, unsigned char *pByteArray, unsigned char length);
void SMBus_Init (void);
void Timer0_Init (void);
void Timer3_Init (void);

#else
//-----------------------------------------------------------------------------
// Extern Global Variables
//-----------------------------------------------------------------------------
extern unsigned char* pSMB_DATA_IN;
extern unsigned char* pSMB_DATA_OUT;
extern unsigned char SMB_DATA_LEN;
extern unsigned char WORD_ADDR;
extern unsigned char DEVICE_ADDR;
extern bit SMB_BUSY;
extern bit SMB_RW;
extern bit SMB_SENDWORDADDR;
extern bit SMB_RANDOMREAD;
extern bit SMB_ACKPOLL;

//-----------------------------------------------------------------------------
// Extern Global Functions
//-----------------------------------------------------------------------------
extern void HWI2CWrite(unsigned char device, unsigned char addr, unsigned char *pByteArray, unsigned char length);
extern void HWI2CRead(unsigned char device, unsigned char addr, unsigned char *pByteArray, unsigned char length);
extern void SMBus_Init (void);
extern void Timer0_Init (void);
extern void Timer3_Init (void);

#endif

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------