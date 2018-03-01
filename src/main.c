
#include <stdio.h>

#include "ww.h"
#include "test_shapes.h"
#include "owl_anim.h"
#include "mario_sprite.h"

int main( int argc, char * argv[] ) {
	
	if(ww_window_create(argc, argv, "Pixarray", 1280, 720)) {
		printf("Failure\n");
		return 1;
	}
	
	test_shapes_init();
	init_owl_anim();
	init_mario_sprite();
	
	mario->pad_x = 0;
	mario->pad_y = 595;
	
	while(!ww_window_received_quit_event()) {
		
		ww_clear_buffer();
		
		//~ ww_draw_animation(tripenta);
		//~ ww_draw_animation(owl);
		
		mario->active_animation = 3;
		mario->pad_x += 1;
		ww_draw_sprite(mario);
		
		ww_window_update_events();
		ww_window_update_buffer();
		
		if (mario->pad_x > 1000)
			goto quit;
	}
	
	quit:
	
	ww_free_anim(tripenta);
	ww_free_anim(owl);
	ww_free_sprite(mario);
		
	ww_window_destroy();
	return 0;
}
