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
void VGA_text (int, int, char*);
int main(void)

{
	int xline1, xline2, lines, yline1, yline2,linesy;
	int x_dir, x;
	int red, black, blue, green;
	int x_0, y0, y1, y_2, y_3, y_4, y_5, y_6,y_7,y_8,y_9,y_10,y_11;
	int x_1, x_2, x_3, x_4, x_5, x_6,x_7,x_8,x_9;
	int x_0a, x_1a, x_2a, x_3a, x_4a, x_5a, x_6a, x_7a, x_8a, x_9a;
	int x_0b, x_1b, x_2b, x_3b, x_4b, x_5b, x_6b, x_7b, x_8b, x_9b;
	int x_0c, x_1c, x_2c, x_3c, x_4c, x_5c, x_6c, x_7c, x_8c, x_9c;
	int x_0d, x_1d, x_2d, x_3d, x_4d, x_5d, x_6d, x_7d, x_8d, x_9d;
	int x_0e, x_1e, x_2e, x_3e, x_4e, x_5e, x_6e, x_7e, x_8e, x_9e;
	int SW_value = 0;
	volatile int * interval_timer_ptr = (int *) 0xFF202000;	// interal timer base address
	volatile int * PS2_ptr = (int *) 0xFF200100;			// PS/2 keyboard port address
	volatile int * SW_ptr = (int *) 0xFF200040; /* Slider Switches */

	/* initialize some variables */
	
	timeout = 0; //Start the program with the "update line position now" flag cleared

	/* set the interval timer period */

	


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
	//Coordinates for 4th row box
	y_6 = 147;
	y_7 = 117;
	//Coordinates for the 5th row box
	y_8 = 116;
	y_9 = 86;
	//Coordinates for the 6th row box
	y_10 = 85;
	y_11 = 55;
	
	x_dir = 30;	//boxes move in intervals of 30 pixels
	red = 0xF800;	// red
	black = 0; //black
	blue = 0x001F; //blue
	green = 0x07E0; //green
	char clear[100]="                                                                                                                                      \0";
	char testing[80]="Welcome To (((STACKER)))\0";
	char instructions[80]="Use switches to stop the box. Start with SW0.\0";
	char rules[80]="To win, stack the box on top until you reach the green!\0";
	VGA_text(1,1,testing);
	while (1)
	{

int counter = 0x5F5E100;		// 1/(50 MHz) x (0xBEBC20) = 250 msec	
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
		draw_box(0, y_10, 320, y_9, green); //draws green line
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
		x_2a=x_2;
		x_3a=x_3;
		x_4a=x_4;
		x_5a=x_5;
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
			x_0b=x_0;
			x_1b=x_1;
			x_2b=x_2;
			x_3b=x_3;
			x_4b=x_4;
			x_5b=x_5;
			x_6b=x_6;
			x_7b=x_7;
			x_8b=x_8;
			x_9b=x_9;
			draw_box(x_0a, y_3, x_1a, y_2, get_color(x_0a,230)); //draws black box
			draw_box(x_6a, y_3, x_7a, y_2, get_color(x_6a,230)); //draws black box
			draw_box(x_8a, y_3, x_9a, y_2, get_color(x_8a,230)); //draws black box
			draw_box(x_2a, y_3, x_3a, y_2, black); //black box (left)
			draw_box(x_4a, y_3, x_5a, y_2, black); //black box (right)
		
		draw_box(x_0, y_5, x_1, y_4, get_color(x_0a,y_3)); //draws red box
		draw_box(x_6, y_5, x_7, y_4, get_color(x_6a,y_3)); //draws red box
		draw_box(x_8, y_5, x_9, y_4, get_color(x_8a,y_3)); //draws red box
		draw_box(x_2, y_5, x_3, y_4, black); //black box (left)
		draw_box(x_4, y_5, x_5, y_4, black); //black box (right)
		}
		
		//Row D
		if (SW_value == 7) {
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
			x_0c=x_0;
			x_1c=x_1;
			x_2c=x_2;
			x_3c=x_3;
			x_4c=x_4;
			x_5c=x_5;
			x_6c=x_6;
			x_7c=x_7;
			x_8c=x_8;
			x_9c=x_9;
			draw_box(x_0b, y_5, x_1b, y_4, get_color(x_0b,200)); //draws black box
			draw_box(x_6b, y_5, x_7b, y_4, get_color(x_6b,200)); //draws black box
			draw_box(x_8b, y_5, x_9b, y_4, get_color(x_8b,200)); //draws black box
			draw_box(x_2b, y_5, x_3b, y_4, black); //black box (left)
			draw_box(x_4b, y_5, x_5b, y_4, black); //black box (right)
			
		draw_box(x_0, y_7, x_1, y_6, get_color(x_0b,y_5)); //draws red box
		draw_box(x_6, y_7, x_7, y_6, get_color(x_6b,y_5)); //draws red box
		draw_box(x_8, y_7, x_9, y_6, get_color(x_8b,y_5)); //draws red box
		draw_box(x_2, y_7, x_3, y_6, black); //black box (left)
		draw_box(x_4, y_7, x_5, y_6, black); //black box (right)
		}
		
		//Row E
		if (SW_value == 15) {
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
			x_0d=x_0;
			x_1d=x_1;
			x_2d=x_2;
			x_3d=x_3;
			x_4d=x_4;
			x_5d=x_5;
			x_6d=x_6;
			x_7d=x_7;
			x_8d=x_8;
			x_9d=x_9;
			draw_box(x_0c, y_7, x_1c, y_6, get_color(x_0c,170)); //draws black box
			draw_box(x_6c, y_7, x_7c, y_6, get_color(x_6c,170)); //draws black box
			draw_box(x_8c, y_7, x_9c, y_6, get_color(x_8c,170)); //draws black box
			draw_box(x_2c, y_7, x_3c, y_6, black); //black box (left)
			draw_box(x_4c, y_7, x_5c, y_6, black); //black box (right)
			
		draw_box(x_0, y_9, x_1, y_8, get_color(x_0c,y_7)); //draws red box
		draw_box(x_6, y_9, x_7, y_8, get_color(x_6c,y_7)); //draws red box
		draw_box(x_8, y_9, x_9, y_8, get_color(x_8c,y_7)); //draws red box
		draw_box(x_2, y_9, x_3, y_8, black); //black box (left)
		draw_box(x_4, y_9, x_5, y_8, black); //black box (right)
		}

		//Row F
		if (SW_value == 31) {
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
			x_0e=x_0;
			x_1e=x_1;
			x_2e=x_2;
			x_3e=x_3;
			x_4e=x_4;
			x_5e=x_5;
			x_6e=x_6;
			x_7e=x_7;
			x_8e=x_8;
			x_9e=x_9;
			draw_box(x_0d, y_9, x_1d, y_8, get_color(x_0d,140)); //draws black box
			draw_box(x_6d, y_9, x_7d, y_8, get_color(x_6d,140)); //draws black box
			draw_box(x_8d, y_9, x_9d, y_8, get_color(x_8d,140)); //draws black box
			draw_box(x_2d, y_9, x_3d, y_8, black); //black box (left)
			draw_box(x_4d, y_9, x_5d, y_8, black); //black box (right)
			
		draw_box(x_0, y_11, x_1, y_10, get_color(x_0d,y_9)); //draws red box
		draw_box(x_6, y_11, x_7, y_10, get_color(x_6d,y_9)); //draws red box
		draw_box(x_8, y_11, x_9, y_10, get_color(x_8d,y_9)); //draws red box
		draw_box(x_2, y_11, x_3, y_10, black); //black box (left)
		draw_box(x_4, y_11, x_5, y_10, black); //black box (right)
		}
		
		//win or losing condition
		if (SW_value == 63) {
			if (get_color(x_0,y_11)==get_color(x_0e,y_9) || get_color(x_6,y_11)==get_color(x_6e,y_9) || get_color(x_8,y_11)==get_color(x_8e,y_9)){
				char win[7] = "You Win\0";
				VGA_text(20,9,win);
			}	
			draw_box(x_0e, y_11, x_1e, y_10, get_color(x_0e,110)); //draws black box
			draw_box(x_6e, y_11, x_7e, y_10, get_color(x_6e,110)); //draws black box
			draw_box(x_8e, y_11, x_9e, y_10, get_color(x_8e,110)); //draws black box
			draw_box(x_2e, y_11, x_3e, y_10, black); //black box (left)
			draw_box(x_4e, y_11, x_5e, y_10, black); //black box (right)
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

void VGA_text(int x, int y, char * text_ptr)
{
	int offset;
  	volatile char * character_buffer = (char *) 0x09000000;	// VGA character buffer

	/* assume that the text string fits on one line */
	offset = (y << 7) + x;
	while ( *(text_ptr) )
	{
		*(character_buffer + offset) = *(text_ptr);	// write to the character buffer
		++text_ptr;
		++offset;
	}
}