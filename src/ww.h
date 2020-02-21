
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

typedef struct {
	unsigned char color[3];
	short *x;
	short *y;
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
	ww_polygon_t ** polys;
	int count;
} ww_frame_t;

typedef struct {
	ww_frame_t ** frames;
	int *delay;
	int d_progress;
	int active_frame;
	int count;
} ww_animation_t;

typedef struct {
	ww_animation_t ** animations;
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

typedef struct{
	unsigned char str;
	unsigned char sel;
	unsigned char up;
	unsigned char dn;
	unsigned char lt;
	unsigned char rt;
	unsigned char ba;
	unsigned char bb;
	unsigned char bx;
	unsigned char by;
	unsigned char cfrm;
	unsigned char paus;
	unsigned char back;
} ww_istate_t;

extern ww_window_t window;
extern ww_istate_t istate;
extern ww_istate_t ipstate;

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
int ww_draw_raw_polygon(const Sint16 * vx, const Sint16 * vy, int n, unsigned char color[3]);
void ww_scale_polygon(ww_polygon_t * poly);
int ww_draw_polygon(ww_polygon_t * poly);
int ww_draw_frame(ww_frame_t * frame);
int ww_draw_animation(ww_animation_t * anim, int paused);
int ww_draw_sprite(ww_sprite_t * sprite);
ww_polygon_t * ww_new_polygon(unsigned char color[3], short * x, short * y, int count);
ww_frame_t * ww_new_frame(ww_polygon_t * polys, ...);
ww_animation_t * ww_new_animation(int * delay, ww_frame_t * frames, ...);
ww_sprite_t * ww_new_sprite(int depth, ww_animation_t * animations, ...);
ww_sprite_t * ww_clone_sprite(ww_sprite_t * in_sprite);
void ww_free_polygon(ww_polygon_t * poly);
void ww_free_frame(ww_frame_t * frame);
void ww_free_anim(ww_animation_t * anim);
void ww_free_sprite(ww_sprite_t * sprite);
void ww_render_bars();
void ww_clear_buffer();
int ww_window_update_buffer();

#endif
