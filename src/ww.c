

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <wchar.h>

#include <SDL2/SDL.h>

#include "ww.h"

ww_window_t window = NULL;

ww_istate_t istate = {
	.c_str  = 0,
	.c_sel  = 0,
	.c_up   = 0,
	.c_dn   = 0,
	.c_lt   = 0,
	.c_rt   = 0,
	.c_a    = 0,
	.c_b    = 0,
	.c_x    = 0,
	.c_y    = 0,
	.cfrm = 0,
	.paus = 0,
	.back = 0
};

ww_istate_t ipstate = {
	.c_str  = 0,
	.c_sel  = 0,
	.c_up   = 0,
	.c_dn   = 0,
	.c_lt   = 0,
	.c_rt   = 0,
	.c_a    = 0,
	.c_b    = 0,
	.c_x    = 0,
	.c_y    = 0,
	.cfrm = 0,
	.paus = 0,
	.back = 0
};

SDL_GameController *ctrlr = NULL;

int ww_calc_window(){
	
	ww_window_s *window_p = (ww_window_s*) window;
	
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
	
	return 0;
}

void ww_help(char * binary){
	
	printf("Usage: %s [OPTION]...\n", binary);
	printf("\n");
	printf("\t-W, --width\tWindow starting width\n");
	printf("\t\t\t[ 1 - 16000 ]\n");
	printf("\t-H, --height\tWindow starting height\n");
	printf("\t\t\t[ 1 - 9000 ]\n");
	printf("\t-S, --scale\tVector scale (for performance)\n");
	printf("\t\t\t[ 1/16 | 1/8 | 1/4 | 1/2 | 1 | 2 | 4 | 8 ]\n");
	printf("\t-B, --bits\tSet bit level for color pallette\n");
	printf("\t\t\t[ 8 | 12 | 15 | 16 | 24 ]\n");
	printf("\t-F, --fullscreen\tStart in fullscreen mode\n");
	printf("\t-N, --no-accel\tDisable hardware acceleration\n");
	printf("\t-D, --disable-vsync\tDisable hardware vsync\n");
	
}

int ww_window_create(int argc, char * argv[], char * title, int width, int height) {
		
	ww_window_s *window_p = ( ww_window_s* ) calloc( 1, sizeof( ww_window_s ) );
	window = (ww_window_t *) window_p;
	
	window_p->gamename = title;
	window_p->ww_width = width;
	window_p->ww_height = height;
	window_p->ww_default_width = width;
	window_p->ww_default_height = height;
	window_p->ww_ratio = 1.0;
	window_p->ww_scale = SC_ONE;
	window_p->pf = SDL_PIXELFORMAT_RGB888;
	window_p->acc = SDL_RENDERER_ACCELERATED;
	window_p->vsync = SDL_RENDERER_PRESENTVSYNC;
	window_p->ticks = 0;
	window_p->framediff = 0.0;
	window_p->frames = 0;
	
	for(int i = 0; i < argc; i++){
		
		if( strcmp(argv[i], "-W") == 0 || strcmp(argv[i], "--width") == 0 ){
			
			if (argc > (i + 1)){
				int t_width = atoi(argv[i + 1]);
				if( t_width > 0 && t_width <= 16000 ){
					window_p->ww_width = t_width;
					i++;
				} else {
					printf("Width setting invalid [ 0 - 16000 ]\n");
					ww_help(argv[0]);
					return -1;
				}
			} else {
				printf("No width provided [ -W 320 ] \n");
				ww_help(argv[0]);
				return -1;
			}
			
		} else if( strcmp(argv[i], "-H") == 0 || strcmp(argv[i], "--height") == 0 ){
			
			if (argc > (i + 1)){
				int t_height = atoi(argv[i + 1]);
				
				if( t_height > 0 && t_height <= 9000 ){
					window_p->ww_height = t_height;
					i++;
				} else {
					printf("Height setting invalid [ 0 - 9000 ]\n");
					ww_help(argv[0]);
					return -1;
				}
			} else {
				printf("No height provided [ -H 240 ] \n");
				ww_help(argv[0]);
				return -1;
			}
			
		} else if( strcmp(argv[i], "-F") == 0 || strcmp(argv[i], "--fullscreen") == 0 ){
			window_p->fs = 1;
			
		} else if( strcmp(argv[i], "-S") == 0 || strcmp(argv[i], "--scale") == 0 ){
			
			if (argc > (i + 1)){
				
				if( strcmp(argv[i+1], "1/16") == 0)
					window_p->ww_scale = SC_SIXTEENTH;
				else if( strcmp(argv[i+1], "1/8") == 0)
					window_p->ww_scale = SC_EIGHTH;
				else if( strcmp(argv[i+1], "1/4") == 0)
					window_p->ww_scale = SC_QUARTER;
				else if( strcmp(argv[i+1], "1/2") == 0)
					window_p->ww_scale = SC_HALF;
				else if( strcmp(argv[i+1], "1") == 0)
					window_p->ww_scale = SC_ONE;
				else if( strcmp(argv[i+1], "2") == 0)
					window_p->ww_scale = SC_TWO;
				else if( strcmp(argv[i+1], "4") == 0)
					window_p->ww_scale = SC_FOUR;
				else if( strcmp(argv[i+1], "8") == 0)
					window_p->ww_scale = SC_EIGHT;
				else {
					printf("Scale setting invalid ie. [ 1/2 ]\n");
					return -1;
					ww_help(argv[0]);
				}
				
				i++;
				
				// smooth the downscaling
				if(window_p->ww_scale >= 1.0)
					SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
				
			} else {
				printf("No scale provided ie. [ -S 1/2 ] \n");
				return -1;
				ww_help(argv[0]);
			}
			
		} else if( strcmp(argv[i], "-B") == 0 || strcmp(argv[i], "--bits") == 0 ){
		
			if (argc > (i + 1)){
				
				if( strcmp(argv[i+1], "8") == 0)
					window_p->pf = SDL_PIXELFORMAT_RGB332;
				else if( strcmp(argv[i+1], "12") == 0)
					window_p->pf = SDL_PIXELFORMAT_RGB444;
				else if( strcmp(argv[i+1], "15") == 0)
					window_p->pf = SDL_PIXELFORMAT_RGB555;
				else if( strcmp(argv[i+1], "16") == 0)
					window_p->pf = SDL_PIXELFORMAT_RGB565;
				else if( strcmp(argv[i+1], "24") == 0)
					window_p->pf = SDL_PIXELFORMAT_RGB888;
				else {
					printf("Bit setting invalid [ 15 ]\n");
					return -1;
					ww_help(argv[0]);
				}
				
				i++;
				
			} else {
				printf("No bit level provided [ -B 8 ] \n");
				return -1;
				ww_help(argv[0]);
			}
			
		} else if( strcmp(argv[i], "-N") == 0 || strcmp(argv[i], "--no-accel") == 0 ){
			window_p->acc = SDL_RENDERER_SOFTWARE;
			window_p->vsync = 0;
		} else if( strcmp(argv[i], "-D") == 0 || strcmp(argv[i], "--disable-vsync") == 0 ){
			window_p->vsync = 0;
			
		} else if (argc > 0 && i != 0) {
			ww_help(argv[0]);
			return -1;
		} else {
			continue;
		}
		
	}
	
	ww_calc_window();
	
	if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER ) < 0 ) {
		printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
		return -1;
	}

	int rc = SDL_GameControllerAddMapping("190000006c696e6b6465762064657600,RG350,platform:Linux,x:b3,a:b0,b:b1,y:b2,back:b8,start:b9,dpleft:h0.8,dpdown:h0.4,dpright:h0.2,dpup:h0.1,leftshoulder:b4,lefttrigger:b6,rightshoulder:b5,righttrigger:b7,leftstick:b10,rightstick:b11,leftx:a0,lefty:a1,rightx:a2,righty:a3,");
	if(rc == -1){
		SDL_Log( "Couldn't load RG350 controller map! SDL_Error: %s\n", SDL_GetError() );
	}
	
	uint32_t flags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;
	
	if(window_p->fs){
		flags = SDL_WINDOW_FULLSCREEN_DESKTOP;
	}
		
	window_p->ww_sdl_window = SDL_CreateWindow( title,
								SDL_WINDOWPOS_CENTERED,
								SDL_WINDOWPOS_CENTERED,
								window_p->ww_width, window_p->ww_height, 
								flags);
	
	if(!window_p->ww_sdl_window) {
		printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
		return -1;
	}
	window_p->ww_sdl_renderer = SDL_CreateRenderer( window_p->ww_sdl_window, -1,
		window_p->acc | window_p->vsync );
	
	
	if(!window_p->ww_sdl_renderer) {
		printf( "Renderer could not be created! SDL_Error: %s\n", SDL_GetError() );
		return -1;
	}
	
	window_p->ww_sdl_texture = SDL_CreateTexture( window_p->ww_sdl_renderer,
		window_p->pf,
		SDL_TEXTUREACCESS_TARGET,
		window_p->ww_width * (window_p->ww_scale), window_p->ww_height * (window_p->ww_scale));

	if(!window_p->ww_sdl_texture) {
		printf( "Screen texture could not be created! SDL_Error: %s\n", SDL_GetError() );
		return -1;
	}

	SDL_RenderClear( window_p->ww_sdl_renderer );
	
	SDL_SetRenderTarget(window_p->ww_sdl_renderer, window_p->ww_sdl_texture);

	//Check for joysticks
	if( SDL_NumJoysticks() > 0 ){
		//Load joystick
	
		if(SDL_IsGameController(0)){
			ctrlr = SDL_GameControllerOpen( 0 );
			if( ctrlr == NULL )	{
				printf( "Warning: Unable to open game controller! SDL Error: %s\n", SDL_GetError() );
			}
		}
	}

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
	
	SDL_Quit();

	return 0;
}

int ww_window_event(SDL_Event *event){
	
	ww_window_s *window_p = (ww_window_s*) window;
	int rc = 0;
	
	const Uint8* currentistates = SDL_GetKeyboardState( NULL );
	if( event->type == SDL_WINDOWEVENT )
	{
		
		switch( event->window.event )
		{
			case SDL_WINDOWEVENT_SIZE_CHANGED:
				window_p->ww_width = event->window.data1;
				window_p->ww_height = event->window.data2;
				
				ww_calc_window();
				
				if(window_p->ww_sdl_texture) SDL_DestroyTexture( window_p->ww_sdl_texture );

				window_p->ww_sdl_texture = SDL_CreateTexture( window_p->ww_sdl_renderer,
					window_p->pf,
					SDL_TEXTUREACCESS_TARGET,
					window_p->ww_width * (window_p->ww_scale), window_p->ww_height * (window_p->ww_scale));
				
				if(!window_p->ww_sdl_texture) {
					printf( "Screen texture could not be created! SDL_Error: %s\n", SDL_GetError() );
					return -1;
				}
				
				SDL_RenderClear( window_p->ww_sdl_renderer );
				window_p->ww_size_changed = 1;
				
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
	if(event->type == SDL_KEYUP ){
		if( ((event->key.keysym.sym == SDLK_RETURN) && 
		((currentistates[SDL_SCANCODE_RALT]) || 
		(currentistates[SDL_SCANCODE_LALT]))) ){
			ww_toggle_fs();
			rc = 1;
		}
	}
	
	// consume the RETURN press, so alt+ent doesn't let the .ent perform game logic
	if(event->type == SDL_KEYDOWN ){
		if( ((event->key.keysym.sym == SDLK_RETURN) && 
		((currentistates[SDL_SCANCODE_RALT]) || 
		(currentistates[SDL_SCANCODE_LALT]))) ){			
			rc = 1;
		}
	}
	
	return rc;
}

void ww_toggle_fs(){
	ww_window_s *window_p = (ww_window_s*) window;
	
	if( window_p->fs ){
		SDL_SetWindowFullscreen( window_p->ww_sdl_window, SDL_FALSE );
		window_p->fs = 0;
	}else{
		SDL_SetWindowFullscreen( window_p->ww_sdl_window, 
			SDL_WINDOW_FULLSCREEN_DESKTOP );
		window_p->fs = 1;
	}
}

void ww_key_event(SDL_Event *event){
	
	if( event->type == SDL_KEYDOWN && event->key.repeat == 0){
		switch(event->key.keysym.sym){
			case SDLK_ESCAPE:
				istate.c_sel = 1;
				istate.paus = 1;
				istate.back = 1;
				break;
			case SDLK_RETURN:
				istate.c_str = 1;
				istate.cfrm = 1;
				istate.paus = 1;
				break;
			case SDLK_UP:
				istate.c_y = 1;
				break;
			case SDLK_DOWN:
				istate.c_a = 1;
				istate.cfrm = 1;
				break;
			case SDLK_LEFT:
				istate.c_x = 1;
				break;
			case SDLK_RIGHT:
				istate.c_b = 1;
				istate.back = 1;
				break;
			case SDLK_w:
				istate.c_up = 1;
				break;
			case SDLK_a:
				istate.c_lt = 1;
				break;
			case SDLK_s:
				istate.c_dn = 1;
				break;
			case SDLK_d:
				istate.c_rt = 1;
				break;
		}
	} else if ( event->type == SDL_KEYUP ) {
		switch(event->key.keysym.sym){
			case SDLK_ESCAPE:
				istate.c_sel = 0;
				istate.paus = 0;
				istate.back = 0;
				break;
			case SDLK_RETURN:
				istate.c_str = 0;
				istate.cfrm = 0;
				istate.paus = 0;
				break;
			case SDLK_UP:
				istate.c_y = 0;
				break;
			case SDLK_DOWN:
				istate.c_a = 0;
				istate.cfrm = 0;
				break;
			case SDLK_LEFT:
				istate.c_x = 0;
				break;
			case SDLK_RIGHT:
				istate.c_b = 0;
				istate.back = 0;
				break;
			case SDLK_w:
				istate.c_up = 0;
				break;
			case SDLK_a:
				istate.c_lt = 0;
				break;
			case SDLK_s:
				istate.c_dn = 0;
				break;
			case SDLK_d:
				istate.c_rt = 0;
				break;
		}
	}

	if( event->type == SDL_CONTROLLERBUTTONDOWN){
		switch(event->cbutton.button){
			case SDL_CONTROLLER_BUTTON_BACK:
				istate.c_sel = 1;
				istate.paus = 1;
				istate.back = 1;
				break;
			case SDL_CONTROLLER_BUTTON_START:
				istate.c_str = 1;
				istate.cfrm = 1;
				istate.paus = 1;
				break;
			case SDL_CONTROLLER_BUTTON_Y:
				istate.c_y = 1;
				break;
			case SDL_CONTROLLER_BUTTON_A:
				istate.c_a = 1;
				istate.cfrm = 1;
				break;
			case SDL_CONTROLLER_BUTTON_X:
				istate.c_x = 1;
				break;
			case SDL_CONTROLLER_BUTTON_B:
				istate.c_b = 1;
				istate.back = 1;
				break;
			case SDL_CONTROLLER_BUTTON_DPAD_UP:
				istate.c_up = 1;
				break;
			case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
				istate.c_dn = 1;
				break;
			case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
				istate.c_lt = 1;
				break;
			case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
				istate.c_rt = 1;
				break;
		}
	} else if( event->type == SDL_CONTROLLERBUTTONUP){
		switch(event->cbutton.button){
			case SDL_CONTROLLER_BUTTON_BACK:
				istate.c_sel = 0;
				istate.paus = 0;
				istate.back = 0;
				break;
			case SDL_CONTROLLER_BUTTON_START:
				istate.c_str = 0;
				istate.cfrm = 0;
				istate.paus = 0;
				break;
			case SDL_CONTROLLER_BUTTON_Y:
				istate.c_y = 0;
				break;
			case SDL_CONTROLLER_BUTTON_A:
				istate.c_a = 0;
				istate.cfrm = 0;
				break;
			case SDL_CONTROLLER_BUTTON_X:
				istate.c_x = 0;
				break;
			case SDL_CONTROLLER_BUTTON_B:
				istate.c_b = 0;
				istate.back = 0;
				break;
			case SDL_CONTROLLER_BUTTON_DPAD_UP:
				istate.c_up = 0;
				break;
			case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
				istate.c_dn = 0;
				break;
			case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
				istate.c_lt = 0;
				break;
			case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
				istate.c_rt = 0;
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
	
	ww_istate_t old_istate = istate;
	ww_istate_t newi = { 0 };
	ipstate = newi;
	
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
	
		if (old_istate.c_sel == 0 && istate.c_sel == 1){ ipstate.c_sel = 1;  }
		if (old_istate.c_str == 0 && istate.c_str == 1){ ipstate.c_str = 1;  }
		if (old_istate.c_up  == 0 && istate.c_up  == 1){ ipstate.c_up  = 1;  }
		if (old_istate.c_lt  == 0 && istate.c_lt  == 1){ ipstate.c_lt  = 1;  }
		if (old_istate.c_dn  == 0 && istate.c_dn  == 1){ ipstate.c_dn  = 1;  }
		if (old_istate.c_rt  == 0 && istate.c_rt  == 1){ ipstate.c_rt  = 1;  }
		if (old_istate.c_a   == 0 && istate.c_a   == 1){ ipstate.c_a   = 1;  }
		if (old_istate.c_b   == 0 && istate.c_b   == 1){ ipstate.c_b   = 1;  }
		if (old_istate.c_x   == 0 && istate.c_x   == 1){ ipstate.c_x   = 1;  }
		if (old_istate.c_y   == 0 && istate.c_y   == 1){ ipstate.c_y   = 1;  }
		if (old_istate.cfrm  == 0 && istate.cfrm  == 1){ ipstate.cfrm  = 1;  }
		if (old_istate.paus  == 0 && istate.paus  == 1){ ipstate.paus  = 1;  }
		if (old_istate.back  == 0 && istate.back  == 1){ ipstate.back  = 1;  }
	}
	
	return 0;
}

int ww_frames_passed(){
	ww_window_s *window_p = (ww_window_s*) window;
	
	return window_p->frames;
}

void ww_window_send_quit_event() {
	if(!window) {
		fprintf( stderr, "Cannot update non-existent window\n" );
		return;
	}

	ww_window_s *window_p = (ww_window_s*) window;
	window_p->ww_received_quit_event = 1;
}

int ww_window_received_quit_event() {
	if(!window) {
		fprintf( stderr, "Cannot update non-existent window\n" );
		return -1;
	}

	ww_window_s *window_p = (ww_window_s*) window;
	return window_p->ww_received_quit_event;
}

int _gfxPrimitivesCompareInt(const void *a, const void *b)
{
	return (*(const int *) a) - (*(const int *) b);
}

void ww_render_bars(){
	
	ww_window_s *window_p = (ww_window_s*) window;
	
	unsigned char bar_grey[] = { 0x1C, 0x1F, 0x20 };
	
	if (window_p->ww_pad_x != 0){
		// left
		short bar_ax[4] = { 
			0, 
			window_p->ww_pad_x * (window_p->ww_scale), 
			window_p->ww_pad_x * (window_p->ww_scale), 
			0 
		};
		short bar_ay[4] = { 
			0, 
			0, 
			window_p->ww_height * (window_p->ww_scale), 
			window_p->ww_height * (window_p->ww_scale)
		};
		ww_draw_raw_polygon(bar_ax, bar_ay, 4, 0, window_p->ww_height * (window_p->ww_scale), bar_grey);
		// right
		short bar_bx[4] = { 
			(window_p->ww_width - window_p->ww_pad_x) * (window_p->ww_scale), 
			(window_p->ww_width) * (window_p->ww_scale),
			(window_p->ww_width) * (window_p->ww_scale), 
			(window_p->ww_width - window_p->ww_pad_x) * (window_p->ww_scale)
		};
		short bar_by[4] = { 
			0, 
			0, 
			window_p->ww_height * (window_p->ww_scale), 
			window_p->ww_height * (window_p->ww_scale)
		};
		ww_draw_raw_polygon(bar_bx, bar_by, 4, 0, window_p->ww_height * (window_p->ww_scale), bar_grey);
		
	} else {
		// top
		short bar_ax[4] = { 
			0, 
			window_p->ww_width * (window_p->ww_scale),
			window_p->ww_width * (window_p->ww_scale),
			0
		};
		short bar_ay[4] = { 
			0, 
			0, 
			window_p->ww_pad_y * (window_p->ww_scale), 
			window_p->ww_pad_y * (window_p->ww_scale)
		};
		ww_draw_raw_polygon(bar_ax, bar_ay, 4, 0, window_p->ww_pad_y * (window_p->ww_scale), bar_grey);
		// bottom
		short bar_bx[4] = { 
			0, 
			window_p->ww_width * (window_p->ww_scale), 
			window_p->ww_width * (window_p->ww_scale),
			0
		};
		short bar_by[4] = {
			(window_p->ww_height - window_p->ww_pad_y) * (window_p->ww_scale), 
			(window_p->ww_height - window_p->ww_pad_y) * (window_p->ww_scale), 
			window_p->ww_height * (window_p->ww_scale),
			window_p->ww_height * (window_p->ww_scale)
		};
		
		short max_y = bar_by[0];
		for (int i = 1; i < 4; i++)
			if (bar_by[i] > max_y)
				max_y = bar_by[i];
		
		ww_draw_raw_polygon(bar_bx, bar_by, 4, 0, max_y, bar_grey);
		
	}
	
}

void ww_clear_buffer(){
	
	ww_window_s *window_p = (ww_window_s*) window;
	
	SDL_SetRenderDrawColor(window_p->ww_sdl_renderer, 0, 0, 0, 255);
	
	// clear texture
	SDL_RenderClear( window_p->ww_sdl_renderer );
	
	// clear renderer
	SDL_SetRenderTarget(window_p->ww_sdl_renderer, NULL);
	SDL_RenderClear(window_p->ww_sdl_renderer);
	
	// draw to texture
	SDL_SetRenderTarget(window_p->ww_sdl_renderer, window_p->ww_sdl_texture);

}

//~ #include <sys/time.h>
//~ struct timeval t0 = {.tv_usec = 0};
//~ struct timeval t1 = {.tv_usec = 0};

int ww_window_update_buffer() {
	
	//~ gettimeofday(&t1, 0);
	
	//~ if ( t0.tv_usec != 0 ){
		//~ printf("frametime: %f\n", (t1.tv_sec - t0.tv_sec) * 1000.0f + (t1.tv_usec - t0.tv_usec) / 1000.0f);
	//~ }
	
	//~ t0 = t1;
	
	if(!window) {
		return -1;
	}
	ww_window_s *window_p = (ww_window_s*) window;
	
	ww_render_bars();
	
	if (window_p->ww_size_changed) {
		
		// don't draw, let the buffer get wiped
		
		window_p->ww_size_changed = 0;
		
	} else {
		
		// draw
		
		SDL_SetRenderTarget(window_p->ww_sdl_renderer, NULL);
		
		SDL_RenderCopy( window_p->ww_sdl_renderer, window_p->ww_sdl_texture, NULL, NULL );
		SDL_RenderPresent( window_p->ww_sdl_renderer );
		
		SDL_SetRenderTarget(window_p->ww_sdl_renderer, window_p->ww_sdl_texture);
	}
	
	unsigned int new_ticks = SDL_GetTicks();
	
	// ms since last update
	unsigned int passed_ms = (new_ticks - window_p->ticks);
	
	// frames since last update
	window_p->frames = (float)passed_ms / 16.67;
	
	// store unused ticks
	window_p->framediff += (float)passed_ms - ( window_p->frames * 16.67 );
	
	// how many 16.67's have passed
	while (window_p->framediff > 16.67){
		window_p->framediff -= 16.67;
		window_p->frames++;
	}
		
	window_p->ticks = new_ticks;
	
	ww_clear_buffer();
	
	return 0;
}

ww_sprite_t * ww_new_sprite(ww_reference_t payload){
	
	size_t sz = (
		sizeof(ww_sprite_t)
		+ sizeof(ww_animation_t) * payload.alloc[0]
		+ sizeof(ww_frame_t)     * payload.alloc[1]
		+ sizeof(int)            * payload.alloc[1] // delay per frame
		+ sizeof(ww_polygon_t)   * payload.alloc[2]
		+ sizeof(short)          * payload.alloc[3] * 4
	);
	
	//~ printf("sizes:\n\tspri: %lu\n\tanim: %lu\n\tfram: %lu\n\tpoly: %lu\n\tint: %lu\n\n",
		//~ sizeof(ww_sprite_t),
		//~ sizeof(ww_animation_t),
		//~ sizeof(ww_frame_t),
		//~ sizeof(ww_polygon_t),
		//~ sizeof(short)
	//~ );
	
	ww_sprite_t * s = calloc(1, sz);
	
	s->count = payload.alloc[0];
	s->scale = 1.0;
	
	uint8_t * c = (uint8_t *)s + sizeof(ww_sprite_t);
	
	//~ printf("s: %p\n", (void *)s);
	//~ printf("s: %p sz: %x s+sz: %p &s[1]: %p c: %p\n", (void *)s, (unsigned int)sz, (uint8_t *)s + sz, (void *)&s[1], (void *)c);
	
	//~ printf("curr: %p targ: %p\n", (void *)c, (void *)&s->animations);
	
	s->animations = (void *)c;
	c += sizeof(ww_animation_t) * payload.alloc[0];
	
	int fstride = 0;
	int pstride = 0;
	int vstride = 0;
	int astride = 0;
	
	for(int a = 0; a < payload.alloc[0]; a++){
		// just need the anim index
		
		//~ printf("pre-delay -- curr: %p targ: %p\n", (void *)c, (void *)&s->animations[a].frames);
		s->animations[a].count = payload.frames[fstride];
		
		// copy in delay -- ensure at least 1
		s->animations[a].delay = (void *)c;
		for(int i = 0; i < payload.frames[fstride]; i++){
			s->animations[a].delay[i] = ( payload.delays[a][i] > 0 ? payload.delays[a][i] : 1 );
		}
		
		c += sizeof(int) * payload.frames[fstride];
		
		//~ printf("pre-frame -- curr: %p targ: %p\n", (void *)c, (void *)&s->animations[a].frames);
		s->animations[a].frames = (void *)c;
		c += sizeof(ww_frame_t) * payload.frames[fstride];
		
		for(int f = 0; f < payload.frames[fstride]; f++){
			
			s->animations[a].frames[f].count = payload.polygons[pstride];
			
			//~ printf("pre-polys -- curr: %p targ: %p\n", (void *)c, (void *)&s->animations[a].frames[f].polys);
			s->animations[a].frames[f].polys = (void *)c;
			c += sizeof(ww_polygon_t) * payload.polygons[pstride];
			
			for(int p = 0; p < payload.polygons[pstride]; p++){
				
				s->animations[a].frames[f].polys[p].count = payload.vertices[vstride];
				s->animations[a].frames[f].polys[p].ratio = 1.0;
				s->animations[a].frames[f].polys[p].s_parent = s;
				//~ s->animations[a].frames[f].polys[p].s_scale = 1.0; // ensures rescale on first draw
				s->animations[a].frames[f].polys[p].scale = 1.0;
				memcpy(s->animations[a].frames[f].polys[p].color, payload.colors[vstride], sizeof(ww_rgba_t));
				
				//~ printf("pre-x     -- curr: %p anim: %d  fram: %d poly: %d xory: x\n", c, a, f, p);
				s->animations[a].frames[f].polys[p].x = (void *)c;
				memcpy(c, payload.arrays[astride], sizeof(short) * payload.vertices[vstride]);
				c += sizeof(short) * payload.vertices[vstride];
				
				//~ printf("pre-scalX -- curr: %p anim: %d  fram: %d poly: %d xory: x\n", c, a, f, p);
				s->animations[a].frames[f].polys[p].scaled_x = (void *)c;
				memcpy(c, payload.arrays[astride], sizeof(short) * payload.vertices[vstride]);
				c += sizeof(short) * payload.vertices[vstride];
				
				astride++;
				//~ printf("pre-y     -- curr: %p anim: %d  fram: %d poly: %d xory: y\n", c, a, f, p);
				s->animations[a].frames[f].polys[p].y = (void *)c;
				memcpy(c, payload.arrays[astride], sizeof(short) * payload.vertices[vstride]);
				c += sizeof(short) * payload.vertices[vstride];
				
				//~ printf("pre-scalY -- curr: %p anim: %d  fram: %d poly: %d xory: y\n", c, a, f, p);
				s->animations[a].frames[f].polys[p].scaled_y = (void *)c;
				memcpy(c, payload.arrays[astride], sizeof(short) * payload.vertices[vstride]);
				c += sizeof(short) * payload.vertices[vstride];
				
				ww_scale_polygon(&s->animations[a].frames[f].polys[p]);
				
				vstride++;
				astride++;
				//~ printf("vs: %d as: %d\n", vstride, astride);
			}
			pstride++;
		}
		fstride++;
	}
	
	//~ for(int i = 0; i < s->count; i++){
		//~ for(int j = 0; j < s->animations[i].count; j++){
			//~ for(int k = 0; k < s->animations[i].frames[j].count; k++){
				//~ for(int l = 0; l < s->animations[i].frames[j].polys[k].count; l++){
					//~ if (s->animations[i].frames[j].polys[k].y[l] > s->maxy){
						//~ s->maxy = s->animations[i].frames[j].polys[k].y[l];
					//~ }
				//~ }
			//~ }
		//~ }
	//~ }
	
	//~ printf("curr: %p targ: %p\n", c, NULL);
	
	return s;
	
}

ww_sprite_t * ww_clone_sprite(ww_sprite_t * insprite){
	
	int ac = insprite->count;
	int fc = 0;
	int pc = 0;
	int vc = 0;
	//~ printf("0\n");
	
	for(int i = 0; i < ac; i++){
		fc += insprite->animations[i].count;
		
		for(int j = 0; j < insprite->animations[i].count; j++){
			pc += insprite->animations[i].frames[j].count;
			
			for(int k = 0; k < insprite->animations[i].frames[j].count; k++){
				vc += insprite->animations[i].frames[j].polys[k].count;
			}
		}
	}
	//~ printf("1\n");
	
	int alloc[] = { ac, fc, pc, vc };
	
	int frames[fc];
	int * delays[fc];
	int polygons[pc];
	unsigned char * colors[pc]; // = malloc(sizeof(char *) * pc);
	int vertices[vc];
	short * arrays[vc * 2];
	//~ printf("2\n");
	
	int p_stride = 0;
	int c_stride = 0;
	int a_stride = 0;
	
	
	for(int i = 0; i < ac; i++){
		frames[i] = insprite->animations[i].count;
		delays[i] = insprite->animations[i].delay;
		
		for(int j = 0; j < insprite->animations[i].count; j++){
			polygons[p_stride] = insprite->animations[i].frames[j].count;
			p_stride++;
			
			for(int k = 0; k < insprite->animations[i].frames[j].count; k++){
				colors[c_stride] = insprite->animations[i].frames[j].polys[k].color;
				vertices[c_stride] = insprite->animations[i].frames[j].polys[k].count;
				c_stride++;
				
				arrays[a_stride] = insprite->animations[i].frames[j].polys[k].x;
				a_stride++;
				arrays[a_stride] = insprite->animations[i].frames[j].polys[k].y;
				a_stride++;
			}
		}
	}
	
				//~ printf("colors[%d][0]: %d\n", 0, colors[0][0]);
				//~ printf("colors[%d][1]: %d\n", 0, colors[0][1]);
				//~ printf("colors[%d][2]: %d\n", 0, colors[0][2]);
				//~ printf("colors[%d][0]: %d\n", 1, colors[1][0]);
				//~ printf("colors[%d][1]: %d\n", 1, colors[1][1]);
				//~ printf("colors[%d][2]: %d\n", 1, colors[1][2]);
	
	//~ printf("3\n");
	
	ww_reference_t clone_payload = {
		.alloc = alloc,
		.frames = frames,
		.delays = delays,
		.polygons = polygons,
		.colors = colors,
		.vertices = vertices,
		.arrays = arrays,
	};
	
	//~ printf("aanim[0] -- BA: %d\tba:%d\n", BA.alloc[0], alloc[0]);
	//~ printf("afram[1] -- BA: %d\tba:%d\n", BA.alloc[1], alloc[1]);
	//~ printf("apoly[2] -- BA: %d\tba:%d\n", BA.alloc[2], alloc[2]);
	//~ printf("avert[3] -- BA: %d\tba:%d\n", BA.alloc[3], alloc[3]);
	
	//~ for(int i = 0; i < alloc[0]; i++)
		//~ printf("frames[%d] -- BA: %d\tba: %d\n", i, BA.frames[i], frames[i]);
	//~ for(int i = 0; i < alloc[0]; i++)
		//~ printf("delays[%d] -- BA: %d\tba: %d\n", i, BA.delays[i][0], delays[i][0]);
	//~ for(int i = 0; i < alloc[1]; i++)
		//~ printf("polygs[%d] -- BA: %d\tba: %d\n", i, BA.polygons[i], polygons[0]);
	//~ for(int i = 0; i < alloc[2]; i++){
		//~ printf("colors[%d][0] -- BA: %d\tba: %d\n", i, BA.colors[i][0], colors[i][0]);
		//~ printf("colors[%d][1] -- BA: %d\tba: %d\n", i, BA.colors[i][1], colors[i][1]);
		//~ printf("colors[%d][2] -- BA: %d\tba: %d\n", i, BA.colors[i][2], colors[i][2]);
	//~ }
	
	ww_sprite_t * s = ww_new_sprite(clone_payload);
	
	s->pad_x = insprite->pad_x;
	s->pad_y = insprite->pad_y;
	s->paused = insprite->paused;
	s->scale = insprite->scale;
	
	return s;
	
}

//~ int *gfxPrimitivesPolyInts = NULL;

int ww_draw_raw_polygon(const Sint16 * vx, const Sint16 * vy, int n, short miny, short maxy, unsigned char * color)
{
	ww_window_s *window_p = (ww_window_s*) window;
	
	int result;
	int i;
	int y, xa, xb;
	int x1, y1;
	int x2, y2;
	int ind1, ind2;
	int ints;
	//~ gfxPrimitivesPolyInts = (int *) realloc(gfxPrimitivesPolyInts, sizeof(int) * n);
	int *gfxPrimitivesPolyInts = (int *) malloc(sizeof(int) * n);
	
	//~ printf("color: %d %d %d\n", color[0], color[1], color[2]);
	
	if (vx == NULL || vy == NULL || n < 3) {
		return -1;
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
		}
	}
	
	free(gfxPrimitivesPolyInts);
	
	return (result);
}



void ww_scale_polygon(ww_polygon_t * poly){
	//~ printf("scale_p\n");
	ww_window_s *window_p = (ww_window_s*) window;
	
	ww_sprite_t * parent = (ww_sprite_t *)(poly->s_parent);
	
	if( poly->ratio   != window_p->ww_ratio || 
		poly->w_pad_x != window_p->ww_pad_x ||
		poly->w_pad_y != window_p->ww_pad_y ||
		poly->s_pad_x != parent->pad_x ||
		poly->s_pad_y != parent->pad_y ||
		poly->s_scale != parent->scale ||
		poly->scale   != window_p->ww_scale ){
		
		poly->ratio = window_p->ww_ratio;
		poly->w_pad_x = window_p->ww_pad_x;
		poly->w_pad_y = window_p->ww_pad_y;
		poly->s_pad_x = parent->pad_x;
		poly->s_pad_y = parent->pad_y;
		poly->s_scale = parent->scale;
		poly->scale = window_p->ww_scale;
		
		poly->miny = 0;
		poly->maxy = 0;
		
		int minx = 0;
		int maxx = 0;
		
		for(int i = 0; i < poly->count; i++){
			poly->scaled_x[i] = poly->scale * ((poly->x[i] * poly->s_scale + poly->s_pad_x) * poly->ratio + poly->w_pad_x);
			poly->scaled_y[i] = poly->scale * ((poly->y[i] * poly->s_scale + poly->s_pad_y) * poly->ratio + poly->w_pad_y);
			
			if (poly->scaled_x[i] < minx) {
				minx = poly->scaled_x[i];
			} else if (poly->scaled_x[i] > maxx) {
				maxx = poly->scaled_x[i];
			}
			
			if (poly->scaled_y[i] < poly->miny) {
				poly->miny = poly->scaled_y[i];
			} else if (poly->scaled_y[i] > poly->maxy) {
				poly->maxy = poly->scaled_y[i];
			}
		}
		
		if( (maxx - minx) > parent->width)
			parent->width = (maxx - minx);
		if( (poly->maxy - poly->miny) > parent->height)
			parent->height = (poly->maxy - poly->miny);
	
	}
	
}

int ww_draw_polygon(ww_polygon_t * poly){
	//~ printf("draw_p\n");
	
	//~ printf("dp_pre_poly: %d\n", poly->scaled_x[0]);
	ww_scale_polygon(poly);
	//~ printf("dp_pos_poly: %d\n", poly->scaled_x[0]);
	
	return ww_draw_raw_polygon(poly->scaled_x, poly->scaled_y, poly->count, poly->miny, poly->maxy, poly->color);
}

int ww_draw_frame(ww_frame_t * frame){
	//~ printf("draw_f\n");
	
	//~ printf("frame: %d\n", frame->polys[0].x[0]);
	int rc = 0;
	
	// TODO implement z-depth sorted draws
	for(int i = 0; i < frame->count; i++){
		rc += ww_draw_polygon(&frame->polys[i]);
	}
	
	return rc;
	
}

int ww_draw_animation(ww_animation_t * anim, int paused){
	//~ printf("draw_a\n");
	
	//~ printf("anim: %d\n", anim->frames[0].polys[0].x[0]);
	if (paused == 0) {
		anim->d_progress -= ww_frames_passed();
		//~ anim->d_progress--;
		
		while(anim->d_progress <= 0){
			
			int old_delay = anim->delay[anim->active_frame];
			anim->active_frame++;
			
			if(anim->active_frame == anim->count)
				anim->active_frame = 0;
			
			anim->d_progress = anim->delay[anim->active_frame] - (anim->d_progress % old_delay);
		}
		
	}
		//~ int rc = 0;
	int rc = ww_draw_frame(&anim->frames[anim->active_frame]);
	
	return rc;
	
}

int ww_draw_sprite(ww_sprite_t * sprite){
	
	//~ printf("sprite: %d\n", sprite->animations[0].frames[0].polys[0].x[0]);
	int rc = ww_draw_animation(&sprite->animations[sprite->active_animation], sprite->paused);
	
	return rc;
	
}
