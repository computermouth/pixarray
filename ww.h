
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <SDL2/SDL.h>

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
	window_p->ww_sdl_renderer = SDL_CreateRenderer( window_p->ww_sdl_window, -1, SDL_RENDERER_ACCELERATED ); //SDL_RENDERER_SOFTWARE
	
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
	int ww_pitch = 0;
	if(SDL_LockTexture( window_p->ww_sdl_texture, NULL, &texture_pixels, &ww_pitch )) {
		fprintf( stderr, "Could not unlock texture: %s\n", SDL_GetError() );
		return -1;
	}
	memcpy( texture_pixels, buffer, ww_pitch*window_p->ww_width );
    SDL_UnlockTexture( window_p->ww_sdl_texture );

    SDL_RenderClear( window_p->ww_sdl_renderer );
    SDL_RenderCopy( window_p->ww_sdl_renderer, window_p->ww_sdl_texture, NULL, NULL );
    SDL_RenderPresent( window_p->ww_sdl_renderer );

	return 0;
}

void ww_memset_pixel( uint32_t x, uint32_t y, void * value ){
	ww_window_s *window_p = (ww_window_s*) window;
	
	memcpy(buffer + (((window_p->ww_width * y) + x ) * 4), value, 3);
}

void ww_memset_pixel_range( uint32_t x, uint32_t y, void * value, size_t size){
	for (uint32_t i = 0; i < size; i++)
		ww_memset_pixel( x + i, y, value);
}

void ww_hline( int32_t x1, int32_t x2, int32_t y, void * value){
	ww_window_s *window_p = (ww_window_s*) window;
	
	if( x1 < 0 )
		x1 = 0;
	if( x2 > window_p->ww_width )
		x2 = window_p->ww_width - 1;
	if( y  > window_p->ww_height || y < 0 || x2 <= x1 ){
		printf("nope\n");
		return;
	}
	
	ww_memset_pixel_range( x1, y, value, ( x2 - x1 ) );
	
}
