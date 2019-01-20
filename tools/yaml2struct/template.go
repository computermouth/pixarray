package main

var (
	CTemplate = `

{{$GSpritename := .Spritename}}

// #ifdef {{.Spritename}}_SPRITE_H
// #error "{{.Spritename}} already exists"
// #else
#ifndef {{.Spritename}}_SPRITE_H
#define {{.Spritename}}_SPRITE_H

// declare sprite
extern ww_sprite_t * {{.Spritename}};
ww_sprite_t * mario = NULL;

{{range .Animations}}
	
	// declare animation {{$GAnimation := .Animation}}
	extern ww_animation_t * {{$GSpritename}}_{{.Animation}};
	ww_animation_t * {{$GSpritename}}_{{.Animation}} = NULL;
	
	{{range .Frames}}
		
		//declare frame {{$GFrame := .Frame}}
		extern ww_frame_t * {{$GSpritename}}_{{$GAnimation}}_{{.Frame}};
		ww_frame_t * {{$GSpritename}}_{{$GAnimation}}_{{.Frame}} = NULL;
		
		void init_{{$GSpritename}}_{{$GAnimation}}_{{$GFrame}}(){
		{{range .Layers}}
		
			{{if .Color}}
			//declare layer
			ww_rgba_t {{$GSpritename}}_{{$GAnimation}}_{{$GFrame}}_{{.Layer}}_color = { {{index .TrueColor 0}}, {{index .TrueColor 1}}, {{index .TrueColor 2}} };
			short {{$GSpritename}}_{{$GAnimation}}_{{$GFrame}}_{{.Layer}}_x[{{len .X}}] = { {{$lenX := dec (len .X)}}{{range $i, $e := .X}}{{$e}}{{if ne $i $lenX}}, {{end}}{{end}} };
			short {{$GSpritename}}_{{$GAnimation}}_{{$GFrame}}_{{.Layer}}_y[{{len .Y}}] = { {{$lenY := dec (len .Y)}}{{range $i, $e := .Y}}{{$e}}{{if ne $i $lenY}}, {{end}}{{end}} };
			{{$GSpritename}}_{{$GAnimation}}_{{$GFrame}}_{{.Layer}} = ww_new_polygon({{$GSpritename}}_{{$GAnimation}}_{{$GFrame}}_{{.Layer}}_color, {{$GSpritename}}_{{$GAnimation}}_{{$GFrame}}_{{.Layer}}_x, {{$GSpritename}}_{{$GAnimation}}_{{$GFrame}}_{{.Layer}}_y, {{len .X}})
			{{end}}
			
		{{end}}
		}
		
	{{end}}
  
{{end}}

// init frame
void init_mario_jump_left_0(){
	// put after {{range .Layers}} corresponding {{end}}
	mario_jump_left_0_frame = ww_new_frame( mario_jump_left_0_fist1, NULL);
}

// init anim
void init_mario_jump_left_anim(){
	
	init_mario_jump_left_0();
	
	int delay[] = { 0 };
	mario_jump_left = ww_new_animation( delay, mario_jump_left_0_frame, NULL);
	
}

void init_mario_sprite(){
	
	init_mario_jump_left_anim();

	mario = ww_new_sprite( 0,
		mario_jump_left,
		NULL
	);
	
}

#endif
`
)
