
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
		printf("input found: %s\n", filepath);
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
	FILE *fp = NULL;
	char *line = NULL;
	size_t len = 0;
	ssize_t read;
	ww_polygon_t ** polys = NULL;
	
	fp = fopen(copy_path,"r");
	if(fp == NULL){
		printf("e: couldn't open file %s\n", copy_path);
		return 1;
	}
	
	stage_t stage = Find_Shapes;
	int shape_index = 0;
	int num_shapes = 0;
	int point_index = 0;
	
	while ( (read = getline(&line, &len, fp)) != -1 ) {
		
		// get number of shapes
		if(stage == Find_Shapes){
			char * needle = "##    Shapes   : ";
			char * found = NULL;
			
			found = strstr(line, needle);
			if(found != NULL){
				num_shapes = atoi(line + 17);
				printf("polygons in file: %d\n", num_shapes);
				
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
			free(line);
			line = dup_line;
			
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
				shape_index += 1;
				point_index = 0;
				stage = Parse_Color;
			}
			
			continue;
		}
		
	
	}
	
	for(int i = 0; i < num_shapes; i++){
		free(polys[i]->name);
		free(polys[i]->x);
		free(polys[i]->y);
		free(polys[i]);
	}
	
	free(polys);
	fclose(fp);
	if ( line !=NULL )
		free(line);
	
	
	
	return 0;
}
