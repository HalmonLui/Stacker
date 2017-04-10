#include "nios2_ctrl_reg_macros.h"
#include <stdio.h>

/* This program implements a basic animation by drawing a HORIZONTAL line that moves up
 * and down the screen. The speed of movement is controlled by the VGA vertical sync
 * timing. 
 * For DE0-CV: YELLOW 0xFF00, RED 0xFA00, GREEN 0x1CE0, BLUE 0x001F 
 */
 
/* global variables */
extern volatile int timeout;	// used to synchronize line updates on 250ms
								// intervals set by the intervalTimer_ISR

volatile int pixel_buffer_start = 0x08000000;// VGA pixel buffer
int resolution_x = 320; // VGA screen size	320 for DE0-CV
int resolution_y = 240; // VGA screen size	240 for DE0-CV


void clear_screen(void);
void draw_hline(int, int, int, int, int);
void draw_vline(int, int, int, int, int);
void draw_box(int, int, int, int, int);
void plot_pixel(int, int, short int);

int main(void)

{
	
	
	
	int x_0, y0, y1, y_2, y_3, y_4, y_5, x_1, x_2, x_3, x_4, x_5, x_L1, x_L2, x_L3, x_L4, x_dir, x, color, color_1;
	int x_0a, y_3a, x_1a, x_2a, x_3a, y_2a, x_4a, x_5a, x_L1a, x_L2a, x_L3a, x_L4a;
	int x_0b, y_3b, x_1b, x_2b, x_3b, y_2b, x_4b, x_5b, x_L1b, x_L2b, x_L3b, x_L4b;
	int SW_value = 0;
	volatile int * interval_timer_ptr = (int *) 0xFF202000;	// interal timer base address
	volatile int * PS2_ptr = (int *) 0xFF200100;			// PS/2 keyboard port address
	volatile int * SW_ptr = (int *) 0xFF200040; /* Slider Switches */

	/* initialize some variables */
	
	timeout = 0; //Start the program with the "update line position now" flag cleared

	/* set the interval timer period */

	int counter = 500000;		// 1/(50 MHz) x (0xBEBC20) = 250 msec	
	//int counter = 0x02FAF080;		// 1/(50 MHz) x (0x02FAF080) = 1sec
	*(interval_timer_ptr + 0x2) = (counter & 0xFFFF);
	*(interval_timer_ptr + 0x3) = (counter >> 16) & 0xFFFF;

	/* start interval timer, enable its interrupts */
	*(interval_timer_ptr + 1) = 0x7; // STOP = 0, START = 1, CONT = 1, ITO = 1 
	
    *(PS2_ptr) = 0xFF;		// reset
	*(PS2_ptr + 1) = 0x1; 	//write to the PS/2 Control register to enable interrupts

	NIOS2_WRITE_IENABLE( 0xC3 );/* set interrupt mask bits for levels 0 (interval
								* timer), 1 (pushbuttons), 6 (audio), and 7 (PS/2) */

	NIOS2_WRITE_STATUS( 1 );		// enable Nios II interrupts


	clear_screen ( );	// Normally make the VGA screen BLACK. 
						// BLUE for highlighting erasure here.
						
	//Coordinates for bottom row box					
	x_0 = 32; 	//red box
	x_1 = 126;	//red box
	x_2 = 1;	//black box
	x_3 = 31;	//black box
	x_4 = 127;	//black box
	x_5 = 157;	//black box
	x_L1 = 63; //line that separates red box
	x_L2 = 64; //line that separates red box
	x_L3 = 95; //line that separates red box
	x_L4 = 96; //line that separates red box
	
	//row B
	x_0a = 32; 	//red box
	x_1a = 126;	//red box
	x_2a = 1;	//black box
	x_3a = 31;	//black box
	x_4a = 127;	//black box
	x_5a = 157;	//black box
	x_L1a = 63; //line that separates red box
	x_L2a = 64; //line that separates red box
	x_L3a = 95; //line that separates red box
	x_L4a = 96; //line that separates red box
	
	//row C
	x_0b = 32; 	//red box
	x_1b = 126;	//red box
	x_2b = 1;	//black box
	x_3b = 31;	//black box
	x_4b = 127;	//black box
	x_5b = 157;	//black box
	x_L1b = 63; //line that separates red box
	x_L2b = 64; //line that separates red box
	x_L3b = 95; //line that separates red box
	x_L4b = 96; //line that separates red box
	
	y0 = 210;	//height of red box is 30
	y1 = 240;
	
	//Coordinates for second row box
	y_2 = 209;	//height of red box is 30
	y_3 = 179;
	//Coordinates for 3rd row box
	y_4 = 178;	//height of red box is 30
	y_5 = 148;
	
	
	x_dir = 30;	//boxes move in intervals of 30 pixels
	color = 0xF800;	// red
	color_1 = 0; //black
	
	draw_box(x_0, y0, x_1, y1, color);
	while (1)
	{
		while(!timeout)
		{};
		// Hold the line in current position 
		SW_value = *(SW_ptr); // Read the Slider Switches
		int count = 0;
		while (count < 5000000){
		count++;
		}
		
		
		
		if ((x_0 <= 0) || (x_1 >= (291))) //Changes direction of red box
		{
			x_dir = -x_dir;
		}
		if ((x_2 <= 0) || (x_3 >= (resolution_x+1))) //Changes direction of black box (left)
		{
			x_dir = -x_dir;
		}
		if ((x_L1 <= 0) || (x_L2 >= (resolution_x+1))) //Changes direction of lines
		{
			x_dir = -x_dir;
		}
		if ((x_L3 <= 0) || (x_L4 >= (resolution_x+1))) //Changes direction of lines
		{
			x_dir = -x_dir;
		}

		if ((x_0a <= 0) || (x_1a >= (291))) //Changes direction of red box
		{
			x_dir = -x_dir;
		}
		if ((x_2a <= 0) || (x_3a >= (resolution_x+1))) //Changes direction of black box (left)
		{
			x_dir = -x_dir;
		}
		if ((x_L1a <= 0) || (x_L2a >= (resolution_x+1))) //Changes direction of lines
		{
			x_dir = -x_dir;
		}
		if ((x_L3a <= 0) || (x_L4a >= (resolution_x+1))) //Changes direction of lines
		{
			x_dir = -x_dir;
		}

		// Draw new line
		color = 0xF800;	// RED for DE0-CV  0xF800
		if (SW_value == 0) {
			//Row A
			// Change line position, moving it 10 pixels to the right.
			x_0 = x_0 + x_dir;
			x_1 = x_1 + x_dir;
			x_2 = x_2 + x_dir;
			x_3 = x_3 + x_dir;
			x_4 = x_4 + x_dir;
			x_5 = x_5 + x_dir;
			x_L1 = x_L1 + x_dir;
			x_L2 = x_L2 + x_dir;
			x_L3 = x_L3 + x_dir;
			x_L4 = x_L4 + x_dir;
			draw_box(x_0, y0, x_1, y1, color);	//Box 0 (red)
			draw_box(x_2, y0, x_3, y1, color_1); //black box (left)
			draw_box(x_4, y0, x_5, y1, color_1); //black box (right)
			draw_box(x_L1, y0, x_L2, y1, color_1); //Line1
			draw_box(x_L3, y0, x_L4, y1, color_1); //Line2
		}
		if (SW_value == 1) {
			//Row B
			x_0a = x_0a + x_dir; 
			x_1a = x_1a + x_dir;
			x_2a = x_2a + x_dir;
			x_3a = x_3a + x_dir;
			x_4a = x_4a + x_dir;
			x_5a = x_5a + x_dir;
			x_L1a = x_L1a + x_dir;
			x_L2a = x_L2a + x_dir;
			x_L3a = x_L3a + x_dir;
			x_L4a = x_L4a + x_dir;			
			draw_box(x_0a, y_3, x_1a, y_2, color);	//Box 1 (red)
			draw_box(x_2a, y_3, x_3a, y_2, color_1); //black box (left)
			draw_box(x_4a, y_3, x_5a, y_2, color_1); //black box (right)
			draw_box(x_L1a, y_3, x_L2a, y_2, color_1); //Line1
			draw_box(x_L3a, y_3, x_L4a, y_2, color_1); //Line2
		}
		if (SW_value == 3) {
			//Row C
			if (x_0a < x_0) { //if red is over extended left
			x_0a=x_0; //sets first black line to shrink red box B
			draw_box(0, y_3, 320, y_2, color_1); //resets row B color to black
			draw_box(x_0a, y_3, x_1a, y_2, color);	//Box B (red)
			draw_box(x_2a, y_3, x_3a, y_2, color_1); //black box (left)
			draw_box(x_4a, y_3, x_5a, y_2, color_1); //black box (right)
			draw_box(x_L1a, y_3, x_L2a, y_2, color_1); //Line1
			draw_box(x_L3a, y_3, x_L4a, y_2, color_1); //Line2
			
			//Starts row C
			draw_box(x_0, y_5, x_1, y_4, color);	//Box C (red)
			draw_box(x_2, y_5, x_3, y_4, color_1); //black box (left)
			draw_box(x_4, y_5, x_5, y_4, color_1); //black box (right)
			draw_box(x_L1, y_5, x_L2, y_4, color_1); //Line1
			draw_box(x_L3, y_5, x_L4, y_4, color_1); //Line2
			}
			else if (x_1a > x_1) { //if red is over extended right
			x_1a=x_1; //sets second black line to shrink red box B
			draw_box(0, y_3, 320, y_2, color_1); //resets row B color to black
			draw_box(x_0a, y_3, x_1a, y_2, color);	//Box B (red)
			draw_box(x_2a, y_3, x_3a, y_2, color_1); //black box (left)
			draw_box(x_4a, y_3, x_5a, y_2, color_1); //black box (right)
			draw_box(x_L1a, y_3, x_L2a, y_2, color_1); //Line1
			draw_box(x_L3a, y_3, x_L4a, y_2, color_1); //Line2
			
			//Starts row C
			x_0b = x_0b + x_dir;
			x_1b = x_1b + x_dir;
			x_2b = x_2b + x_dir;
			x_3b = x_3b + x_dir;
			x_4b = x_4b + x_dir;
			x_5b = x_5b + x_dir;
			x_L1b = x_L1b + x_dir;
			x_L2b = x_L2b + x_dir;
			x_L3b = x_L3b + x_dir;
			x_L4b = x_L4b + x_dir;
			draw_box(x_0b, y_5, x_1b, y_4, color);	//Box C (red)
			draw_box(x_2b, y_5, x_3b, y_4, color_1); //black box (left)
			draw_box(x_4b, y_5, x_5b, y_4, color_1); //black box (right)
			draw_box(x_L1b, y_5, x_L2b, y_4, color_1); //Line1
			draw_box(x_L3b, y_5, x_L4b, y_4, color_1); //Line2
			}

		}
		
		
	}
		
		
	}







/* Function to blank the VGA screen */
void clear_screen( )
{
	int clearColor = 0; 
	
/*  Normally make the VGA screen BLACK. 
	BLUE for highlighting erasure here.
	for DE0-CV substitute 0x001F
*/	
	int y, x;
	int pixel_ptr;

	for (y = 0; y < resolution_y; y++)
	{
		for (x = 0; x < resolution_x; x++)
		{		
			plot_pixel(x, y, clearColor);	// clear pixel			
		}
	}
}



/* This function draws a line between points (x0, y0)
 * and (x1, y1). One ROW at a time. The function assumes that the coordinates are valid
 * 
 */
 
void draw_box(int x0, int y0, int x1, int y1, int color)
{
	int y;
	int x;
	
	for(y=y0;y<y1;y++)
	{
		for(x=x0;x<x1;x++)
		{
			plot_pixel(x,y,color);
		}
	}
}



/* Draw Horizontal Line */
void draw_hline(int x0, int y0, int x1, int y1, int color)
{
	int y=y0;
	int x;
	
	for(x=x0;x<x1;x++)
	{
		plot_pixel(x,y,color);
	}
}

/* Draw Vertical Line */
void draw_vline(int x0, int y0, int x1, int y1, int color)
{
	int x=x0;
	int y;
	
	for(y=y0;y<y1;y++)
	{
		plot_pixel(x,y,color);
	}
}





void plot_pixel(int x, int y, short int pixel_color)
{
	//*(volatile char *)(pixel_buffer_start + (y << 7)+x) = pixel_color;
	*(volatile short int *)(pixel_buffer_start + (y << 10) + (x << 1)) = pixel_color; //For DE0-CV
}