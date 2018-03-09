

#ifndef cat_sprite_H
#define cat_sprite_H

#include "cat_walk_right_00.h"
#include "cat_walk_right_01.h"
#include "cat_walk_right_02.h"
#include "cat_walk_right_03.h"
#include "cat_walk_right_04.h"
#include "cat_walk_right_05.h"

#include "cat_walk_left_00.h"
#include "cat_walk_left_01.h"
#include "cat_walk_left_02.h"
#include "cat_walk_left_03.h"
#include "cat_walk_left_04.h"
#include "cat_walk_left_05.h"

extern ww_animation_t * cat_walk_right;
ww_animation_t * cat_walk_right = NULL;


void init_cat_walk_right_anim(){
	
	init_cat_walk_right_00();
	init_cat_walk_right_01();
	init_cat_walk_right_02();
	init_cat_walk_right_03();
	init_cat_walk_right_04();
	init_cat_walk_right_05();
	
	int delay[] = { 6, 6, 6, 6, 6, 6 };
	cat_walk_right = ww_new_animation( 
		delay, 
		cat_walk_right_00_frame,
		cat_walk_right_01_frame,
		cat_walk_right_02_frame,
		cat_walk_right_03_frame,
		cat_walk_right_04_frame,
		cat_walk_right_05_frame,
		NULL
	);
	
}

extern ww_animation_t * cat_walk_left;
ww_animation_t * cat_walk_left = NULL;


void init_cat_walk_left_anim(){
	
	init_cat_walk_left_00();
	init_cat_walk_left_01();
	init_cat_walk_left_02();
	init_cat_walk_left_03();
	init_cat_walk_left_04();
	init_cat_walk_left_05();
	
	int delay[] = { 6, 6, 6, 6, 6, 6 };
	cat_walk_left = ww_new_animation( 
		delay, 
		cat_walk_left_00_frame,
		cat_walk_left_01_frame,
		cat_walk_left_02_frame,
		cat_walk_left_03_frame,
		cat_walk_left_04_frame,
		cat_walk_left_05_frame,
		NULL
	);
	
}

extern ww_sprite_t * cat;
ww_sprite_t * cat = NULL;

void init_cat_sprite(){
	
	init_cat_walk_right_anim();
	init_cat_walk_left_anim();
	
	cat = ww_new_sprite( 0,
		cat_walk_right,
		cat_walk_left,
		NULL
	);
	
}

#endif
