
#include "ww.h"

extern ww_polygon_t * pink_pentagon;
ww_polygon_t * pink_pentagon = NULL;

extern ww_polygon_t * green_triangle;
ww_polygon_t * green_triangle = NULL;

void test_shapes_init(){
	ww_rgba_t pink = { 0xa5, 0x54, 0x90 };
	short pentagon_x[5] = { 290, 380, 500, 480, 310};
	short pentagon_y[5] = { 270, 160, 270, 410, 420};

	pink_pentagon = ww_new_polygon(pink, pentagon_x, pentagon_y, 5);
	
	ww_rgba_t green = { 0x81, 0xff, 0xa0 };
	short triangle_x[3] = {  38,  76, 105 };
	short triangle_y[3] = { 120,  10, 376 };
	
	green_triangle = ww_new_polygon(green, triangle_x, triangle_y, 3);
}
