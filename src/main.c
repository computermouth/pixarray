
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

void inits(){
	init_ba();
	init_bb();
	init_bx();
	init_by();
	init_controller();
	init_down();
	init_left();
	init_right();
	init_sel();
	init_start();
	init_up();	
}

int main( int argc, char * argv[] ) {
	
	if(ww_window_create(argc, argv, "Pixarray", 1024, 576)) {
		printf("Closing..\n");
		return 1;
	}
	
	inits();
	
	while(!ww_window_received_quit_event()) {
		
		if(istate.a)
			ba->active_animation=1;
		else
			ba->active_animation=0;
		if(istate.b)
			bb->active_animation=1;
		else
			bb->active_animation=0;
		if(istate.x)
			bx->active_animation=1;
		else
			bx->active_animation=0;
		if(istate.y)
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
		ww_draw_sprite(ba);
		ww_draw_sprite(bb);
		ww_draw_sprite(bx);
		ww_draw_sprite(by);
		ww_draw_sprite(down);
		ww_draw_sprite(left);
		ww_draw_sprite(right);
		ww_draw_sprite(sel);
		ww_draw_sprite(start);
		ww_draw_sprite(up);	
		
		ww_window_update_events();
		ww_window_update_buffer();
	}
	
	ww_window_destroy();
	return 0;
}
