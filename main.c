
#include <stdio.h>

#include "ww.h"

int main( void ) {
	
	if(ww_window_create("Testing", 640, 480)) {
		printf("Failure\n");
		return 1;
	}
	
	ww_rgba_t pink = { 0xa5, 0x54, 0x90 };
	ww_memset_pixel( 40, 40, &pink);
	
	ww_rgba_t green = { 0x00, 0xff, 0x90 };
	ww_memset_pixel_range( 320, 80, &green, (640));
	
	ww_rgba_t blue = { 0x00, 0x54, 0x90 };
	ww_memset_pixel_range( 0, 120, &blue, (640*2));
	
	ww_rgba_t white = { 0xff, 0xff, 0xff };
	ww_memset_pixel_range( 0, 160, &white, (640*20));
	
	ww_rgba_t grey = { 0x88, 0x88, 0x88 };
	ww_hline( 0, 10, 200, &grey);
	
	ww_rgba_t yellow = { 0xff, 0xff, 0x00 };
	ww_hline(   0,  10, 240, &yellow);
	ww_hline(  20,  30, 240, &yellow);
	ww_hline(  40,  50, 240, &yellow);
	ww_hline(  60,  70, 240, &yellow);
	ww_hline(  80,  90, 240, &yellow);
	ww_hline( -3100, 9110, 240, &yellow);
	ww_hline( 120, 130, 240, &yellow);
	ww_hline( 140, 150, 240, &yellow);
	
	while(!ww_window_received_quit_event()) {
		ww_window_update_events();
		ww_window_update_buffer();
	}
	
	ww_window_destroy();
	return 0;
}
