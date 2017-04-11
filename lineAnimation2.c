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
	
	int rowA,mover;
	int xline1, xline2, lines, yline1, yline2,linesy;
	int x_0, y0, y1, y_2, y_3, y_4, y_5, x_1, x_2, x_3, x_4, x_5, x_6,x_7,x_8,x_9, x_dir, x, red, black, blue;
	int x_0a, x_1a,x_6a,x_7a,x_8a,x_9a;
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
	x_0 = 34; 	//red box
	x_1 = 63;	//red box
	x_2 = 4;	//black box
	x_3 = 33;	//black box
	x_4 = 124;	//black box
	x_5 = 153;	//black box
	x_6 = 64; //red box
	x_7 = 93; //red box
	x_8 = 94; //red box
	x_9 = 123; //red box

	
	y0 = 210;	//height of red box is 30
	y1 = 240;
	
	//Coordinates for second row box
	y_2 = 209;	//height of red box is 30
	y_3 = 179;
	//Coordinates for 3rd row box
	y_4 = 178;	//height of red box is 30
	y_5 = 148;
	
	
	x_dir = 30;	//boxes move in intervals of 30 pixels
	red = 0xF800;	// red
	black = 0; //black
	blue = 0x001F; //blue

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
		
		if ((x_0 <= 0) || (x_9 >= (291))) //Changes direction of red box
		{
			x_dir = -x_dir;
		}
		if ((x_2 <= 0) || (x_3 >= (resolution_x+1))) //Changes direction of black box (left)
		{
			x_dir = -x_dir;
		}
		if (SW_value == 0) {
			//Row A

		xline1=3;
		xline2=4;lines=3;
		for (lines; lines <= 320; lines=lines+30){//blue lines
			draw_box(xline1, 0, xline2, 240, blue); //draws lines for the background image
			xline1=xline1+30;//add 30 to left side
			xline2=xline2+30;//add 30 to right side
		}
		x_0=x_0+x_dir;
		x_1=x_1+x_dir;
		x_2=x_2+x_dir;
		x_3=x_3+x_dir;
		x_4=x_4+x_dir;
		x_5=x_5+x_dir;
		x_6=x_6+x_dir;
		x_7=x_7+x_dir;
		x_8=x_8+x_dir;
		x_9=x_9+x_dir;
		draw_box(x_0, y0, x_1, y1, red); //draws red box
		draw_box(x_6, y0, x_7, y1, red); //draws red box
		draw_box(x_8, y0, x_9, y1, red); //draws red box
		draw_box(x_2, y0, x_3, y1, black); //black box (left)
		draw_box(x_4, y0, x_5, y1, black); //black box (right)
	}
		
		
		
		if (SW_value == 1) {
			//Row B
		x_0=x_0+x_dir;
		x_1=x_1+x_dir;
		x_2=x_2+x_dir;
		x_3=x_3+x_dir;
		x_4=x_4+x_dir;
		x_5=x_5+x_dir;
		x_6=x_6+x_dir;
		x_7=x_7+x_dir;
		x_8=x_8+x_dir;
		x_9=x_9+x_dir;
		x_0a=x_0;
		x_1a=x_1;
		x_6a=x_6;
		x_7a=x_7;
		x_8a=x_8;
		x_9a=x_9;
		draw_box(x_0, y_3, x_1, y_2, red); //draws red box
		draw_box(x_6, y_3, x_7, y_2, red); //draws red box
		draw_box(x_8, y_3, x_9, y_2, red); //draws red box
		draw_box(x_2, y_3, x_3, y_2, black); //black box (left)
		draw_box(x_4, y_3, x_5, y_2, black); //black box (right)
	}
	 //Random Comment
		//Row C	
		if (SW_value == 3) {
			x_0=x_0+x_dir;
			x_1=x_1+x_dir;
			x_2=x_2+x_dir;
			x_3=x_3+x_dir;
			x_4=x_4+x_dir;
			x_5=x_5+x_dir;
			x_6=x_6+x_dir;
			x_7=x_7+x_dir;
			x_8=x_8+x_dir;
			x_9=x_9+x_dir;

			draw_box(x_0a, y_3, x_1a, y_2, get_color(x_0,230)); //draws black box
			draw_box(x_6a, y_3, x_7a, y_2, get_color(x_6,230)); //draws black box
			draw_box(x_8a, y_3, x_9a, y_2, get_color(x_8,230)); //draws black box
			
		draw_box(x_0, y_5, x_1, y_4, get_color(x_0a,y_3)); //draws red box
		draw_box(x_6, y_5, x_7, y_4, get_color(x_6a,y_3)); //draws red box
		draw_box(x_8, y_5, x_9, y_4, get_color(x_8a,y_3)); //draws red box
		draw_box(x_2, y_5, x_3, y_4, black); //black box (left)
		draw_box(x_4, y_5, x_5, y_4, black); //black box (right)
	}
	} //closes while
} //closes main







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
 
void draw_box(int x0, int y0, int x1, int y1, int red)
{
	int y;
	int x;
	for(y=y0;y<y1;y++)
	{
		for(x=x0;x<x1;x++)
		{
			plot_pixel(x,y,red);
		}
	}
}

/* Draw Horizontal Line */
void draw_hline(int x0, int y0, int x1, int y1, int red)
{
	int y=y0;
	int x;
	for(x=x0;x<x1;x++)
	{
		plot_pixel(x,y,red);
	}
}

/* Draw Vertical Line */
void draw_vline(int x0, int y0, int x1, int y1, int red)
{
	int x=x0;
	int y;
	for(y=y0;y<y1;y++)
	{
		plot_pixel(x,y,red);
	}
}

void plot_pixel(int x, int y, short int pixel_color)
{
	//*(volatile char *)(pixel_buffer_start + (y << 7)+x) = pixel_color;
	*(volatile short int *)(pixel_buffer_start + (y << 10) + (x << 1)) = pixel_color; //For DE0-CV
}
int get_color(int x_vga, int y_vga) {
	volatile short *pixel_address = (volatile short*)(0x08000000 + (y_vga<<10) + (x_vga<<1));
	int color = *pixel_address;
    //these if statements are in place because sometimes the colors will be negative
    //if(color == 924) {//color is blue
 	//	return 0x39C;
    //}
    if(color == -2239) {//color is yellow
		return 0xF741;
    }
    if(color == -1398) {//color is red
        return 0xFA8A;
    }
    //if(color == 6080) {//color is green
    //    return 0x17C0;
    //}
	return color;
}