
#include <stdio.h>

#include "ww.h"
#include "states.h"
#include "files.h"
#include "untitled.h"
#include "buttons.h"
#include "selector.h"
#include "title.h"

game_state_t game_state = { 0 };

void inits(){
	init_untitled();
	init_buttons();
	init_selector();
	init_title();
}

void frees(){
	ww_free_sprite(untitled);
	ww_free_sprite(buttons);
	ww_free_sprite(selector);
	ww_free_sprite(title);
}

void process_top_menu(){
	
	// move left
	if(press_state.lt){
		game_state.top_menu_selected--;
		press_state.lt = 0;
	}
	// wrap left
	if(game_state.top_menu_selected == TOP_MENU_SELECTED_PLAY - 1)
		game_state.top_menu_selected = TOP_MENU_SELECTED_QUIT;
	
	// move right
	if(press_state.rt){
		game_state.top_menu_selected++;
		press_state.rt = 0;
	}
	// wrap right
	if(game_state.top_menu_selected > TOP_MENU_SELECTED_QUIT)
		game_state.top_menu_selected = TOP_MENU_SELECTED_PLAY;
	
	selector->pad_x = 200 * game_state.top_menu_selected;
	
	if(press_state.ent){
		switch(game_state.top_menu_selected){
			case TOP_MENU_SELECTED_PLAY:
				break;
			case TOP_MENU_SELECTED_OPTIONS:
				break;
			case TOP_MENU_SELECTED_QUIT:
				game_state.top_state = TOP_STATE_QUIT;
				break;
			default:
				printf("??\n");
				break;
		}
	}
	
	ww_draw_sprite(title);
	ww_draw_sprite(buttons);
	ww_draw_sprite(selector);
	
}

void process_state(){
	
	switch (game_state.top_state){
		case TOP_STATE_IDLE:
			process_top_menu();
			break;
		case TOP_STATE_PLAY:
			break;
		case TOP_STATE_OPTIONS:
			break;
		case TOP_STATE_QUIT:
			ww_window_send_quit_event();
			break;
	}
	
}

int main( int argc, char * argv[] ) {
	
	if(ww_window_create(argc, argv, "Pixarray", 1024, 576)) {
		printf("Closing..\n");
		return 1;
	}
	
	if ( verify_or_create_save(&game_state) != 0 ){
		printf("E: failed to locate or create save file\n");
		return 1;
	} else {
		if ( mload(&game_state) != 0 ){
			return 1;
		}
	}
	
	inits();
	
	while(!ww_window_received_quit_event()) {
		
		process_state();
		
		ww_window_update_events();		
		ww_window_update_buffer();
	}
	
	frees();
	
	ww_window_destroy();
	return 0;
}
