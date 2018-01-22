
#include <stdio.h>

#include "ww.h"
#include "test_shapes.h"
#include "owl_anim.h"
#include "mario_sprite.h"

int main( void ) {
	
	if(ww_window_create("Testing", 1280, 720)) {
		printf("Failure\n");
		return 1;
	}
	
	test_shapes_init();
	init_owl_anim();
	
	while(!ww_window_received_quit_event()) {
		
		ww_clear_buffer();
		
		ww_draw_animation(tripenta);
		ww_draw_animation(owl);
		
		ww_window_update_events();
		ww_window_update_buffer();
	}
	
	ww_free_anim(tripenta);
	ww_free_anim(owl);
		
	ww_window_destroy();
	return 0;
}
