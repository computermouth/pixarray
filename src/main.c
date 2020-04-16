
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

nn_sprite_t * ba         = NULL;
nn_sprite_t * bb         = NULL;
nn_sprite_t * bx         = NULL;
nn_sprite_t * by         = NULL;
nn_sprite_t * controller = NULL;
nn_sprite_t * down       = NULL;
nn_sprite_t * left       = NULL;
nn_sprite_t * right      = NULL;
nn_sprite_t * sel        = NULL;
nn_sprite_t * start      = NULL;
nn_sprite_t * up         = NULL;

void inits(){
	ba         = nn_new_sprite(BA        );
	bb         = nn_new_sprite(BB        );
	bx         = nn_new_sprite(BX        );
	by         = nn_new_sprite(BY        );
	controller = nn_new_sprite(CONTROLLER);
	down       = nn_new_sprite(DOWN      );
	left       = nn_new_sprite(LEFT      );
	right      = nn_new_sprite(RIGHT     );
	sel        = nn_new_sprite(SEL       );
	start      = nn_new_sprite(START     );
	up         = nn_new_sprite(UP        );
}

int main( int argc, char * argv[] ) {
	
	if(ww_window_create(argc, argv, "Pixarray", 1024, 576)) {
		printf("Closing..\n");
		return 1;
	}
	
	inits();
		
	while(!ww_window_received_quit_event()) {
		
		
		ww_window_update_events();
		
		if(istate.ba)
			ba->active_animation=1;
		else
			ba->active_animation=0;
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
		
		nn_draw_sprite(controller);
		nn_draw_sprite(ba        );
		nn_draw_sprite(bb        );
		nn_draw_sprite(bx        );
		nn_draw_sprite(by        );
		nn_draw_sprite(down      );
		nn_draw_sprite(left      );
		nn_draw_sprite(right     );
		nn_draw_sprite(sel       );
		nn_draw_sprite(start     );
		nn_draw_sprite(up        );
		
		ww_window_update_buffer();
	}
	
	free(ba);
	
	ww_window_destroy();
	return 0;
}
