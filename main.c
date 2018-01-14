
#include <stdio.h>

#include "ww.h"
#include "test_shapes.h"

int main( void ) {
	
	if(ww_window_create("Testing", 800, 500)) {
		printf("Failure\n");
		return 1;
	}
	
	test_shapes_init();
	
	while(!ww_window_received_quit_event()) {
		
		ww_clear_buffer();
		
		ww_draw_animation(tripenta);
		
		ww_window_update_events();
		ww_window_update_buffer();
	}
	
	ww_free_anim(tripenta);
		
	ww_window_destroy();
	return 0;
}
