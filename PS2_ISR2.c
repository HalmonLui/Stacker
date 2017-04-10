extern volatile char byte1, byte2;

/***************************************************************************************
 * Pushbutton - Interrupt Service Routine                                
 *                                                                          
 * This routine checks which KEY has been pressed. If it is KEY1 or KEY2, it writes this 
 * value to the global variable key_pressed. If it is KEY3 then it loads the SW switch 
 * values and stores in the variable pattern
****************************************************************************************/
void PS2_ISR( void )
{
  	volatile int * PS2_ptr = (int *) 0xFF200100; 
// PS/2 port address. Change for DE0-CV to 0xFF200100//
	int PS2_data, RVALID;

	PS2_data = *(PS2_ptr);					// read the Data register in the PS/2 port
	RVALID = (PS2_data & 0x8000);			// extract the RVALID field
	if (RVALID)
	{
		/* always save the last two bytes received */
		byte1 = byte2;
		byte2 = PS2_data & 0xFF;

		if ( (byte1 == (char) 0xAA) && (byte2 == (char) 0x00) )
			// mouse present; initialize sending of data
			*(PS2_ptr) = 0xF4;
	}
	return;
}
