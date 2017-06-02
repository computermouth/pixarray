
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <SDL2/SDL.h>

typedef void* window_t;
typedef unsigned char* pixel_buffer_t;
typedef unsigned char rgba_t[3];

typedef struct{
	SDL_Window* sdl_window;
	SDL_Surface* sdl_surface;
	SDL_Renderer* sdl_renderer;
	SDL_Texture* sdl_texture;
	int width;
	int height;
	int pitch;
	int received_quit_event;
} window_s;

int window_create(window_t *window, char* title, int width, int height) {

	window_s *window_p = ( window_s* ) calloc( 1, sizeof( window_s ) );

	if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
		printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
		return -1;
	}

	window_p->width = width;
	window_p->width = height;
	window_p->sdl_window = SDL_CreateWindow( title,
								SDL_WINDOWPOS_UNDEFINED,
								SDL_WINDOWPOS_UNDEFINED,
								width, height, SDL_WINDOW_SHOWN );
	if(!window_p->sdl_window) {
		printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
		return -1;
	}
	window_p->sdl_renderer = SDL_CreateRenderer( window_p->sdl_window, -1, SDL_RENDERER_ACCELERATED ); //SDL_RENDERER_SOFTWARE
	
	if(!window_p->sdl_renderer) {
		printf( "Renderer could not be created! SDL_Error: %s\n", SDL_GetError() );
		return -1;
	}

	window_p->sdl_texture = SDL_CreateTexture( window_p->sdl_renderer,
		SDL_PIXELFORMAT_BGR888,
		SDL_TEXTUREACCESS_STREAMING,
		width, height );

	if(!window_p->sdl_texture) {
		printf( "Screen texture could not be created! SDL_Error: %s\n", SDL_GetError() );
		return -1;
	}

	SDL_RenderClear( window_p->sdl_renderer );
	SDL_RenderPresent( window_p->sdl_renderer );

	*window = (window_t *) window_p;
	return 0;
}

int window_destroy(window_t window) {
	if(!window) {
		fprintf( stderr, "Trying to destroy non-existent window\n" );
		return -1;
	}
	
	window_s *window_p = (window_s*) window;

	if(window_p->sdl_texture) SDL_DestroyTexture( window_p->sdl_texture );
	if(window_p->sdl_renderer) SDL_DestroyRenderer( window_p->sdl_renderer );
	if(window_p->sdl_window) SDL_DestroyWindow( window_p->sdl_window );
	free(window);

	return 0;
}

int window_update_events(window_t window) {
	if(!window) {
		fprintf( stderr, "Cannot update non-existent window\n" );
		return -1;
	}

	window_s *window_p = (window_s*) window;

	SDL_Event event;
	while(SDL_PollEvent(&event)) {
		switch(event.type) {
			case SDL_QUIT:
				window_p->received_quit_event = 1;
			break;
		}
	}
	return 0;
}

int window_received_quit_event(window_t window) {
	if(!window) {
		fprintf( stderr, "Cannot update non-existent window\n" );
		return -1;
	}

	window_s *window_p = (window_s*) window;
	return window_p->received_quit_event;
}

int window_update_buffer(window_t window, pixel_buffer_t pixels) {
	if(!window) {
		return -1;
	}
	window_s *window_p = (window_s*) window;

	void *texture_pixels = NULL;
	int pitch = 0;
	if(SDL_LockTexture( window_p->sdl_texture, NULL, &texture_pixels, &pitch )) {
		fprintf( stderr, "Could not unlock texture: %s\n", SDL_GetError() );
		return -1;
	}
	memcpy( texture_pixels, pixels, pitch*window_p->width );
    SDL_UnlockTexture( window_p->sdl_texture );

    SDL_RenderClear( window_p->sdl_renderer );
    SDL_RenderCopy( window_p->sdl_renderer, window_p->sdl_texture, NULL, NULL );
    SDL_RenderPresent( window_p->sdl_renderer );

	return 0;
}

void memset_pixel( void * dest, void * value, size_t size ){
	for (uint i = 0; i < size; i++)
		memcpy(dest+(i*4), value, 3);
}

int main( void ) {
	window_t window = NULL;
	
	pixel_buffer_t buffer = calloc(640 * 480, sizeof(int));
	
	rgba_t pink = { 0xa5, 0x54, 0x90 };
	memset_pixel(buffer, &pink, (640*480));
	
	rgba_t blue = { 0x00, 0x54, 0x90 };
	memset_pixel(buffer, &blue, (640*240));
	
	rgba_t green = { 0x00, 0xff, 0x90 };
	memset_pixel(buffer + (640 * 155 * 4), &green, (640*1));
	memset_pixel(buffer + (640 * 175 * 4), &green, (640*2));
	memset_pixel(buffer + (640 * 195 * 4), &green, (640*4));
	memset_pixel(buffer + (640 * 215 * 4), &green, (640*8));
	
	if(window_create(&window, "Testing", 640, 480)) {
		printf("Failure\n");
		return 1;
	}
	
	while(!window_received_quit_event(window)) {
		window_update_events(window);
		window_update_buffer(window, buffer);
	}
	
	window_destroy(window);
	return 0;
}
