
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
	init_mario_sprite();
	
	mario->pad_x = 0;
	mario->pad_y = 650;
	
	while(!ww_window_received_quit_event()) {
		
		ww_clear_buffer();
		
		ww_draw_animation(tripenta);
		ww_draw_animation(owl);
		
		mario->active_animation = 3;
		ww_draw_sprite(mario);
		
		//~ if( mario->animations[mario->active_animation]->d_progress == 
			//~ mario->animations[mario->active_animation]->count - 1){
				//~ mario->active_animation++;
				
				//~ if(mario->active_animation == mario->count - 1)
					//~ mario->active_animation = 0;
		//~ }
		
		ww_window_update_events();
		ww_window_update_buffer();
		
	}
	
	ww_free_anim(tripenta);
	ww_free_anim(owl);
	ww_free_sprite(mario);
		
	ww_window_destroy();
	return 0;
}
