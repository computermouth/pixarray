
#include <stdio.h>

#include "ww.h"
#include "untitled.h"

int main( int argc, char * argv[] ) {
	
	if(ww_window_create(argc, argv, "Pixarray", 1280, 720)) {
		printf("Closing..\n");
		return 1;
	}
	
	init_untitled();
	
	untitled->pad_x = 400;
	untitled->pad_y = 100;
	untitled->active_animation = 3;
	
	uint8_t untitled_count = 0;
	
	while(!ww_window_received_quit_event()) {
		
		ww_window_update_events();
		
		//~ ww_draw_sprite(loading);
		
		if (untitled_count == 128){
			untitled->active_animation++;
			untitled->active_animation = untitled->active_animation % 4;
			untitled_count = 0;
		}
		ww_draw_sprite(untitled);
		
		ww_window_update_buffer();
		
		untitled_count++;
	}
	
	ww_free_sprite(untitled);
		
	ww_window_destroy();
	return 0;
}
