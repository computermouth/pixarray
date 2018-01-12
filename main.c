
#include <stdio.h>

#include "ww.h"
#include "test_shapes.h"

int main( void ) {
	
	if(ww_window_create("Testing", 640, 480)) {
		printf("Failure\n");
		return 1;
	}
	
	test_shapes_init();
	
	while(!ww_window_received_quit_event()) {
		
		ww_draw_polygon(pink_pentagon);
		ww_draw_polygon(yellow_octagon);
		
		ww_draw_polygon(triangle0);
		ww_draw_polygon(triangle1);
		ww_draw_polygon(triangle2);
		ww_draw_polygon(triangle3);
		ww_draw_polygon(triangle4);
		ww_draw_polygon(triangle5);
		ww_draw_polygon(triangle6);
		ww_draw_polygon(triangle7);
		ww_draw_polygon(triangle8);
		ww_draw_polygon(triangle9);
		
		ww_window_update_events();
		ww_window_update_buffer();
	}
	
	ww_free_polygon(pink_pentagon);
	ww_free_polygon(yellow_octagon);
	
	ww_free_polygon(triangle0);
	ww_free_polygon(triangle1);
	ww_free_polygon(triangle2);
	ww_free_polygon(triangle3);
	ww_free_polygon(triangle4);
	ww_free_polygon(triangle5);
	ww_free_polygon(triangle6);
	ww_free_polygon(triangle7);
	ww_free_polygon(triangle8);
	ww_free_polygon(triangle9);
		
	ww_window_destroy();
	return 0;
}
