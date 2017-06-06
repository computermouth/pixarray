
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <SDL2/SDL.h>

#ifndef _WW_H_
#define _WW_H_

typedef void* ww_window_t;
typedef unsigned char* ww_pixel_buffer_t;
typedef unsigned char ww_rgba_t[3];

typedef struct{
	SDL_Window* ww_sdl_window;
	SDL_Surface* ww_sdl_surface;
	SDL_Renderer* ww_sdl_renderer;
	SDL_Texture* ww_sdl_texture;
	int ww_width;
	int ww_height;
	int ww_pitch;
	int ww_received_quit_event;
} ww_window_s;

typedef struct {
	unsigned char color[3];
	short *x;
	short *y;
	int points;
} ww_polygon_t;

extern ww_window_t window;
extern ww_pixel_buffer_t buffer;

ww_window_t window = NULL;
ww_pixel_buffer_t buffer = NULL;

int ww_window_create(char* title, int width, int height) {

	ww_window_s *window_p = ( ww_window_s* ) calloc( 1, sizeof( ww_window_s ) );
	
	buffer = calloc(width * height * 4, sizeof(char));
	
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
		printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
		return -1;
	}

	window_p->ww_width = width;
	window_p->ww_height = height;
	window_p->ww_sdl_window = SDL_CreateWindow( title,
								SDL_WINDOWPOS_UNDEFINED,
								SDL_WINDOWPOS_UNDEFINED,
								width, height, SDL_WINDOW_SHOWN );
	if(!window_p->ww_sdl_window) {
		printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
		return -1;
	}
	window_p->ww_sdl_renderer = SDL_CreateRenderer( window_p->ww_sdl_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC ); //SDL_RENDERER_SOFTWARE
	
	if(!window_p->ww_sdl_renderer) {
		printf( "Renderer could not be created! SDL_Error: %s\n", SDL_GetError() );
		return -1;
	}

	window_p->ww_sdl_texture = SDL_CreateTexture( window_p->ww_sdl_renderer,
		SDL_PIXELFORMAT_BGR888,
		SDL_TEXTUREACCESS_STREAMING,
		width, height );

	if(!window_p->ww_sdl_texture) {
		printf( "Screen texture could not be created! SDL_Error: %s\n", SDL_GetError() );
		return -1;
	}

	SDL_RenderClear( window_p->ww_sdl_renderer );
	SDL_RenderPresent( window_p->ww_sdl_renderer );

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

int ww_window_update_events() {
	if(!window) {
		fprintf( stderr, "Cannot update non-existent window\n" );
		return -1;
	}

	ww_window_s *window_p = (ww_window_s*) window;

	SDL_Event event;
	while(SDL_PollEvent(&event)) {
		switch(event.type) {
			case SDL_QUIT:
				window_p->ww_received_quit_event = 1;
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

int ww_window_update_buffer() {
	if(!window) {
		return -1;
	}
	ww_window_s *window_p = (ww_window_s*) window;

	void *texture_pixels = NULL;
	window_p->ww_pitch = 0;
	if(SDL_LockTexture( window_p->ww_sdl_texture, NULL, &texture_pixels, &window_p->ww_pitch )) {
		fprintf( stderr, "Could not lock texture: %s\n", SDL_GetError() );
		return -1;
	}
	
	memcpy( texture_pixels, buffer, window_p->ww_pitch*window_p->ww_height );
    SDL_UnlockTexture( window_p->ww_sdl_texture );

    //~ SDL_RenderClear( window_p->ww_sdl_renderer );
    SDL_RenderCopy( window_p->ww_sdl_renderer, window_p->ww_sdl_texture, NULL, NULL );
    SDL_RenderPresent( window_p->ww_sdl_renderer );

	return 0;
}

void ww_draw_pixel( uint32_t x, uint32_t y, void * value ){
	ww_window_s *window_p = (ww_window_s*) window;
	
	memcpy(buffer + (((window_p->ww_width * y) + x ) * 4), value, 3);
}

void ww_draw_pixel_range( uint32_t x, uint32_t y, void * value, size_t size){
	for (uint32_t i = 0; i < size; i++)
		ww_draw_pixel( x + i, y, value);
}

void ww_draw_hline( int32_t x1, int32_t x2, int32_t y, void * value){
	ww_window_s *window_p = (ww_window_s*) window;
	
	if( x1 < 0 )
		x1 = 0;
	if( x2 > window_p->ww_width )
		x2 = window_p->ww_width - 1;
	if( y  > window_p->ww_height || y < 0 ||x2 <= x1 ){
		return;
	}
	
	ww_draw_pixel_range( x1, y, value, ( x2 - x1 ) );
	
}

int _gfxPrimitivesCompareInt(const void *a, const void *b)
{
	return (*(const int *) a) - (*(const int *) b);
}

int ww_draw_raw_polygon(const Sint16 * vx, const Sint16 * vy, int n, void * color)
{
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
	for (y = miny; (y <= maxy); y++) {
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
			ww_draw_hline( xa, xb, y, color);
		}
	}
	
	free(gfxPrimitivesPolyInts);
	
	return (result);
}

int ww_draw_polygon(ww_polygon_t * poly){
	return ww_draw_raw_polygon(poly->x, poly->y, poly->points, poly->color);
}

ww_polygon_t * ww_new_polygon(unsigned char color[3], short * x, short * y, int points){
	
	ww_polygon_t * poly = calloc(1, sizeof(ww_polygon_t));
	poly->x = calloc(points, sizeof(short));
	poly->y = calloc(points, sizeof(short));
	memcpy(poly->x, x, points*sizeof(short));
	memcpy(poly->y, y, points*sizeof(short));
	memcpy(poly->color, color, 3);
	poly->points = points;
	
	return poly;
}

void ww_free_polygon(ww_polygon_t * poly){
	free(poly->x);
	free(poly->y);
	free(poly);
}

#endif
