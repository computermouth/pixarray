
#include <stdio.h>

#include "ww.h"
#include "test_shapes.h"

int main( void ) {
	
	if(ww_window_create("Testing", 640, 480)) {
		printf("Failure\n");
		return 1;
	}
	
	test_shapes_init();
	
	ww_draw_polygon(pink_pentagon);
	ww_draw_polygon(green_triangle);
	
	while(!ww_window_received_quit_event()) {
		
		ww_draw_polygon(pink_pentagon);
		ww_draw_polygon(green_triangle);
		
		ww_window_update_events();
		ww_window_update_buffer();
	}
	
	ww_free_polygon(pink_pentagon);
	ww_free_polygon(green_triangle);
	
	ww_window_destroy();
	return 0;
}
