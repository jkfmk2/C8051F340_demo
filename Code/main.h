//-----------------------------------------------------------------------------
// main.h
//-----------------------------------------------------------------------------



#define SYSCLK       	12000000         // SYSCLK frequency in Hz
#define TICK_COUNT_10MS	(SYSCLK / 12 / 100) 
#define BAUDRATE        9600           // Baud rate of UART in bps
#define SMB_FREQUENCY	50000



#define  EEPROM_ADDR    0xA0           // Device address for slave target
#define	USER_FLASH_ADDR	0x0F000
#define	USER_FLASH_ADDR_2	(USER_FLASH_ADDR + 512)

//-----------------------------------------------------------------------------
// 16-bit SFR Definitions for 'F34x
//-----------------------------------------------------------------------------
sfr16 TMR2RL   = 0xca;              // Timer2 reload registers
sfr16 TMR2     = 0xcc;              // Timer2 counter registers
sfr16 TMR3RL   = 0x92;              // Timer3 reload registers
sfr16 TMR3     = 0x94;              // Timer3 counter registers

sbit LED1 = P2^2;                      // LED='1' means ON
sbit LED2 = P2^3;                      // LED='1' means ON

#ifdef	__MAIN__

char in_buff[8] = {0};              // Incoming data buffer
char out_buff[8] = "ABCDEFG";       // Outgoing data buffer
char in_buff2[14] = {0};
char out_buff2[14] = "Update Flash!";

char send;            // Temporary variable
char read;            // Temporary variable
bit error_flag = 0;                 // Flag for checking EEPROM contents
unsigned char i;                    // Temporary counter variable

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------

int data led_count_1sec = 0;

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------
void OSCILLATOR_Init (void);
void PORT_Init (void);
void Timer2_Init (int counts);
void UART0_Init (void);


#else
//-----------------------------------------------------------------------------
// Extern Global Variables
//-----------------------------------------------------------------------------

extern data led_count_1sec;

#endif

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------