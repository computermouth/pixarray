
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <wchar.h>

#include <SDL2/SDL.h>

#ifndef _WW_H_
#define _WW_H_

typedef void* ww_window_t;
typedef unsigned char ww_rgba_t[3];

#define	SC_SIXTEENTH .0625
#define	SC_EIGHTH .125
#define	SC_QUARTER .25
#define	SC_HALF .5
#define	SC_ONE 1.0
#define	SC_TWO 2.0
#define	SC_FOUR 4.0
#define	SC_EIGHT 8.0

typedef struct{
	char *gamename;
	SDL_Window* ww_sdl_window;
	SDL_Renderer* ww_sdl_renderer;
	SDL_Texture* ww_sdl_texture;
	unsigned int ticks;
	float framediff;
	int frames;
	int pf;
	int acc;
	int vsync;
	int ww_width;
	int ww_height;
	int ww_default_width;
	int ww_default_height;
	int ww_pad_x;
	int ww_pad_y;
	float ww_ratio;
	float ww_scale;
	int ww_pitch;
	int ww_received_quit_event;
	int ww_size_changed;
	int focus;
	int min;
	int fs;
} ww_window_s;

typedef struct{
	union { // start      || enter
		char c_str;
		char k_ent;
	};
	union { // select     || escape
		char c_sel;
		char k_esc;
	};
	union { // dpad up    || W
		char c_up;
		char k_w;
	};
	union { // dpad down  || S
		char c_dn;
		char k_s;
	};
	union { // dpad left  || A
		char c_lt;
		char k_a;
	};
	union { // dpad right || S
		char c_rt;
		char k_d;
	};
	union { // A button   || down arrow
		char c_a;
		char k_dn;
	};
	union { // B button   || right arrow
		char c_b;
		char k_rt;
	};
	union { // X button   || left arrow
		char c_x;
		char k_lt;
	};
	union { // Y button   || up arrow
		char c_y;
		char k_up;
	};
	unsigned char cfrm;  // Start  || A button || Enter  || down arrow
	unsigned char paus;  // Select || Start    || Escape || Enter
	unsigned char back;  // Select || B button || Escape || right arrow
} ww_istate_t;

typedef struct {
	unsigned char color[3];
	short *x;
	short *y;
	short miny;
	short maxy;
	float ratio;
	void *s_parent; // pointer to sprite
	int w_pad_x; // window's pad
	int w_pad_y; // window's pad
	int s_pad_x; // sprite's pad
	int s_pad_y; // sprite's pad
	float s_scale; // sprite's scale
	float scale;
	short *scaled_x; // scale * ((x[i] + s_pad_x) * ratio + w_pad_x)
	short *scaled_y; // scale * ((y[i] + s_pad_y) * ratio + w_pad_y)
	int z_depth;
	int count;
} ww_polygon_t;

typedef struct {
	ww_polygon_t * polys;
	int count;
} ww_frame_t;

typedef struct {
	ww_frame_t * frames;
	int *delay;
	int d_progress;
	int active_frame;
	int count;
} ww_animation_t;

typedef struct {
	ww_animation_t * animations;
	int pad_x;
	int pad_y;
	int height;
	int width;
	int active_animation;
	int z_depth;
	int count;
	int paused;
	float scale;
} ww_sprite_t;

typedef struct {
	int * alloc;
	int * frames;
	int ** delays;
	int * polygons;
	unsigned char ** colors;
	int * vertices;
	short ** arrays;
} ww_reference_t;

extern ww_window_t window;
extern ww_istate_t istate;
extern ww_istate_t ipstate;

ww_sprite_t * ww_new_sprite(ww_reference_t);
ww_sprite_t * ww_clone_sprite(ww_sprite_t *);
int ww_draw_raw_polygon(const Sint16 *, const Sint16 *, int, short, short, unsigned char[3]);
void ww_scale_polygon(ww_polygon_t * poly);
int ww_draw_polygon(ww_polygon_t * poly);
int ww_draw_frame(ww_frame_t * frame);
int ww_draw_animation(ww_animation_t * anim, int paused);
int ww_draw_sprite(ww_sprite_t * sprite);


int ww_window_destroy();
int ww_window_create(int argc, char * argv[], char * title, int width, int height);
int ww_window_event(SDL_Event *event);
void ww_toggle_fs();
void ww_key_event(SDL_Event *event);
int ww_window_update_events();
int ww_frames_passed();
void ww_window_send_quit_event();
int ww_window_received_quit_event();
int _gfxPrimitivesCompareInt(const void *a, const void *b);
void ww_render_bars();
void ww_clear_buffer();
int ww_window_update_buffer();

#endif
