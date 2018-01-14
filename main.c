
#include <stdio.h>

#include "ww.h"
#include "test_shapes.h"

int main( void ) {
	
	if(ww_window_create("Testing", 800, 500)) {
		printf("Failure\n");
		return 1;
	}
	
	test_shapes_init();
	
	ww_frame_t * tri012 = NULL;
	tri012 = ww_new_frame(triangle0, triangle1, triangle2, NULL);
	
	ww_frame_t * tri345 = NULL;
	tri345 = ww_new_frame(triangle3, triangle4, triangle5, NULL);
	
	ww_frame_t * tri6789 = NULL;
	tri6789 = ww_new_frame(triangle6, triangle7, triangle8, triangle9, NULL);
	
	ww_frame_t * pentaocta = NULL;
	pentaocta = ww_new_frame(pink_pentagon, yellow_octagon, NULL);
	
	ww_animation_t * tripenta = NULL;
	int delay[] = { 0 , 1, 2 , 3 };
	
	tripenta = ww_new_animation(0, delay, tri012, tri345, tri6789, pentaocta, NULL);
	
	//~ while(!ww_window_received_quit_event()) {
		
		//~ ww_window_update_events();
		//~ ww_window_update_buffer();
	//~ }
	
	ww_free_anim(tripenta);
		
	ww_window_destroy();
	return 0;
}
