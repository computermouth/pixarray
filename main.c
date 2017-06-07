
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
		
		switch(bitlevel){
			case B1:
				bitlevel = B2;
				break;
			case B2:
				bitlevel = B4;
				break;
			case B4:
				bitlevel = B8;
				break;
			case B8:
				bitlevel = B16;
				break;
			case B16:
				bitlevel = B32;
				break;
			case B32:
				bitlevel = B64;
				break;
			case B64:
				bitlevel = B128;
				break;
			case B128:
				bitlevel = B1;
				break;
			default:
				bitlevel = B1;
		}
		printf("%d\n", bitlevel);
		
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
		
		//~ SDL_Delay(1000 * 1);
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
