

#include "mario_jump_left_0.h"

#include "mario_jump_right_0.h"

#include "mario_run_left_0.h"
#include "mario_run_left_1.h"
#include "mario_run_left_2.h"

#include "mario_run_right_0.h"
#include "mario_run_right_1.h"
#include "mario_run_right_2.h"

#include "mario_stand_left_0.h"

#include "mario_stand_right_0.h"

extern ww_animation_t * mario_jump_left;
ww_animation_t * mario_jump_left = NULL;
extern ww_animation_t * mario_jump_right;
ww_animation_t * mario_jump_right = NULL;
extern ww_animation_t * mario_run_left;
ww_animation_t * mario_run_left = NULL;
extern ww_animation_t * mario_run_right;
ww_animation_t * mario_run_right = NULL;
extern ww_animation_t * mario_stand_left;
ww_animation_t * mario_stand_left = NULL;
extern ww_animation_t * mario_stand_right;
ww_animation_t * mario_stand_right = NULL;


void init_mario_jump_left_anim(){
	
	init_mario_jump_left_0();
	
	int delay[] = { 0 };
	mario_jump_left = ww_new_animation( delay, mario_jump_left_0_frame, NULL);
	
}


void init_mario_jump_right_anim(){
	
	init_mario_jump_right_0();
	
	int delay[] = { 0 };
	mario_jump_right = ww_new_animation( delay, mario_jump_right_0_frame, NULL);
	
}


void init_mario_run_left_anim(){
	
	init_mario_run_left_0();
	init_mario_run_left_1();
	init_mario_run_left_2();
	
	int delay[] = { 3, 3, 3 };
	mario_run_left = ww_new_animation(delay,
		mario_run_left_0_frame, 
		mario_run_left_1_frame, 
		mario_run_left_2_frame, 
		NULL);
	
}


void init_mario_run_right_anim(){
	
	init_mario_run_right_0();
	init_mario_run_right_1();
	init_mario_run_right_2();
	
	int delay[] = { 3, 3, 3 };
	mario_run_right = ww_new_animation(delay,
		mario_run_right_0_frame, 
		mario_run_right_1_frame, 
		mario_run_right_2_frame, 
		NULL);
	
}


void init_mario_stand_left_anim(){
	
	init_mario_stand_left_0();
	
	int delay[] = { 0 };
	mario_stand_left = ww_new_animation(delay, mario_stand_left_0_frame, NULL);
	
}


void init_mario_stand_right_anim(){
	
	init_mario_stand_right_0();
	
	int delay[] = { 0 };
	mario_stand_right = ww_new_animation(delay, mario_stand_right_0_frame, NULL);
	
}

extern ww_sprite_t * mario;
ww_sprite_t * mario = NULL;

void init_mario_sprite(){
	
	init_mario_jump_left_anim();
	init_mario_jump_right_anim();
	init_mario_run_left_anim();
	init_mario_run_right_anim();
	init_mario_stand_left_anim();
	init_mario_stand_right_anim();
	
	mario = ww_new_sprite( 0,
		mario_jump_left,
		mario_jump_right,
		mario_run_left,
		mario_run_right,
		mario_stand_left,
		mario_stand_right,
		NULL
	);
	
}


