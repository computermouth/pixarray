
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <wchar.h>

#include <SDL2/SDL.h>

#ifndef _WW_H_
#define _WW_H_

typedef void* ww_window_t;
typedef unsigned char* ww_pixel_buffer_t;
typedef unsigned char ww_rgba_t[3];

typedef struct{
	SDL_Window* ww_sdl_window;
	SDL_Renderer* ww_sdl_renderer;
	SDL_Texture* ww_sdl_texture;
	int ww_width;
	int ww_height;
	int ww_default_width;
	int ww_default_height;
	int ww_pad_x;
	int ww_pad_y;
	float ww_ratio;
	int ww_pitch;
	int ww_received_quit_event;
	int focus;
	int min;
	int fs;
} ww_window_s;

typedef struct {
	unsigned char color[3];
	short *x;
	short *y;
	float ratio;
	void *s_parent;
	int w_pad_x;
	int w_pad_y;
	int u_pad_x;
	int u_pad_y;
	short *scaled_x;
	short *scaled_y;
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
} ww_sprite_t;

typedef struct{
	unsigned char esc;
	unsigned char ent;
	unsigned char w;
	unsigned char a;
	unsigned char s;
	unsigned char d;
	unsigned char up;
	unsigned char dn;
	unsigned char lt;
	unsigned char rt;
} ww_keystate_t;

extern ww_window_t window;
extern ww_pixel_buffer_t buffer;
extern ww_keystate_t keystate;

ww_window_t window = NULL;
ww_pixel_buffer_t buffer = NULL;
ww_keystate_t keystate = {
	.esc = 0,
	.ent = 0,
	.w = 0,
	.a = 0,
	.s = 0,
	.d = 0,
	.up = 0,
	.dn = 0,
	.lt = 0,
	.rt = 0
};

int ww_window_create(char* title, int width, int height) {

	ww_window_s *window_p = ( ww_window_s* ) calloc( 1, sizeof( ww_window_s ) );
	
	buffer = calloc(width * height * 4, sizeof(char));
	
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
		printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
		return -1;
	}

	window_p->ww_width = width;
	window_p->ww_height = height;
	window_p->ww_default_width = width;
	window_p->ww_default_height = height;
	window_p->ww_ratio = 1.0;
	window_p->ww_sdl_window = SDL_CreateWindow( title,
								SDL_WINDOWPOS_CENTERED,
								SDL_WINDOWPOS_CENTERED,
								width, height, 
								SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE );
	
	if(!window_p->ww_sdl_window) {
		printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
		return -1;
	}
	window_p->ww_sdl_renderer = SDL_CreateRenderer( window_p->ww_sdl_window, -1, 
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC ); //SDL_RENDERER_SOFTWARE
	
	if(!window_p->ww_sdl_renderer) {
		printf( "Renderer could not be created! SDL_Error: %s\n", SDL_GetError() );
		return -1;
	}

	window_p->ww_sdl_texture = SDL_CreateTexture( window_p->ww_sdl_renderer,
		SDL_PIXELFORMAT_BGR888,
		SDL_TEXTUREACCESS_TARGET,
		width, height );

	if(!window_p->ww_sdl_texture) {
		printf( "Screen texture could not be created! SDL_Error: %s\n", SDL_GetError() );
		return -1;
	}

	SDL_RenderClear( window_p->ww_sdl_renderer );
	SDL_RenderPresent( window_p->ww_sdl_renderer );
	
	SDL_SetRenderTarget(window_p->ww_sdl_renderer, window_p->ww_sdl_texture);

	window = (ww_window_t *) window_p;
	return 0;
}

int ww_window_destroy() {
	if(!window) {
		fprintf( stderr, "Trying to destroy non-existent window\n" );
		return -1;
	}
	
	ww_window_s *window_p = (ww_window_s*) window;

	if(window_p->ww_sdl_texture) SDL_DestroyTexture( window_p->ww_sdl_texture );
	if(window_p->ww_sdl_renderer) SDL_DestroyRenderer( window_p->ww_sdl_renderer );
	if(window_p->ww_sdl_window) SDL_DestroyWindow( window_p->ww_sdl_window );
	free(window);
	free(buffer);
	
	SDL_Quit();

	return 0;
}

int ww_window_event(SDL_Event *event){
	
	ww_window_s *window_p = (ww_window_s*) window;
	int rc = 0;
	
	const Uint8* currentKeyStates = SDL_GetKeyboardState( NULL );
	if( event->type == SDL_WINDOWEVENT )
	{
		
		switch( event->window.event )
		{
			case SDL_WINDOWEVENT_SIZE_CHANGED:
				window_p->ww_width = event->window.data1;
				window_p->ww_height = event->window.data2;
				
				if(((float)window_p->ww_width / (float)window_p->ww_default_width) >= 
					((float)window_p->ww_height / (float)window_p->ww_default_height)){
					//larger width ratio
					window_p->ww_ratio = ((float)window_p->ww_height / 
						(float)window_p->ww_default_height);
					window_p->ww_pad_x = ((window_p->ww_width - 
						(window_p->ww_default_width * window_p->ww_ratio))) / 2;
					window_p->ww_pad_y	= 0;
				}else{
					//larger height ratio
					window_p->ww_ratio = ((float)window_p->ww_width / 
						(float)window_p->ww_default_width);
					window_p->ww_pad_x	= 0;
					window_p->ww_pad_y	= ((window_p->ww_height - 
						(window_p->ww_default_height * window_p->ww_ratio))) / 2;
				}
				
				// should realloc
				free(buffer);
				buffer = calloc(window_p->ww_width * window_p->ww_height * 4, sizeof(char));
				
				if(buffer == NULL) {
					printf( "Buffer could not be resized!\n" );
					return -1;
				}
				
				if(window_p->ww_sdl_texture) SDL_DestroyTexture( window_p->ww_sdl_texture );
				
				window_p->ww_sdl_texture = SDL_CreateTexture( window_p->ww_sdl_renderer,
					SDL_PIXELFORMAT_BGR888,
					SDL_TEXTUREACCESS_STREAMING,
					window_p->ww_width, window_p->ww_height );
				
				if(!window_p->ww_sdl_texture) {
					printf( "Screen texture could not be created! SDL_Error: %s\n", SDL_GetError() );
					return -1;
				}
				
				break;
				
			case SDL_WINDOWEVENT_FOCUS_GAINED:
				window_p->focus = 1;
				break;

			case SDL_WINDOWEVENT_FOCUS_LOST:
				window_p->focus = 0;
				break;

			case SDL_WINDOWEVENT_MINIMIZED:
				window_p->min = 1;
				break;

			case SDL_WINDOWEVENT_MAXIMIZED:
				window_p->min = 0;
				break;
			
			case SDL_WINDOWEVENT_RESTORED:
				window_p->min = 0;
				break;
		}
	}
	if(event->type == SDL_KEYDOWN ){
		if( ((currentKeyStates[SDL_SCANCODE_RETURN]) && 
		((currentKeyStates[SDL_SCANCODE_RALT]) || 
		(currentKeyStates[SDL_SCANCODE_LALT]))) ){
			if( window_p->fs ){
				SDL_SetWindowFullscreen( window_p->ww_sdl_window, SDL_FALSE );
				window_p->fs = 0;
			}else{
				SDL_SetWindowFullscreen( window_p->ww_sdl_window, 
					SDL_WINDOW_FULLSCREEN_DESKTOP );
				window_p->fs = 1;
			}
			
			rc = 1;
		}
	}
	
	return rc;
}

void ww_key_event(SDL_Event *event){
	
	if( event->type == SDL_KEYDOWN ){
		switch(event->key.keysym.sym){
			case SDLK_ESCAPE:
				keystate.esc = 1;
				break;
			case SDLK_RETURN:
				keystate.ent = 1;
				break;
			case SDLK_UP:
				keystate.up = 1;
				break;
			case SDLK_DOWN:
				keystate.dn = 1;
				break;
			case SDLK_LEFT:
				keystate.lt = 1;
				break;
			case SDLK_RIGHT:
				keystate.rt = 1;
				break;
			case SDLK_w:
				keystate.w = 1;
				break;
			case SDLK_a:
				keystate.a = 1;
				break;
			case SDLK_s:
				keystate.s = 1;
				break;
			case SDLK_d:
				keystate.d = 1;
				break;
		}
	} else if ( event->type == SDL_KEYUP) {
		switch(event->key.keysym.sym){
			case SDLK_ESCAPE:
				keystate.esc = 0;
				break;
			case SDLK_RETURN:
				keystate.ent = 0;
				break;
			case SDLK_UP:
				keystate.up = 0;
				break;
			case SDLK_DOWN:
				keystate.dn = 0;
				break;
			case SDLK_LEFT:
				keystate.lt = 0;
				break;
			case SDLK_RIGHT:
				keystate.rt = 0;
				break;
			case SDLK_w:
				keystate.w = 0;
				break;
			case SDLK_a:
				keystate.a = 0;
				break;
			case SDLK_s:
				keystate.s = 0;
				break;
			case SDLK_d:
				keystate.d = 0;
				break;
		}
	}
}

int ww_window_update_events(){
	if(!window) {
		fprintf( stderr, "Cannot update non-existent window\n" );
		return -1;
	}

	ww_window_s *window_p = (ww_window_s*) window;

	SDL_Event event;
	while(SDL_PollEvent(&event)) {
		switch(event.type){
			case SDL_QUIT:
				window_p->ww_received_quit_event = 1;
				break;
			default:
				if ( ! ww_window_event(&event))
					ww_key_event(&event);
				break;
		}
	}
	return 0;
}

int ww_window_received_quit_event() {
	if(!window) {
		fprintf( stderr, "Cannot update non-existent window\n" );
		return -1;
	}

	ww_window_s *window_p = (ww_window_s*) window;
	return window_p->ww_received_quit_event;
}

// DEAD CODE
void ww_draw_pixel( uint32_t x, uint32_t y, unsigned char color[3] ){
	ww_window_s *window_p = (ww_window_s*) window;
	
	memcpy(buffer + (((window_p->ww_width * y) + x ) * 4), color, 3);
	
}

// DEAD CODE
void ww_draw_pixel_range( uint32_t x, uint32_t y, unsigned char color[3], size_t size){
	for (uint32_t i = 0; i < size; i++)
		ww_draw_pixel( x + i, y, color);
}

//~ static inline void ww_draw_hline( int32_t x1, int32_t x2, int32_t y, unsigned char color[3]){
	//~ ww_window_s *window_p = (ww_window_s*) window;
	
	//~ if( x1 < 0 )
		//~ x1 = 0;
	//~ if( x2 > window_p->ww_width - 1 )
		//~ x2 = window_p->ww_width - 1;
	//~ if( y  > window_p->ww_height - 1 || y < 0 || x2 <= x1 ){
		//~ return;
	//~ }
	
	//~ wchar_t * fake_wchar = (wchar_t *)color;
	//~ wmemset( (wchar_t *)(buffer + (((window_p->ww_width * y) + x1 ) * 4)), *fake_wchar, x2 - x1 );
	
	//~ SDL_SetRenderDrawColor(window_p->ww_sdl_renderer, color[0], color[1], color[2], 255);
	//~ SDL_RenderDrawLine(window_p->ww_sdl_renderer, x1, y, x2, y);
	
//~ }

int _gfxPrimitivesCompareInt(const void *a, const void *b)
{
	return (*(const int *) a) - (*(const int *) b);
}

int ww_draw_raw_polygon(const Sint16 * vx, const Sint16 * vy, int n, unsigned char color[3])
{
	ww_window_s *window_p = (ww_window_s*) window;
	
	int result;
	int i;
	int y, xa, xb;
	int miny, maxy;
	int x1, y1;
	int x2, y2;
	int ind1, ind2;
	int ints;
	int *gfxPrimitivesPolyInts = (int *) malloc(sizeof(int) * n);
	
	if (vx == NULL || vy == NULL || n < 3) {
		return -1;
	}
	
	miny = vy[0];
	maxy = vy[0];
	for (i = 1; (i < n); i++) {
		if (vy[i] < miny) {
			miny = vy[i];
		} else if (vy[i] > maxy) {
			maxy = vy[i];
		}
	}
	
	result = 0;
	for (y = miny; y <= maxy; y++) {
		ints = 0;
		for (i = 0; (i < n); i++) {
			if (!i) {
				ind1 = n - 1;
				ind2 = 0;
			} else {
				ind1 = i - 1;
				ind2 = i;
			}
			y1 = vy[ind1];
			y2 = vy[ind2];
			if (y1 < y2) {
				x1 = vx[ind1];
				x2 = vx[ind2];
			} else if (y1 > y2) {
				y2 = vy[ind1];
				y1 = vy[ind2];
				x2 = vx[ind1];
				x1 = vx[ind2];
			} else {
				continue;
			}
			if ( ((y >= y1) && (y < y2)) || ((y == maxy) && (y > y1) && (y <= y2)) ) {
				gfxPrimitivesPolyInts[ints++] = ((65536 * (y - y1)) / (y2 - y1)) * (x2 - x1) + (65536 * x1);
			} 	    
		}

		qsort(gfxPrimitivesPolyInts, ints, sizeof(int), _gfxPrimitivesCompareInt);
		
		result = 0;

		for (i = 0; (i < ints); i += 2) {
			xa = gfxPrimitivesPolyInts[i] + 1;
			xa = (xa >> 16) + ((xa & 32768) >> 15);
			xb = gfxPrimitivesPolyInts[i+1] - 1;
			xb = (xb >> 16) + ((xb & 32768) >> 15);
			
			SDL_SetRenderDrawColor(window_p->ww_sdl_renderer, color[0], color[1], color[2], 255);
			SDL_RenderDrawLine(window_p->ww_sdl_renderer, xa, y, xb, y);
			
			//~ ww_draw_hline( xa, xb, y, color);
		}
	}
	
	free(gfxPrimitivesPolyInts);
	
	return (result);
}

void ww_scale_polygon(ww_polygon_t * poly){
	ww_window_s *window_p = (ww_window_s*) window;
	
	ww_sprite_t * parent = (ww_sprite_t *)(poly->s_parent);
	
	if( poly->ratio != window_p->ww_ratio || 
		poly->w_pad_x != window_p->ww_pad_x ||
		poly->w_pad_y != window_p->ww_pad_y ||
		poly->u_pad_x != parent->pad_x ||
		poly->u_pad_y != parent->pad_y ){
		
		poly->ratio = window_p->ww_ratio;
		poly->w_pad_x = window_p->ww_pad_x;
		poly->w_pad_y = window_p->ww_pad_y;
		poly->u_pad_x = parent->pad_x;
		poly->u_pad_y = parent->pad_y;
		
		
		for(int i = 0; i < poly->count; i++){
			poly->scaled_x[i] = (poly->x[i] ) * poly->ratio + (poly->w_pad_x + poly->u_pad_x);
			poly->scaled_y[i] = (poly->y[i] ) * poly->ratio + (poly->w_pad_y + poly->u_pad_y);
		}
	
	}
	
}

int ww_draw_polygon(ww_polygon_t * poly){
	
	ww_scale_polygon(poly);
	
	return ww_draw_raw_polygon(poly->scaled_x, poly->scaled_y, poly->count, poly->color);
}

int ww_draw_frame(ww_frame_t * frame){
	
	int rc = 0;
	
	// TODO implement z-depth sorted draws
	for(int i = 0; i < frame->count; i++){
		rc += ww_draw_polygon(frame->polys[i]);
	}
	
	return rc;
	
}

int ww_draw_animation(ww_animation_t * anim){
	
	int rc = ww_draw_frame(anim->frames[anim->active_frame]);
	
	if(anim->d_progress == 0){
		anim->active_frame++;
		
		if(anim->active_frame == anim->count)
			anim->active_frame = 0;
		
		anim->d_progress = anim->delay[anim->active_frame];
		
	} else {
		anim->d_progress--;
	}
	
	return rc;
	
}

int ww_draw_sprite(ww_sprite_t * sprite){
	
	int rc = ww_draw_animation(sprite->animations[sprite->active_animation]);
	
	return rc;
	
}

ww_polygon_t * ww_new_polygon(unsigned char color[3], short * x, short * y, int count){
	
	ww_polygon_t * poly = calloc(1, sizeof(ww_polygon_t));
	poly->x = calloc(count, sizeof(short));
	poly->y = calloc(count, sizeof(short));
	memcpy(poly->x, x, count*sizeof(short));
	memcpy(poly->y, y, count*sizeof(short));
	
	poly->scaled_x = calloc(count, sizeof(short));
	poly->scaled_y = calloc(count, sizeof(short));
	memcpy(poly->scaled_x, x, count*sizeof(short));
	memcpy(poly->scaled_y, y, count*sizeof(short));
	
	memcpy(poly->color, color, 3);
	poly->count = count;
	
	return poly;
}

ww_frame_t * ww_new_frame(ww_polygon_t * polys, ...){
	
	ww_frame_t * frame = NULL;
	
	if(polys == NULL){
		printf("ww_new_frame called with only NULL argument");
		return NULL;
	}
	
	frame = calloc(1, sizeof(ww_frame_t));
	
	// count items
	va_list args;
	ww_polygon_t * tmp = polys;
	
	va_start(args, polys);
	while( tmp != NULL ){
		frame->count += 1;
		tmp = va_arg(args, ww_polygon_t *);
	}
	va_end(args);
	
	// alloc
	frame->polys = calloc(frame->count, sizeof(ww_polygon_t *));
	
	// assign
	int i = 1;
	tmp = polys;
	frame->polys[0] = polys;
	va_start(args, polys);
	while( 1 ){
		tmp = va_arg(args, ww_polygon_t *);
		
		if(tmp == NULL)
			break;
		
		frame->polys[i] = tmp;
		i++;
	}
	va_end(args);
	
	return frame;
	
}

ww_animation_t * ww_new_animation(int * delay, ww_frame_t * frames, ...){
	
	ww_animation_t * anim = NULL;
	
	if(frames == NULL){
		printf("ww_new_animation called with only NULL argument");
		return NULL;
	}
	
	anim = calloc(1, sizeof(ww_animation_t));
	
	// count items
	va_list args;
	ww_frame_t * tmp = frames;
	
	va_start(args, frames);
	while( tmp != NULL ){
		anim->count += 1;
		tmp = va_arg(args, ww_frame_t *);
	}
	va_end(args);
	
	// alloc
	anim->frames = calloc(anim->count, sizeof(ww_frame_t *));
	
	// assign
	anim->delay = calloc(anim->count, sizeof(int));
	memcpy(anim->delay, delay, anim->count*sizeof(int));
	anim->d_progress = anim->delay[0];
	
	int i = 1;
	tmp = frames;
	anim->frames[0] = frames;
	va_start(args, frames);
	
	while( 1 ){
		tmp = va_arg(args, ww_frame_t *);
		
		if(tmp == NULL)
			break;
		
		anim->frames[i] = tmp;
		i++;
	}
	va_end(args);
	
	return anim;
}

ww_sprite_t * ww_new_sprite(int depth, ww_animation_t * animations, ...){
	
	ww_sprite_t * sprite = NULL;
	
	if(animations == NULL){
		printf("ww_new_animation called with only NULL argument");
		return NULL;
	}
	
	sprite = calloc(1, sizeof(ww_sprite_t));
	
	// count items
	va_list args;
	ww_animation_t * tmp = animations;
	
	va_start(args, animations);
	while( tmp != NULL ){
		sprite->count += 1;
		tmp = va_arg(args, ww_animation_t *);
	}
	va_end(args);
	
	// alloc
	sprite->animations = calloc(sprite->count, sizeof(ww_animation_t *));
	
	// assign
	sprite->z_depth = depth;
	
	int i = 1;
	tmp = animations;
	sprite->animations[0] = animations;
	va_start(args, animations);
	
	while( 1 ){
		tmp = va_arg(args, ww_animation_t *);
		
		if(tmp == NULL)
			break;
		
		sprite->animations[i] = tmp;
		i++;
	}
	va_end(args);
	
	for(int s = 0; s < sprite->count; s++){
		
		for(int a = 0; a < sprite->animations[s]->count; a++){
		
			for(int f = 0; f < sprite->animations[s]->frames[a]->count; f++){
			
				sprite->animations[s]->frames[a]->polys[f]->s_parent = sprite;
				
				ww_polygon_t * t_poly = sprite->animations[s]->frames[a]->polys[f];
				
				for(int sh = 0; sh < t_poly->count; sh++){
					if(t_poly->x[sh] > sprite->width)
						sprite->width = t_poly->x[sh];
					if(t_poly->y[sh] > sprite->height)
						sprite->height = t_poly->y[sh];
				}
				
			}	
		}	
	}
	
	return sprite;
}

void ww_free_polygon(ww_polygon_t * poly){
	free(poly->x);
	free(poly->y);
	free(poly->scaled_x);
	free(poly->scaled_y);
	free(poly);
}

void ww_free_frame(ww_frame_t * frame){
	
	for(int i = 0; i < frame->count; i++){
		ww_free_polygon( frame->polys[i] );
	}
	
	free(frame->polys);
	free(frame);
	
}

void ww_free_anim(ww_animation_t * anim){
	
	for(int i = 0; i < anim->count; i++){
		ww_free_frame( anim->frames[i] );
	}
	
	free(anim->delay);
	free(anim->frames);
	free(anim);
	
}

void ww_free_sprite(ww_sprite_t * sprite){
	
	for(int i = 0; i < sprite->count; i++){
		ww_free_anim( sprite->animations[i] );
	}
	
	free(sprite->animations);
	free(sprite);
	
}

void ww_render_bars(){
	
	ww_window_s *window_p = (ww_window_s*) window;
	
	unsigned char bar_grey[] = { 0x1C, 0x1F, 0x20 };
	
	if (window_p->ww_pad_x != 0){
		// left
		short bar_ax[4] = { 0, window_p->ww_pad_x, window_p->ww_pad_x, 0 };
		short bar_ay[4] = { 0, 0, window_p->ww_height, window_p->ww_height };
		ww_draw_raw_polygon(bar_ax, bar_ay, 4, bar_grey);
		// right
		short bar_bx[4] = { window_p->ww_width - window_p->ww_pad_x, window_p->ww_width, window_p->ww_width, window_p->ww_width - window_p->ww_pad_x};
		short bar_by[4] = { 0, 0, window_p->ww_height, window_p->ww_height };
		ww_draw_raw_polygon(bar_bx, bar_by, 4, bar_grey);
		
	} else {
		// top
		short bar_ax[4] = { 0, window_p->ww_width,  window_p->ww_width, 0 };
		short bar_ay[4] = { 0, 0, window_p->ww_pad_y, window_p->ww_pad_y};
		ww_draw_raw_polygon(bar_ax, bar_ay, 4, bar_grey);
		// bottom
		short bar_bx[4] = { 0, window_p->ww_width, window_p->ww_width, 0 };
		short bar_by[4] = { window_p->ww_height - window_p->ww_pad_y, window_p->ww_height - window_p->ww_pad_y,  window_p->ww_height,  window_p->ww_height};
		ww_draw_raw_polygon(bar_bx, bar_by, 4, bar_grey);
		
	}
	
}

void ww_clear_buffer(){
	
	ww_window_s *window_p = (ww_window_s*) window;
	
	SDL_SetRenderDrawColor(window_p->ww_sdl_renderer, 0, 0, 0, 255);
	SDL_RenderClear(window_p->ww_sdl_renderer);
	//~ memset(buffer, 0x00, window_p->ww_width * window_p->ww_height * 4 * sizeof(char));

}

int ww_window_update_buffer() {
	if(!window) {
		return -1;
	}
	ww_window_s *window_p = (ww_window_s*) window;

	ww_render_bars();

	//~ void *texture_pixels = NULL;
	//~ window_p->ww_pitch = 0;
	
	//~ if(SDL_LockTexture( window_p->ww_sdl_texture, NULL, &texture_pixels, &window_p->ww_pitch )) {
		//~ fprintf( stderr, "Could not lock texture: %s\n", SDL_GetError() );
		//~ return -1;
	//~ }
	
	//~ memcpy( texture_pixels, buffer, window_p->ww_pitch*window_p->ww_height );
    //~ SDL_UnlockTexture( window_p->ww_sdl_texture );
    
	SDL_SetRenderTarget(window_p->ww_sdl_renderer, NULL);
    
    SDL_RenderCopy( window_p->ww_sdl_renderer, window_p->ww_sdl_texture, NULL, NULL );
    SDL_RenderPresent( window_p->ww_sdl_renderer );
	
	SDL_SetRenderTarget(window_p->ww_sdl_renderer, window_p->ww_sdl_texture);
	
	return 0;
}

#endif
