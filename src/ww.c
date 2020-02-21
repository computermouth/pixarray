

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <wchar.h>

#include <SDL2/SDL.h>

#include "ww.h"

ww_window_t window = NULL;

ww_istate_t istate = {
	.str  = 0,
	.sel  = 0,
	.up   = 0,
	.dn   = 0,
	.lt   = 0,
	.rt   = 0,
	.ba    = 0,
	.bb    = 0,
	.bx    = 0,
	.by    = 0,
	.cfrm = 0,
	.paus = 0,
	.back = 0
};

ww_istate_t ipstate = {
	.str  = 0,
	.sel  = 0,
	.up   = 0,
	.dn   = 0,
	.lt   = 0,
	.rt   = 0,
	.ba    = 0,
	.bb    = 0,
	.bx    = 0,
	.by    = 0,
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
				istate.sel = 1;
				istate.paus = 1;
				istate.back = 1;
				break;
			case SDLK_RETURN:
				istate.str = 1;
				istate.cfrm = 1;
				istate.paus = 1;
				break;
			case SDLK_UP:
				istate.by = 1;
				break;
			case SDLK_DOWN:
				istate.ba = 1;
				istate.cfrm = 1;
				break;
			case SDLK_LEFT:
				istate.bx = 1;
				break;
			case SDLK_RIGHT:
				istate.bb = 1;
				istate.back = 1;
				break;
			case SDLK_w:
				istate.up = 1;
				break;
			case SDLK_a:
				istate.lt = 1;
				break;
			case SDLK_s:
				istate.dn = 1;
				break;
			case SDLK_d:
				istate.rt = 1;
				break;
		}
	} else if ( event->type == SDL_KEYUP ) {
		switch(event->key.keysym.sym){
			case SDLK_ESCAPE:
				istate.sel = 0;
				istate.paus = 0;
				istate.back = 0;
				break;
			case SDLK_RETURN:
				istate.str = 0;
				istate.cfrm = 0;
				istate.paus = 0;
				break;
			case SDLK_UP:
				istate.by = 0;
				break;
			case SDLK_DOWN:
				istate.ba = 0;
				istate.cfrm = 0;
				break;
			case SDLK_LEFT:
				istate.bx = 0;
				break;
			case SDLK_RIGHT:
				istate.bb = 0;
				istate.back = 0;
				break;
			case SDLK_w:
				istate.up = 0;
				break;
			case SDLK_a:
				istate.lt = 0;
				break;
			case SDLK_s:
				istate.dn = 0;
				break;
			case SDLK_d:
				istate.rt = 0;
				break;
		}
	}

	if( event->type == SDL_CONTROLLERBUTTONDOWN){
		switch(event->cbutton.button){
			case SDL_CONTROLLER_BUTTON_BACK:
				istate.sel = 1;
				istate.paus = 1;
				istate.back = 1;
				break;
			case SDL_CONTROLLER_BUTTON_START:
				istate.str = 1;
				istate.cfrm = 1;
				istate.paus = 1;
				break;
			case SDL_CONTROLLER_BUTTON_Y:
				istate.by = 1;
				break;
			case SDL_CONTROLLER_BUTTON_A:
				istate.ba = 1;
				istate.cfrm = 1;
				break;
			case SDL_CONTROLLER_BUTTON_X:
				istate.bx = 1;
				break;
			case SDL_CONTROLLER_BUTTON_B:
				istate.bb = 1;
				istate.back = 1;
				break;
			case SDL_CONTROLLER_BUTTON_DPAD_UP:
				istate.up = 1;
				break;
			case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
				istate.dn = 1;
				break;
			case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
				istate.lt = 1;
				break;
			case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
				istate.rt = 1;
				break;
		}
	} else if( event->type == SDL_CONTROLLERBUTTONUP){
		switch(event->cbutton.button){
			case SDL_CONTROLLER_BUTTON_BACK:
				istate.sel = 0;
				istate.paus = 0;
				istate.back = 0;
				break;
			case SDL_CONTROLLER_BUTTON_START:
				istate.str = 0;
				istate.cfrm = 0;
				istate.paus = 0;
				break;
			case SDL_CONTROLLER_BUTTON_Y:
				istate.by = 0;
				break;
			case SDL_CONTROLLER_BUTTON_A:
				istate.ba = 0;
				istate.cfrm = 0;
				break;
			case SDL_CONTROLLER_BUTTON_X:
				istate.bx = 0;
				break;
			case SDL_CONTROLLER_BUTTON_B:
				istate.bb = 0;
				istate.back = 0;
				break;
			case SDL_CONTROLLER_BUTTON_DPAD_UP:
				istate.up = 0;
				break;
			case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
				istate.dn = 0;
				break;
			case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
				istate.lt = 0;
				break;
			case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
				istate.rt = 0;
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
	
		if (old_istate.sel  == 0 && istate.sel  == 1){ ipstate.sel  = 1;  }
		if (old_istate.str  == 0 && istate.str  == 1){ ipstate.str  = 1;  }
		if (old_istate.up   == 0 && istate.up   == 1){ ipstate.up   = 1;  }
		if (old_istate.lt   == 0 && istate.lt   == 1){ ipstate.lt   = 1;  }
		if (old_istate.dn   == 0 && istate.dn   == 1){ ipstate.dn   = 1;  }
		if (old_istate.rt   == 0 && istate.rt   == 1){ ipstate.rt   = 1;  }
		if (old_istate.ba   == 0 && istate.ba   == 1){ ipstate.ba   = 1;  }
		if (old_istate.bb   == 0 && istate.bb   == 1){ ipstate.bb   = 1;  }
		if (old_istate.bx   == 0 && istate.bx   == 1){ ipstate.bx   = 1;  }
		if (old_istate.by   == 0 && istate.by   == 1){ ipstate.by   = 1;  }
		if (old_istate.cfrm == 0 && istate.cfrm == 1){ ipstate.cfrm = 1;  }
		if (old_istate.paus == 0 && istate.paus == 1){ ipstate.paus = 1;  }
		if (old_istate.back == 0 && istate.back == 1){ ipstate.back = 1;  }
	}
	
	unsigned int new_ticks = SDL_GetTicks();
	
	// ms since last update
	unsigned int passed_ms = (new_ticks - window_p->ticks);
	
	// frames since last update
	window_p->frames = passed_ms / 16;
	
	// leftovers from last frame, plus new .67s, plus 
	window_p->framediff += ((float)window_p->frames * .67) + (passed_ms % 16);
	
	// how many 16.67's have passed
	while (window_p->framediff > 16.67){
		window_p->framediff -= 16.67;
		window_p->frames++;
	}
	
	//~ printf("nt: %u\n\tleftover: %d\n\t\tpassed: %u\n\t\t\tframes: %d\n\t\t\t\tframediff: %f\n", 
		//~ new_ticks, 0, passed_ms, window_p->frames, window_p->framediff);
		
	window_p->ticks = new_ticks;
	
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
		poly->s_pad_x != parent->pad_x ||
		poly->s_pad_y != parent->pad_y ||
		poly->s_scale != parent->scale ||
		poly->scale != window_p->ww_scale ){
		
		poly->ratio = window_p->ww_ratio;
		poly->w_pad_x = window_p->ww_pad_x;
		poly->w_pad_y = window_p->ww_pad_y;
		poly->s_pad_x = parent->pad_x;
		poly->s_pad_y = parent->pad_y;
		poly->s_scale = parent->scale;
		poly->scale = window_p->ww_scale;
		
		for(int i = 0; i < poly->count; i++){
			poly->scaled_x[i] = poly->scale * ((poly->x[i] * poly->s_scale + poly->s_pad_x) * poly->ratio + poly->w_pad_x);
			poly->scaled_y[i] = poly->scale * ((poly->y[i] * poly->s_scale + poly->s_pad_y) * poly->ratio + poly->w_pad_y);
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

int ww_draw_animation(ww_animation_t * anim, int paused){
	
	if (paused == 0) {
		anim->d_progress -= ww_frames_passed();
		
		while(anim->d_progress <= 0){
			
			int old_delay = anim->delay[anim->active_frame];
			anim->active_frame++;
			
			if(anim->active_frame == anim->count)
				anim->active_frame = 0;
			
			anim->d_progress = anim->delay[anim->active_frame] - (anim->d_progress % old_delay);
		}
		
	}
	
	int rc = ww_draw_frame(anim->frames[anim->active_frame]);
	
	return rc;
	
}

int ww_draw_sprite(ww_sprite_t * sprite){
	
	int rc = ww_draw_animation(sprite->animations[sprite->active_animation], sprite->paused);
	
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
	sprite->scale = 1.0;
	
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

ww_sprite_t * ww_clone_sprite(ww_sprite_t * in_sprite){
	
	ww_sprite_t * out_sprite = NULL;
	
	if(in_sprite == NULL){
		printf("ww_clone_sprite called with only NULL argument");
		return NULL;
	}
	
	out_sprite = calloc(1, sizeof(ww_sprite_t));
	
	// assignments
	*out_sprite = *in_sprite;
	// alloc
	out_sprite->animations = NULL;
	out_sprite->animations = calloc(in_sprite->count, sizeof(ww_animation_t *));
	
	for(int i = 0; i < in_sprite->count; i++){
		
		out_sprite->animations[i] = calloc(1, sizeof(ww_animation_t));
		*(out_sprite->animations[i]) = *(in_sprite->animations[i]);
		
		out_sprite->animations[i]->delay  = NULL;
		out_sprite->animations[i]->delay  = calloc(in_sprite->animations[i]->count, sizeof(ww_frame_t *));
		for(int j = 0; j < out_sprite->animations[i]->count; j++)
			out_sprite->animations[i]->delay[j] = in_sprite->animations[i]->delay[j];
		
		out_sprite->animations[i]->frames = NULL;
		out_sprite->animations[i]->frames = calloc(in_sprite->animations[i]->count, sizeof(ww_frame_t *));
		
		for(int j = 0; j < in_sprite->animations[i]->count; j++){
			
			out_sprite->animations[i]->frames[j] = calloc(1, sizeof(ww_frame_t));
			*out_sprite->animations[i]->frames[j] = *in_sprite->animations[i]->frames[j];
			out_sprite->animations[i]->frames[j]->polys = NULL;
			out_sprite->animations[i]->frames[j]->polys = calloc(in_sprite->animations[i]->frames[j]->count, sizeof(ww_polygon_t *));
						
			for(int k = 0; k < in_sprite->animations[i]->frames[j]->count; k++){
				
				out_sprite->animations[i]->frames[j]->polys[k] = calloc(1, sizeof(ww_polygon_t));
				*out_sprite->animations[i]->frames[j]->polys[k] = *in_sprite->animations[i]->frames[j]->polys[k];
				out_sprite->animations[i]->frames[j]->polys[k]->x        = NULL;
				out_sprite->animations[i]->frames[j]->polys[k]->y        = NULL;
				out_sprite->animations[i]->frames[j]->polys[k]->scaled_x = NULL;
				out_sprite->animations[i]->frames[j]->polys[k]->scaled_y = NULL;
				out_sprite->animations[i]->frames[j]->polys[k]->s_parent = out_sprite;
				out_sprite->animations[i]->frames[j]->polys[k]->x        = calloc(in_sprite->animations[i]->frames[j]->polys[k]->count, sizeof(short));
				out_sprite->animations[i]->frames[j]->polys[k]->y        = calloc(in_sprite->animations[i]->frames[j]->polys[k]->count, sizeof(short));
				out_sprite->animations[i]->frames[j]->polys[k]->scaled_x = calloc(in_sprite->animations[i]->frames[j]->polys[k]->count, sizeof(short));
				out_sprite->animations[i]->frames[j]->polys[k]->scaled_y = calloc(in_sprite->animations[i]->frames[j]->polys[k]->count, sizeof(short));
				
				for(int l = 0; l < in_sprite->animations[i]->frames[j]->polys[k]->count; l++){
					
					out_sprite->animations[i]->frames[j]->polys[k]->x[l]        = in_sprite->animations[i]->frames[j]->polys[k]->x[l];     
					out_sprite->animations[i]->frames[j]->polys[k]->y[l]        = in_sprite->animations[i]->frames[j]->polys[k]->y[l];      
					out_sprite->animations[i]->frames[j]->polys[k]->scaled_x[l] = in_sprite->animations[i]->frames[j]->polys[k]->scaled_x[l];
					out_sprite->animations[i]->frames[j]->polys[k]->scaled_y[l] = in_sprite->animations[i]->frames[j]->polys[k]->scaled_y[l];
					
				}
				
			}
			
		}
		
	}
	
	return out_sprite;
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
		ww_draw_raw_polygon(bar_ax, bar_ay, 4, bar_grey);
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
		ww_draw_raw_polygon(bar_bx, bar_by, 4, bar_grey);
		
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
		ww_draw_raw_polygon(bar_ax, bar_ay, 4, bar_grey);
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
		ww_draw_raw_polygon(bar_bx, bar_by, 4, bar_grey);
		
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

int ww_window_update_buffer() {
	
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
	
	ww_clear_buffer();
	
	return 0;
}
