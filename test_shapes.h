
#include "ww.h"

extern ww_polygon_t * pink_pentagon;
ww_polygon_t * pink_pentagon = NULL;

extern ww_polygon_t * yellow_octagon;
ww_polygon_t * yellow_octagon = NULL;

extern ww_polygon_t * triangle0;
ww_polygon_t * triangle0 = NULL;
extern ww_polygon_t * triangle1;
ww_polygon_t * triangle1 = NULL;
extern ww_polygon_t * triangle2;
ww_polygon_t * triangle2 = NULL;
extern ww_polygon_t * triangle3;
ww_polygon_t * triangle3 = NULL;
extern ww_polygon_t * triangle4;
ww_polygon_t * triangle4 = NULL;
extern ww_polygon_t * triangle5;
ww_polygon_t * triangle5 = NULL;
extern ww_polygon_t * triangle6;
ww_polygon_t * triangle6 = NULL;
extern ww_polygon_t * triangle7;
ww_polygon_t * triangle7 = NULL;
extern ww_polygon_t * triangle8;
ww_polygon_t * triangle8 = NULL;
extern ww_polygon_t * triangle9;
ww_polygon_t * triangle9 = NULL;

void test_shapes_init(){
	
	ww_rgba_t pink = { 0xa5, 0x54, 0x90 };
	short pentagon_x[5] = { 290, 380, 500, 480, 310};
	short pentagon_y[5] = { 270, 160, 270, 410, 420};
	pink_pentagon = ww_new_polygon(pink, pentagon_x, pentagon_y, 5);
	
	ww_rgba_t yellow = { 0xff, 0xf5, 0x52 };
	short yellow_octagon_x[8] = { 589, 607, 621, 621, 611, 590, 580, 579};
	short yellow_octagon_y[8] = { 339, 339, 350, 371, 379, 380, 372, 349};
	yellow_octagon = ww_new_polygon(yellow, yellow_octagon_x, yellow_octagon_y, 8);
	
	ww_rgba_t green = { 0x81, 0xff, 0xa0 };
	short triangle0_x[3] = {  - 38,  76, 805 };
	short triangle0_y[3] = {  - 120,  10, 806 };
	triangle0 = ww_new_polygon(green, triangle0_x, triangle0_y, 3);
	
	ww_rgba_t purple = { 0x57, 0x00, 0x6d };
	short triangle1_x[3] = {  48,  86, 115 };
	short triangle1_y[3] = { 120,  10, 376 };
	triangle1 = ww_new_polygon(purple, triangle1_x, triangle1_y, 3);
	
	ww_rgba_t brown = { 0x55, 0x2a, 0x1d };
	short triangle2_x[3] = {  58,  96, 125 };
	short triangle2_y[3] = { 120,  10, 376 };
	triangle2 = ww_new_polygon(brown, triangle2_x, triangle2_y, 3);
	
	ww_rgba_t blue = { 0x48, 0xe3, 0xff };
	short triangle3_x[3] = {  68, 106, 135 };
	short triangle3_y[3] = { 120,  10, 376 };
	triangle3 = ww_new_polygon(blue, triangle3_x, triangle3_y, 3);
	
	ww_rgba_t hotp = { 0xff, 0x00, 0xf0 };
	short triangle4_x[3] = {  78, 116, 145 };
	short triangle4_y[3] = { 120,  10, 376 };
	triangle4 = ww_new_polygon(hotp, triangle4_x, triangle4_y, 3);
	
	ww_rgba_t grey = { 0x7c, 0x7c, 0x7c };
	short triangle5_x[3] = {  88, 126, 155 };
	short triangle5_y[3] = { 120,  10, 376 };
	triangle5 = ww_new_polygon(grey, triangle5_x, triangle5_y, 3);
	
	ww_rgba_t lime = { 0xb1, 0xff, 0x5d };
	short triangle6_x[3] = {  98, 136, 165 };
	short triangle6_y[3] = { 120,  10, 376 };
	triangle6 = ww_new_polygon(lime, triangle6_x, triangle6_y, 3);
	
	ww_rgba_t orange = { 0xff, 0x9c, 0x00 };
	short triangle7_x[3] = { 108, 146, 175 };
	short triangle7_y[3] = { 120,  10, 376 };
	triangle7 = ww_new_polygon(orange, triangle7_x, triangle7_y, 3);
	
	ww_rgba_t white = { 0xff, 0xff, 0xff };
	short triangle8_x[3] = { 118, 156, 185 };
	short triangle8_y[3] = { 120,  10, 376 };
	triangle8 = ww_new_polygon(white, triangle8_x, triangle8_y, 3);
	
	ww_rgba_t lavender = { 0xa7, 0x67, 0xb3 };
	short triangle9_x[3] = { 128, 166, 195 };
	short triangle9_y[3] = { 120,  10, 376 };
	triangle9 = ww_new_polygon(lavender, triangle9_x, triangle9_y, 3);

}
