
#include <stdio.h>

#include "ww.h"
#include "test_shapes.h"
#include "owl_anim.h"
#include "mario_sprite.h"

int main( int argc, char * argv[] ) {
	
	if(ww_window_create(argc, argv, "Pixarray", 1280, 720)) {
		printf("Closing..\n");
		return 1;
	}
	
	ww_window_s *window_p = (ww_window_s*) window;
	
	test_shapes_init();
	init_owl_anim();
	init_mario_sprite();
	
	mario->pad_x = -80;
	mario->pad_y = 595;
	
	while(!ww_window_received_quit_event()) {
		
		SDL_SetRenderDrawColor(window_p->ww_sdl_renderer, 255, 255, 255, 255);
		SDL_RenderDrawLine(window_p->ww_sdl_renderer, 0, 0, 1600, 1600);
		
		mario->active_animation = 3;
		mario->pad_x += 5;
		ww_draw_sprite(mario);
		
		ww_window_update_events();
		ww_window_update_buffer();
		
		if (mario->pad_x > 1320)
			mario->pad_x = -80;
	}
	
	ww_free_anim(tripenta);
	ww_free_anim(owl);
	ww_free_sprite(mario);
		
	ww_window_destroy();
	return 0;
}
