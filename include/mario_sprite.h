

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

void init_mario_jump_left_anim(){
	
	init_mario_jump_left_0();
	
	int delay[] = { 0 };
	mario_jump_left = ww_new_animation(0, delay, mario_jump_left_0_frame, NULL);
	
}

extern ww_animation_t * mario_jump_right;
ww_animation_t * mario_jump_right = NULL;

void init_mario_jump_right_anim(){
	
	init_mario_jump_right_0();
	
	int delay[] = { 0 };
	mario_jump_right = ww_new_animation(0, delay, mario_jump_right_0_frame, NULL);
	
}

extern ww_animation_t * mario_run_left;
ww_animation_t * mario_run_left = NULL;

void init_mario_run_left_anim(){
	
	init_mario_run_left_0();
	init_mario_run_left_1();
	init_mario_run_left_2();
	
	int delay[] = { 10, 10, 10 };
	mario_run_left = ww_new_animation(0, delay,
		mario_run_left_0_frame, 
		mario_run_left_1_frame, 
		mario_run_left_2_frame, 
		NULL);
	
}

extern ww_animation_t * mario_run_right;
ww_animation_t * mario_run_right = NULL;

void init_mario_run_right_anim(){
	
	init_mario_run_right_0();
	init_mario_run_right_1();
	init_mario_run_right_2();
	
	int delay[] = { 10, 10, 10 };
	mario_run_right = ww_new_animation(0, delay,
		mario_run_right_0_frame, 
		mario_run_right_1_frame, 
		mario_run_right_2_frame, 
		NULL);
	
}

extern ww_animation_t * mario_stand_left;
ww_animation_t * mario_stand_left = NULL;

void init_mario_stand_left_anim(){
	
	init_mario_stand_left_0();
	
	int delay[] = { 0 };
	mario_stand_left = ww_new_animation(0, delay, mario_stand_left_0_frame, NULL);
	
}

extern ww_animation_t * mario_stand_right;
ww_animation_t * mario_stand_right = NULL;

void init_mario_stand_right_anim(){
	
	init_mario_stand_right_0();
	
	int delay[] = { 0 };
	mario_stand_right = ww_new_animation(0, delay, mario_stand_right_0_frame, NULL);
	
}
