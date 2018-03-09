
#include <stdio.h>

#include "ww.h"

#include "mario_sprite.h"
#include "loading_sprite.h"
#include "cat_sprite.h"

int main( int argc, char * argv[] ) {
	
	if(ww_window_create(argc, argv, "Pixarray", 1280, 720)) {
		printf("Closing..\n");
		return 1;
	}
	
	init_mario_sprite();
	init_loading_sprite();
	init_cat_sprite();
	
	mario->pad_x = -80;
	mario->pad_y = 595;
	mario->active_animation = 3;
	
	cat->pad_x = 0;
	cat->pad_y = 0;
	cat->active_animation = 1;
	
	while(!ww_window_received_quit_event()) {
		
		ww_window_update_events();
		
		//~ ww_draw_sprite(loading);
		
		mario->pad_x += 5;
		ww_draw_sprite(mario);
		
		//~ cat->pad_x += 2;
		ww_draw_sprite(cat);
		
		ww_window_update_buffer();
		
		if (mario->pad_x > 1320)
			mario->pad_x = -80;
		
		//~ if (cat->pad_x > 1320)
			//~ cat->pad_x = -80;
	}
	
	ww_free_sprite(mario);
	ww_free_sprite(loading);
	ww_free_sprite(cat);
		
	ww_window_destroy();
	return 0;
}
