
#include "ww.h"

ww_rgba_t pink = { 0xa5, 0x54, 0x90 };
short pentagon_x[5] = { 290, 380, 500, 480, 310};
short pentagon_y[5] = { 270, 160, 270, 410, 420};
extern ww_polygon_t * pink_pentagon;
ww_polygon_t * pink_pentagon = NULL;

void test_shapes_init(){
	pink_pentagon = ww_new_polygon(pink, pentagon_x, pentagon_y, 5);
}
