//-----------------------------------------------------------------------------
// Int.c
// Interrupt Service Routines
//-----------------------------------------------------------------------------

#include "include.h"



//-----------------------------------------------------------------------------
// Timer2_ISR
//-----------------------------------------------------------------------------
//
// This routine changes the state of the LED whenever Timer2 overflows.
//
//-----------------------------------------------------------------------------
void Timer2_ISR (void) interrupt INTERRUPT_TIMER2
{
   TF2H = 0;                           // Clear Timer2 interrupt flag

	if (led_count_1sec)
		led_count_1sec--;
}



//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------