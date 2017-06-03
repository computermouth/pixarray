
#include <stdio.h>

#include "ww.h"

int main( void ) {
	
	if(ww_window_create("Testing", 640, 480)) {
		printf("Failure\n");
		return 1;
	}
	
	ww_rgba_t pink = { 0xa5, 0x54, 0x90 };
	ww_memset_pixel(buffer, &pink, (640*480));
	
	ww_rgba_t blue = { 0x00, 0x54, 0x90 };
	ww_memset_pixel(buffer, &blue, (640*240));
	
	ww_rgba_t green = { 0x00, 0xff, 0x90 };
	//~ ww_memset_pixel(buffer + (640 * 155 * 4), &green, (640*1));
	ww_hline(buffer, &green, 640, 120, 520, 155);
	ww_memset_pixel(buffer + (640 * 175 * 4), &green, (640*2));
	ww_memset_pixel(buffer + (640 * 195 * 4), &green, (640*4));
	ww_memset_pixel(buffer + (640 * 215 * 4), &green, (640*8));
	
	while(!ww_window_received_quit_event()) {
		ww_window_update_events();
		ww_window_update_buffer(buffer);
	}
	
	ww_window_destroy();
	return 0;
}
