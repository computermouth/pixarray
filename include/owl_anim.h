
#include "owl0.h"
#include "owl1.h"
#include "owl2.h"

extern ww_animation_t * owl;
ww_animation_t * owl = NULL;

void init_owl_anim(){
	
	init_owl0();
	init_owl1();
	init_owl2();
	
	int delay[] = {5, 5, 5};
	owl = ww_new_animation( delay, owl0_frame, owl1_frame, owl2_frame, NULL );
	
}
