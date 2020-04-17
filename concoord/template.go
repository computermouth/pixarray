package main

var (
	HTemplate = `
{{$GSpritename := .Spritename}}
#ifndef {{toupper .Spritename}}_SPRITE_H
#define {{toupper .Spritename}}_SPRITE_H

#include "ww.h"

extern ww_reference_t {{toupper .Spritename}};

#endif

`
	CTemplate = `

{{$GSpritename := toupper .Spritename}}

#include "{{.Spritename}}.h"

#define {{$GSpritename}}_ANIMATION_COUNT {{len .Animations}}
{{range .Animations}} {{$GAnimation := toupper .Animation}}
#define   {{$GSpritename}}_{{$GAnimation}}_FRAME_COUNT {{len .Frames}}
          static int {{$GSpritename}}_{{$GAnimation}}_DELAY[] = { {{$lenFrames := dec (len .Frames)}}{{range $i, $e := .Frames}}{{.Delay}}{{if ne $i $lenFrames}}, {{end}}{{end}} };
	{{range .Frames}} {{$GFrame := toupper .Frame}}
#define     {{$GSpritename}}_{{$GAnimation}}_{{$GFrame}}_POLYGON_COUNT {{truelayers (.Layers)}}
		{{range .Layers}}
            {{if .Color}}
#define     {{$GSpritename}}_{{$GAnimation}}_{{$GFrame}}_{{toupper .Layer}}_VERTEX_COUNT {{len .X}}
			static ww_rgba_t {{$GSpritename}}_{{$GAnimation}}_{{$GFrame}}_{{toupper .Layer}}_COLOR = { {{index .TrueColor 0}}, {{index .TrueColor 1}}, {{index .TrueColor 2}} };
			static short {{$GSpritename}}_{{$GAnimation}}_{{$GFrame}}_{{toupper .Layer}}_X[{{len .X}}] = { {{$lenX := dec (len .X)}}{{range $i, $e := .X}}{{$e}}{{if ne $i $lenX}}, {{end}}{{end}} };
			static short {{$GSpritename}}_{{$GAnimation}}_{{$GFrame}}_{{toupper .Layer}}_Y[{{len .Y}}] = { {{$lenY := dec (len .Y)}}{{range $i, $e := .Y}}{{$e}}{{if ne $i $lenY}}, {{end}}{{end}} };
            {{end}}
		{{end}}
	{{end}}
{{end}}

static int {{$GSpritename}}_ALLOC[] = {
	/* anim */
	{{$GSpritename}}_ANIMATION_COUNT,
	
	/* fram */ 
	0
	{{range .Animations}} {{$GAnimation := toupper .Animation}}
    + {{$GSpritename}}_{{$GAnimation}}_FRAME_COUNT
    {{end}},
	
	/* layr */
	0
	{{range .Animations}} {{$GAnimation := toupper .Animation}} {{range .Frames}} {{$GFrame := toupper .Frame}}
	+ {{$GSpritename}}_{{$GAnimation}}_{{$GFrame}}_POLYGON_COUNT
	{{end}} {{end}},
	
	/* vert */
	0
	{{range .Animations}} {{$GAnimation := toupper .Animation}} {{range .Frames}} {{$GFrame := toupper .Frame}} {{range .Layers}} {{if .Color}}
	+ {{$GSpritename}}_{{$GAnimation}}_{{$GFrame}}_{{toupper .Layer}}_VERTEX_COUNT
	{{end}} {{end}} {{end}} {{end}},
};

static int {{$GSpritename}}_FRAMES[] = {
	{{range .Animations}} {{$GAnimation := toupper .Animation}}
    {{$GSpritename}}_{{$GAnimation}}_FRAME_COUNT,
    {{end}}
};

static int * {{$GSpritename}}_DELAYS[] = {
	{{range .Animations}} {{$GAnimation := toupper .Animation}}
    {{$GSpritename}}_{{$GAnimation}}_DELAY,
    {{end}}
};

static int {{$GSpritename}}_POLYGONS[] = {
	{{range .Animations}} {{$GAnimation := toupper .Animation}} {{range .Frames}} {{$GFrame := toupper .Frame}}
	{{$GSpritename}}_{{$GAnimation}}_{{$GFrame}}_POLYGON_COUNT,
	{{end}} {{end}}
};

static unsigned char * {{$GSpritename}}_COLORS[] = {
	{{range .Animations}} {{$GAnimation := toupper .Animation}} {{range .Frames}} {{$GFrame := toupper .Frame}} {{range .Layers}} {{if .Color}}
	{{$GSpritename}}_{{$GAnimation}}_{{$GFrame}}_{{toupper .Layer}}_COLOR,
	{{end}} {{end}} {{end}} {{end}}
};

static int {{$GSpritename}}_VERTICES[] = {
	{{range .Animations}} {{$GAnimation := toupper .Animation}} {{range .Frames}} {{$GFrame := toupper .Frame}} {{range .Layers}} {{if .Color}}
	{{$GSpritename}}_{{$GAnimation}}_{{$GFrame}}_{{toupper .Layer}}_VERTEX_COUNT,
	{{end}} {{end}} {{end}} {{end}}
};

static short * {{$GSpritename}}_ARRAYS[] = {
	{{range .Animations}} {{$GAnimation := toupper .Animation}} {{range .Frames}} {{$GFrame := toupper .Frame}} {{range .Layers}} {{if .Color}}
	{{$GSpritename}}_{{$GAnimation}}_{{$GFrame}}_{{toupper .Layer}}_X,
	{{$GSpritename}}_{{$GAnimation}}_{{$GFrame}}_{{toupper .Layer}}_Y,
	{{end}} {{end}} {{end}} {{end}}
};

ww_reference_t {{$GSpritename}} = {
	.alloc    = {{$GSpritename}}_ALLOC,
	.frames   = {{$GSpritename}}_FRAMES,
	.delays   = {{$GSpritename}}_DELAYS,
	.polygons = {{$GSpritename}}_POLYGONS,
	.colors   = {{$GSpritename}}_COLORS,
	.vertices = {{$GSpritename}}_VERTICES,
	.arrays   = {{$GSpritename}}_ARRAYS
};

`
)
