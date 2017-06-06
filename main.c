
#include <stdio.h>

#include "ww.h"

ww_rgba_t pink = { 0xa5, 0x54, 0x90 };
short pentagon_x[5] = { 290, 380, 500, 480, 310};
short pentagon_y[5] = { 270, 160, 270, 410, 420};

int main( void ) {
	
	if(ww_window_create("Testing", 640, 480)) {
		printf("Failure\n");
		return 1;
	}
	
	ww_polygon_t * pink_pentagon = ww_new_polygon(pink, pentagon_x, pentagon_y, 5);
	ww_draw_polygon(pink_pentagon);
	
	while(!ww_window_received_quit_event()) {
		ww_window_update_events();
		ww_window_update_buffer();
	}
	
	ww_free_polygon(pink_pentagon);
	ww_window_destroy();
	return 0;
}
