
#include <stdio.h>

#include "ww.h"

#include "ba.h"
#include "bb.h"
#include "bx.h"
#include "by.h"
#include "controller.h"
#include "down.h"
#include "left.h"
#include "right.h"
#include "sel.h"
#include "start.h"
#include "up.h"

ww_sprite_t * ba         = NULL;
ww_sprite_t * ba2        = NULL;
ww_sprite_t * bb         = NULL;
ww_sprite_t * bx         = NULL;
ww_sprite_t * by         = NULL;
ww_sprite_t * controller = NULL;
ww_sprite_t * down       = NULL;
ww_sprite_t * left       = NULL;
ww_sprite_t * right      = NULL;
ww_sprite_t * sel        = NULL;
ww_sprite_t * start      = NULL;
ww_sprite_t * up         = NULL;

void inits(){
	
	ba         = ww_new_sprite(BA        );
	
	ba2 = ww_clone_sprite(ba);
	free(ba);
	
	bb         = ww_new_sprite(BB        );
	bx         = ww_new_sprite(BX        );
	by         = ww_new_sprite(BY        );
	controller = ww_new_sprite(CONTROLLER);
	down       = ww_new_sprite(DOWN      );
	left       = ww_new_sprite(LEFT      );
	right      = ww_new_sprite(RIGHT     );
	sel        = ww_new_sprite(SEL       );
	start      = ww_new_sprite(START     );
	up         = ww_new_sprite(UP        );
	
}

int main( int argc, char * argv[] ) {
	
	if(ww_window_create(argc, argv, "Pixarray", 1024, 576)) {
		printf("Closing..\n");
		return 1;
	}
		
	inits();
	
	int i = 0;
		
	while(!ww_window_received_quit_event()) {
		
		
		ww_window_update_events();
		
		if(istate.ba)
			ba2->active_animation=1;
		else
			ba2->active_animation=0;
		if(istate.bb)
			bb->active_animation=1;
		else
			bb->active_animation=0;
		if(istate.bx)
			bx->active_animation=1;
		else
			bx->active_animation=0;
		if(istate.by)
			by->active_animation=1;
		else
			by->active_animation=0;
		if(istate.up)
			up->active_animation=1;
		else
			up->active_animation=0;
		if(istate.dn)
			down->active_animation=1;
		else
			down->active_animation=0;
		if(istate.lt)
			left->active_animation=1;
		else
			left->active_animation=0;
		if(istate.rt)
			right->active_animation=1;
		else
			right->active_animation=0;
		if(istate.str)
			start->active_animation=1;
		else
			start->active_animation=0;
		if(istate.sel)
			sel->active_animation=1;
		else
			sel->active_animation=0;
		
		ww_draw_sprite(controller);
		ww_draw_sprite(ba2       );
		ww_draw_sprite(bb        );
		ww_draw_sprite(bx        );
		ww_draw_sprite(by        );
		ww_draw_sprite(down      );
		ww_draw_sprite(left      );
		ww_draw_sprite(right     );
		ww_draw_sprite(sel       );
		ww_draw_sprite(start     );
		ww_draw_sprite(up        );
		
		ww_window_update_buffer();
		
		//~ if (i == 100000)
			//~ ww_window_send_quit_event();
		
		//~ i++;
		
	}
	
	free(controller);
	free(ba2       );
	free(bb        );
	free(bx        );
	free(by        );
	free(down      );
	free(left      );
	free(right     );
	free(sel       );
	free(start     );
	free(up        );
	
	ww_window_destroy();
	return 0;
}
