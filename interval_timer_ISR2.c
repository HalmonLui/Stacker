extern volatile int timeout;
#include <stdio.h> //added by WB

/*****************************************************************************
 * Interval timer interrupt service routine
 *                                                                          
 * Controls refresh of the VGA screen
 * 
******************************************************************************/
void interval_timer_ISR( )
{
	volatile int * interval_timer_ptr = (int *) 0xFF202000; 
	// Base address. Change for DE0-CV 0xFF202000
	 
	//printf("\n entered ISR, timeout = %d", timeout);

	*(interval_timer_ptr) = 0; 			// clear the interrupt
	timeout = 1;						// set global variable
	//printf("\n leaving ISR, timeout = %d", timeout);

	return;
}

