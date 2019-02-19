
#include <stdio.h>

#include "ww.h"
#include "untitled.h"
#include "buttons.h"
#include "selector.h"
#include "title.h"

typedef struct {
	uint32_t volume;
	uint32_t scale;
	enum { OPTIONS_MENU_SELECTED_VOLUME, OPTIONS_MENU_SELECTED_SCALE, OPTIONS_MENU_SELECTED_HOME } selected;
} options_menu_state_t;

typedef struct {
	uint32_t level;
	uint32_t max_hp;
	uint32_t hp;
	uint32_t speed;
	uint32_t baseatk;
	uint32_t basedef;
	uint32_t atk[4];
	uint32_t def[4];
	uint32_t exp[9];
	uint32_t x_pos;
	uint32_t y_pos;
	uint32_t sub_x_pos;
	uint32_t sub_y_pos;
} player_t;

typedef struct {
	uint32_t map[31][31];
	player_t player;
	enum {PLAY_STATE_WALK, BATTLE, LEVELUP, WEAPONUP, DEFUP}
} play_state_t;

typedef struct {
	enum { TOP_STATE_IDLE, TOP_STATE_PLAY, TOP_STATE_OPTIONS, TOP_STATE_QUIT } top_state;
	enum { TOP_MENU_SELECTED_PLAY, TOP_MENU_SELECTED_OPTIONS, TOP_MENU_SELECTED_QUIT } top_menu_selected;
	options_menu_state_t options_menu_state;
	play_state_t play_state;
} game_state_t;

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
	if(pressstate.lt){
		game_state.top_menu_selected--;
		pressstate.lt = 0;
	}
	// wrap left
	if(game_state.top_menu_selected == TOP_MENU_SELECTED_PLAY - 1)
		game_state.top_menu_selected = TOP_MENU_SELECTED_QUIT;
	
	// move right
	if(pressstate.rt){
		game_state.top_menu_selected++;
		pressstate.rt = 0;
	}
	// wrap right
	if(game_state.top_menu_selected > TOP_MENU_SELECTED_QUIT)
		game_state.top_menu_selected = TOP_MENU_SELECTED_PLAY;
	
	selector->pad_x = 200 * game_state.top_menu_selected;
	
	if(pressstate.ent){
		switch(game_state.top_menu_selected){
			case TOP_MENU_SELECTED_PLAY:
				break;
			case TOP_MENU_SELECTED_OPTIONS:
				break;
			case TOP_MENU_SELECTED_QUIT:
				game_state.top_state = TOP_STATE_QUIT;
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
