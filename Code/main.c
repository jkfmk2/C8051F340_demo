//-----------------------------------------------------------------------------
// F34x_Blinky.c
//-----------------------------------------------------------------------------
// Copyright 2007 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// This program flashes the P2.2 green LED on the C8051F34x target board
// five times a second using the interrupt handler for Timer2.
//
//
// How To Test:
//
// 1) Download code to a 'F34x target board
// 2) Run the code and if the P2.2 LED blinks, the code works
//
//
// FID:            34X000001
// Target:         C8051F34x
// Tool chain:     Keil C51 7.50 / Keil EVAL C51
// Command Line:   None
//
// Release 1.1
//    -27 SEP 2006 (GP)
//    -Fixed #define for SYSCLK from 24.5/8 Mhz to 12/8 Mhz.
//
// Release 1.0
//    -Initial Revision (GP)
//    -01 DEC 2005
//

#define __MAIN__

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include "Include.h"

//-----------------------------------------------------------------------------
// main() Routine
//-----------------------------------------------------------------------------
void main (void) 
{		
   PCA0MD &= ~0x40;                         // WDTE = 0 (clear watchdog timer 
                                            // enable)
	
   OSCILLATOR_Init();                      // Initialize system clock 
	Timer0_Init();
	Timer2_Init (TICK_COUNT_10MS);  // Init Timer2 to generate 
                                   // interrupts at a 100Hz rate.
	Timer3_Init();
   PORT_Init();                            // Initialize crossbar and GPIO
	UART0_Init();
	SMBus_Init();
   
   EA = 1;									// Enable global interrupts

	led_count_1sec = 100;

	printf("\nhello!!\n");
	
	send = 0xAA;
	printf("EEPROM Data Write : %bx\n", send);	
	HWI2CWrite(0xA0, 0x25, &send, 1);
	HWI2CRead(0xA0, 0x25, &read, 1);
   // Check that the data was read properly
   if (read != send)
   {
      error_flag = 1;
   }	
	printf("EEPROM Data Read : %bx\n", read);

   // Store the outgoing data buffer at EEPROM address 0x50
   HWI2CWrite(0xA0, 0x50, out_buff, sizeof(out_buff));

   // Fill the incoming data buffer with data starting at EEPROM address 0x50
   HWI2CRead(0xA0, 0x50, in_buff, sizeof(in_buff));
	printf("Read : %s\n", in_buff);
   // Check that the data that came from the EEPROM is the same as what was
   // sent
   for (i = 0; i < sizeof(in_buff); i++)
   {
      if (in_buff[i] != out_buff[i])
      {
         error_flag = 1;
      }
   }	
	
	FLASH_Write(USER_FLASH_ADDR, out_buff, sizeof(out_buff));
	printf("Flash Data Write : %s\n", out_buff);
	FLASH_Read(in_buff, USER_FLASH_ADDR, sizeof(in_buff));
	printf("Flash Data Read : %s\n", in_buff);	
	
	FLASH_Update(USER_FLASH_ADDR, out_buff2, sizeof(out_buff2));
	FLASH_Read(in_buff2, USER_FLASH_ADDR, sizeof(in_buff2));
	printf("2nd Flash Data Read : %s\n", in_buff2);

	
   while (1)  // Spin forever
	{
		if (led_count_1sec == 0)
		{
			LED1 = ~LED1;                       // Change state of LED
			//printf("1sec tick\n");
			led_count_1sec = 100;
		}
	}                             
}

//-----------------------------------------------------------------------------
// Initialization Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// OSCILLATOR_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function initializes the system clock to (12 Mhz / 8)
//
//-----------------------------------------------------------------------------
void OSCILLATOR_Init (void)
{
   OSCICN = 0x83;                      // Configure internal oscillator as 12MHz
   RSTSRC  = 0x04;                     // Enable missing clock detector
}

//-----------------------------------------------------------------------------
// PORT_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures the crossbar and GPIO ports.
//
// P2.2   digital   push-pull     LED1
// P0.4   digital   push-pull    UART TX
// P0.5   digital   open-drain   UART RX
//-----------------------------------------------------------------------------
void PORT_Init (void)
{
//-----------------------------------------------------------------------------
// XBR0
// BIT0 : URT0E, BIT1 : SPI0E, BIT2 : SMB0E, BIT3 : SYSCKE, BIT4 : CP0E, BIT5 : CP0AE, BIT6 : CP1E, BIT7 : CP1AE
//-----------------------------------------------------------------------------	
	XBR0 = _BIT0 | _BIT2;                    // Enable UART on P0.4(TX) and P0.5(RX)
														// Enable SMBus P0.0(SDA) and P0.1(SCL)			
//-----------------------------------------------------------------------------
// XBR1
// BIT0~BIT2 : PCA0ME, BIT3 : ECIE, BIT4 : T0E, BIT5 : T1E, BIT6 : XBARE, BIT7 : WEAKPUD (0 : enable; 1 : disable)
//-----------------------------------------------------------------------------
	XBR1 = _BIT6;                    // Enable crossbar and weak pull-ups
//-----------------------------------------------------------------------------
// PxMDIN
// 0 : analog input
// 1 : digital input	
//-----------------------------------------------------------------------------
	
//-----------------------------------------------------------------------------
// PxMDOUT
// 0 : open-drain
// 1 : push-pull	
//-----------------------------------------------------------------------------
	P0MDOUT = _BIT4;                    // Enable UTX as push-pull output
	P2MDOUT = _BIT2 | _BIT3;                    // Enable LED as a push-pull output
	
	LED1 = _LOW;
	LED2 = _LOW;
	SDA = _HIGH;
	SCL = _HIGH;
}

//-----------------------------------------------------------------------------
// Timer2_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : 
//   1)  int counts - calculated Timer overflow rate
//                    range is postive range of integer: 0 to 32767
//
// Configure Timer2 to 16-bit auto-reload and generate an interrupt at 
// interval specified by <counts> using SYSCLK/48 as its time base.
//
//-----------------------------------------------------------------------------
void Timer2_Init (int counts)
{
   TMR2CN  = 0x00;                     // Stop Timer2; Clear TF2;
                                       // Use SYSCLK/12 as timebase
   CKCON  &= ~0x30;                    // Timer2 clocked based on T2XCLK;

   TMR2RL  = -counts;                  // Init reload values
   TMR2    = 0xffff;                   // Set to reload immediately
   ET2     = 1;                        // Enable Timer2 interrupts
   TR2     = 1;                        // Start Timer2
}



//-----------------------------------------------------------------------------
// UART0_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Configure the UART0 using Timer1, for <BAUDRATE> and 8-N-1.
//-----------------------------------------------------------------------------
void UART0_Init (void)
{
   SCON0 = 0x10;                       // SCON0: 8-bit variable bit rate
                                       //        level of STOP bit is ignored
                                       //        RX enabled
                                       //        ninth bits are zeros
                                       //        clear RI0 and TI0 bits
   if (SYSCLK/BAUDRATE/2/256 < 1) {
      TH1 = -(SYSCLK/BAUDRATE/2);
      CKCON &= ~0x0B;                  // T1M = 1; SCA1:0 = xx
      CKCON |=  0x08;
   } else if (SYSCLK/BAUDRATE/2/256 < 4) {
      TH1 = -(SYSCLK/BAUDRATE/2/4);
      CKCON &= ~0x0B;                  // T1M = 0; SCA1:0 = 01                  
      CKCON |=  0x01;
   } else if (SYSCLK/BAUDRATE/2/256 < 12) {
      TH1 = -(SYSCLK/BAUDRATE/2/12);
      CKCON &= ~0x0B;                  // T1M = 0; SCA1:0 = 00
   } else {
      TH1 = -(SYSCLK/BAUDRATE/2/48);
      CKCON &= ~0x0B;                  // T1M = 0; SCA1:0 = 10
      CKCON |=  0x02;
   }

   TL1 = TH1;                          // Init Timer1
   TMOD &= ~0xf0;                      // TMOD: timer 1 in 8-bit autoreload
   TMOD |=  0x20;                       
   TR1 = 1;                            // START Timer1
   TI0 = 1;                            // Indicate TX0 ready
}


//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------