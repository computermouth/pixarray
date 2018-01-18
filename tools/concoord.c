
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>

typedef struct {
	char *name;
	unsigned char color[3];
	short *x;
	short *y;
	int z_depth;
	int count;
} ww_polygon_t;

typedef enum {
	Find_Shapes,
	Find_Name,
	Find_Points,
	Parse_Points,
	Parse_Color
} stage_t;

int main(int argc, char *argv[]){
	
	if (argc != 2){
		printf("%s usage: \n", argv[0]);
		printf("\t%s [file]\n", argv[0]);
		return 1;
	}
	
	// get full path filename
	char full_path[4097];
	char copy_path[4097];
	char *filepath = NULL;
	filepath = realpath(argv[1], full_path);
	memcpy(copy_path, full_path, 4097 * sizeof(char));
	
	if (filepath != NULL){
		printf("opening: %s\n", filepath);
	} else {
		printf("e: unable to locate file: %s\n", argv[1]);
		return 1;
	}
	
	// get filename
	char *filename = NULL;
	filename = basename(filepath);
	
	// get directory
	char *directory = NULL;
	directory = dirname(filepath);
	
	char *period = NULL;
	period = strrchr(filename, '.');
	
	if( filename == NULL || directory == NULL || period == NULL ){
		printf("e: unable to set output filename\n");
		return 1;
	}
	
	// change the output filename extension
	period[1] = 'h';
	period[2] = '\0';
	char *slash = "/";
	
	// get output file name
	char output_file[4097];
	output_file[0] = '\0';
	strcat(output_file, directory);
	strcat(output_file, slash);
	strcat(output_file, filename);
	
	// open input file
	FILE *in_fp = NULL;
	char *line = NULL;
	size_t len = 0;
	ssize_t read;
	ww_polygon_t ** polys = NULL;
	
	in_fp = fopen(copy_path,"r");
	if(in_fp == NULL){
		printf("e: couldn't open file %s\n", copy_path);
		return 1;
	}
	
	stage_t stage = Find_Shapes;
	int shape_index = 0;
	int num_shapes = 0;
	int point_index = 0;
	unsigned int line_index = 0;
	
	while ( (read = getline(&line, &len, in_fp)) != -1 ) {
		line_index++;
		
		// get number of shapes
		if(stage == Find_Shapes){
			char * needle = "##    Shapes   : ";
			char * found = NULL;
			
			found = strstr(line, needle);
			if(found != NULL){
				num_shapes = atoi(line + 17);
				//~ printf("polygons in file: %d\n", num_shapes);
				
				polys = calloc(num_shapes, sizeof(ww_polygon_t *));
				for(int i = 0; i < num_shapes; i++)
					polys[i] = calloc(1, sizeof(ww_polygon_t));
				
				if(polys == NULL) {
					printf("e: failed to allocate memory for polygons\n");
					return 1;
				}
				
				stage = Find_Name;
			}
			
			continue;
		}
		
		if(stage == Find_Name){
			char * needle = "#declare ";
			char * found = NULL;
			
			found = strstr(line, needle);
			if(found != NULL){
				
				char *name = NULL;
				name = strtok(line+9, " ");
				
				polys[shape_index]->name = calloc(strlen(name) + 1, sizeof(char));
				strcpy(polys[shape_index]->name, name);
				
				//~ printf("name: %s\n", name);
				
				stage = Find_Points;
			}
			
			continue;
		}
		
		if(stage == Find_Points){
			char * needle = " //nr points";
			char * found = NULL;
			
			found = strstr(line, needle);
			if(found != NULL){
				
				char *points = NULL;
				points = strtok(line+4, " ");
				
				polys[shape_index]->count = atoi(points) / 4;
				polys[shape_index]->x = calloc(polys[shape_index]->count, sizeof(short));
				polys[shape_index]->y = calloc(polys[shape_index]->count, sizeof(short));
				
				//~ printf("points: %d\n", polys[shape_index]->count);
				
				stage = Parse_Points;
			}
			
			continue;
		}
		
		if(stage == Parse_Points){
			
			char * dup_line = NULL;
			dup_line = calloc(len, sizeof(char));
			strcpy(dup_line, line);
			
			
			// find x[point_index] between '<' and ','
			char * delim_pos = NULL;
			
			delim_pos = strchr(line, '<');
			if(delim_pos == NULL){
				printf("e: failed to parse x point\n");
				return 1;
			}
			
			char * token = NULL;
			token = strtok(delim_pos+1, ",");
			
			// set x[point_index]
			polys[shape_index]->x[point_index] = (int)atof(token);
			// restore line
			strcpy(line, dup_line);
			
			// find y[point_index] between ',' and '>'
			delim_pos = NULL;
			delim_pos = strchr(line, ',');
			if(delim_pos == NULL){
				printf("e: failed to parse y point\n");
				return 1;
			}
			
			token = NULL;
			token = strtok(delim_pos+1, ">");
			
			// set y[point_index]
			polys[shape_index]->y[point_index] = (int)atof(token);
			
			//~ printf("x[%d]: %d\ty[%d]: %d\n", point_index, polys[shape_index]->x[point_index], point_index, polys[shape_index]->y[point_index]);
			
			point_index += 1;
			if(point_index == polys[shape_index]->count){
				point_index = 0;
				stage = Parse_Color;
			}
			
			free(dup_line);
			
			continue;
		}
		
		if(stage == Parse_Color){
			char * needle = "_COLOR    = ";
			char * found = NULL;
			
			found = strstr(line, needle);
			
			if(found == NULL)
				continue;
				
			//~ printf("line[%d]: %s", line_index, line);
			
			//~ char * dup_line = NULL;
			//~ dup_line = calloc(len, sizeof(char));
			//~ strcpy(dup_line, line);
			
			
			// find color[r] between '<' and ','
			char * delim_pos = NULL;
			
			delim_pos = strchr(line, '<');
			if(delim_pos == NULL){
				printf("e: failed to parse x point\n");
				return 1;
			}
			
			char * token = NULL;
			token = strtok(delim_pos+1, ",");
			
			// set color[r]
			//~ printf("token0: %s\n", token);
			polys[shape_index]->color[0] = (int)(atof(token) * 255);
			token = strtok(NULL, ",");
			//~ printf("token1: %s\n", token);
			polys[shape_index]->color[1] = (int)(atof(token) * 255);
			token = strtok(NULL, ",");
			//~ printf("token2: %s\n", token);
			polys[shape_index]->color[2] = (int)(atof(token) * 255);
			
			//~ printf("c[0]: %d\nc[1]: %d\nc[2]: %d\n",
				//~ polys[shape_index]->color[0],
				//~ polys[shape_index]->color[1],
				//~ polys[shape_index]->color[2]);
			
			
			stage = Find_Name;
			shape_index += 1;
			
			if(shape_index == num_shapes)
				break;
			
			continue;
		}
		
	}
	
	printf("writing out to: %s\n", output_file);
	
	// create new file
	FILE *out_fp = NULL;
		
	out_fp = fopen(output_file,"w+");
	if(out_fp == NULL){
		printf("e: couldn't open file %s\n", output_file);
		return 1;
	}
	
	int rc = 0;
	// write shapes out to header
	rc = fprintf(out_fp, "\n#include\"ww.h\"\n\n#ifndef TEST_SHAPES_H\n#define TEST_SHAPES_H\n\n");
	if(rc < 0){
		printf("e: failed to write to %s", output_file);
		return rc;
	}
	
	// get filename without extension
	period[0] = '\0';
	char * frame_name = filename;
	
	// write externs and declarations
	for(int i = 0; i < num_shapes; i++){
		rc = fprintf(out_fp, 
			"extern ww_polygon_t * %s_%s;\nww_polygon_t * %s_%s = NULL;\n", 
			frame_name, polys[i]->name, 
			frame_name, polys[i]->name);
		if(rc < 0){
			printf("e: failed to write to %s", output_file);
			return rc;
		}
	}
	
	// start init function
	rc = fprintf(out_fp, "\n\nvoid init_%s(){\n\n", frame_name);
	if(rc < 0){
		printf("e: failed to write to %s", output_file);
		return rc;
	}
	
	// write out polygons
	for(int i = 0; i < num_shapes; i++){
		
		// color
		rc = fprintf(out_fp, 
			"\tww_rgba_t %s_%s_color = { %d, %d, %d };\n",
			frame_name, polys[i]->name, 
			polys[i]->color[0], polys[i]->color[1], polys[i]->color[2]);
		if(rc < 0){
			printf("e: failed to write to %s", output_file);
			return rc;
		}
		
		// x begin
		rc = fprintf(out_fp, 
			"\tshort %s_%s_x[%d] = {", 
			frame_name, polys[i]->name, polys[i]->count );
		if(rc < 0){
			printf("e: failed to write to %s", output_file);
			return rc;
		}
		// x content
		for(int j = 0; j < polys[i]->count; j++){
			rc = fprintf(out_fp, 
				" %d,", 
				polys[i]->x[j] );
			if(rc < 0){
				printf("e: failed to write to %s", output_file);
				return rc;
			}
		}
		// x end
		rc = fprintf(out_fp, 
			"};\n");
		if(rc < 0){
			printf("e: failed to write to %s", output_file);
			return rc;
		}
		
		// y begin
		rc = fprintf(out_fp, 
			"\tshort %s_%s_y[%d] = {", 
			frame_name, polys[i]->name, polys[i]->count );
		if(rc < 0){
			printf("e: failed to write to %s", output_file);
			return rc;
		}
		// y content
		for(int j = 0; j < polys[i]->count; j++){
			rc = fprintf(out_fp, 
				" %d,", 
				polys[i]->y[j] );
			if(rc < 0){
				printf("e: failed to write to %s", output_file);
				return rc;
			}
		}
		// y end
		rc = fprintf(out_fp, 
			"};\n");
		if(rc < 0){
			printf("e: failed to write to %s", output_file);
			return rc;
		}
		
		// new_polygon
		rc = fprintf(out_fp, 
			"\t%s_%s = ww_new_polygon(%s_%s_color, %s_%s_x, %s_%s_y, %d);\n\n",
			frame_name, polys[i]->name, 
			frame_name, polys[i]->name,
			frame_name, polys[i]->name,
			frame_name, polys[i]->name,
			polys[i]->count);
		if(rc < 0){
			printf("e: failed to write to %s", output_file);
			return rc;
		}
		
	}
	
	// end init function
	rc = fprintf(out_fp, 
		"\n\n}\n\n");
	if(rc < 0){
		printf("e: failed to write to %s", output_file);
		return rc;
	}
	
	// end file
	rc = fprintf(out_fp, 
		"#endif\n");
	if(rc < 0){
		printf("e: failed to write to %s", output_file);
		return rc;
	}
	
	for(int i = 0; i < num_shapes; i++){
		free(polys[i]->name);
		free(polys[i]->x);
		free(polys[i]->y);
		free(polys[i]);
	}
	
	free(polys);
	fclose(in_fp);
	fclose(out_fp);
	if ( line !=NULL )
		free(line);
	
	return 0;
}
