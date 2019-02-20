
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include "ww.h"
#include "states.h"
#include "mpack.h"
#include "files.h"


char *home_dir = NULL;
char *local_dir = "/.local";
char *share_dir = "/share";
char game_dir[500];
char game_file[500];
char tmp_path[500];

int check_dir(char *tp){

	struct stat st = {0};
	
	// create
	if (stat(tp, &st) == -1) {
#ifdef _WIN32
		mkdir(tp);
#else
		mkdir(tp, 0755);
#endif
	}
	
	// fail if nonexistent
	if (stat(tp, &st) == -1) {
		return 1;
	}
	
	return 0;
}

int check_file(char *tp, game_state_t * gs){

	struct stat st = {0};
	
	int rc = 0;
	
	// create
	if (stat(tp, &st) == -1) {
		rc += mwrite(gs);
		if (rc != 0)
			return rc;
		
		rc += mread(gs);
		if (rc != 0)
			return rc;
	}
	
	// fail if nonexistent
	if (stat(tp, &st) == -1) {
		return 1;
	}
	
	return 0;
}

int verify_or_create_save(game_state_t * gs){

	ww_window_s *window_p = ( ww_window_s* ) calloc( 1, sizeof( ww_window_s ) );
	window = (ww_window_t *) window_p;
	
	int rc = 0;


	home_dir = getenv("HOME");

	memset(game_dir, 0, 500 * sizeof(char));
	sprintf(game_dir, "/%s", window_p->gamename);
	memset(game_file, 0, 500 * sizeof(char));
	sprintf(game_file, "/%s.ich", window_p->gamename);

	// check homedir exists
	memset(tmp_path, 0, 500 * sizeof(char));
	sprintf(tmp_path, "%s", home_dir);
	rc += check_dir(tmp_path);
	if (rc != 0) {
		printf("E: Failed to find/create %s\n", tmp_path);
		return rc;
	}
	
	// check local_dir exists
	memset(tmp_path, 0, 500 * sizeof(char));
	sprintf(tmp_path, "%s%s", home_dir, local_dir);
	rc += check_dir(tmp_path);
	if (rc != 0) {
		printf("E: Failed to find/create %s\n", tmp_path);
		return rc;
	}
	
	// check share_dir exists
	memset(tmp_path, 0, 500 * sizeof(char));
	sprintf(tmp_path, "%s%s%s", home_dir, local_dir, share_dir);
	rc += check_dir(tmp_path);
	if (rc != 0) {
		printf("E: Failed to find/create %s\n", tmp_path);
		return rc;
	}
	
	// check game_dir exists
	memset(tmp_path, 0, 500 * sizeof(char));
	sprintf(tmp_path, "%s%s%s%s", home_dir, local_dir, share_dir, game_dir);
	rc += check_dir(tmp_path);
	if (rc != 0) {
		printf("E: Failed to find/create %s\n", tmp_path);
		return rc;
	}
	
	// check game_file exists
	memset(tmp_path, 0, 500 * sizeof(char));
	sprintf(tmp_path, "%s%s%s%s%s", home_dir, local_dir, share_dir, game_dir, game_file);
	rc += check_file(tmp_path, gs);
	if (rc != 0) {
		printf("E: Failed to find/create %s\n", tmp_path);
		return rc;
	}

	return rc;
}

int mwrite(game_state_t * gs){
	// encode to memory buffer
	char* data;
	size_t size;
	mpack_writer_t writer;
	mpack_writer_init_growable(&writer, &data, &size);
	int rc = 0;

	// write the example on the msgpack homepage
	mpack_start_map(&writer, 2);
	mpack_write_cstr(&writer, "v");
	mpack_write_u8(&writer, 0);
	mpack_write_cstr(&writer, "p");
	mpack_start_array(&writer, 26);
	mpack_write_u32(&writer, gs->play_state.player.level   );
	mpack_write_u32(&writer, gs->play_state.player.max_hp  );
	mpack_write_u32(&writer, gs->play_state.player.hp      );
	mpack_write_u32(&writer, gs->play_state.player.speed   );
	mpack_write_u32(&writer, gs->play_state.player.baseatk );
	mpack_write_u32(&writer, gs->play_state.player.basedef );
	mpack_write_u32(&writer, gs->play_state.player.atk[0]  );
	mpack_write_u32(&writer, gs->play_state.player.atk[1]  );
	mpack_write_u32(&writer, gs->play_state.player.atk[2]  );
	mpack_write_u32(&writer, gs->play_state.player.atk[3]  );
	mpack_write_u32(&writer, gs->play_state.player.def[0]  );
	mpack_write_u32(&writer, gs->play_state.player.def[1]  );
	mpack_write_u32(&writer, gs->play_state.player.def[2]  );
	mpack_write_u32(&writer, gs->play_state.player.def[3]  );
	mpack_write_u32(&writer, gs->play_state.player.exp[0]  );
	mpack_write_u32(&writer, gs->play_state.player.exp[1]  );
	mpack_write_u32(&writer, gs->play_state.player.exp[2]  );
	mpack_write_u32(&writer, gs->play_state.player.exp[3]  );
	mpack_write_u32(&writer, gs->play_state.player.exp[4]  );
	mpack_write_u32(&writer, gs->play_state.player.exp[5]  );
	mpack_write_u32(&writer, gs->play_state.player.exp[6]  );
	mpack_write_u32(&writer, gs->play_state.player.exp[7]  );
	mpack_write_u32(&writer, gs->play_state.player.exp[8]  );
	mpack_write_u32(&writer, gs->options_menu_state.volume );
	mpack_write_u32(&writer, gs->options_menu_state.scale  );
	mpack_write_u32(&writer, 255  );
	mpack_finish_array(&writer);
	mpack_finish_map(&writer);

	// finish writing
	if (mpack_writer_destroy(&writer) != mpack_ok) {
		fprintf(stderr, "An error occurred encoding the data!\n");
		return -1;
	}
	
	// test if file exists
	FILE *out = NULL;
	
	memset(tmp_path, 0, 500 * sizeof(char));
	sprintf(tmp_path, "%s%s%s%s%s", home_dir, local_dir, share_dir, game_dir, game_file);
	
	char *outname = tmp_path;
	
	if( access(outname, F_OK) != -1) {
		printf("E: file '%s' already exists\n", outname);
		rc = 2;
		return rc;
	}
	
	// open and write
	out = fopen(outname, "wb");
	
	for(size_t i = 0; i < size; i++){
		fputc(data[i], out);
	}
	
	// use the data
	//~ do_something_with_data(data, size);
	free(data);
	if (out != NULL) fclose(out);
	
	// validate write
	return mread(gs);
}

int mread(game_state_t * gs){
	// parse a file into a node tree
	mpack_tree_t tree;
	
	memset(tmp_path, 0, 500 * sizeof(char));
	sprintf(tmp_path, "%s%s%s%s%s", home_dir, local_dir, share_dir, game_dir, game_file);
	
	mpack_tree_init_filename(&tree, tmp_path, 0);
	mpack_tree_parse(&tree);
	mpack_node_t root = mpack_tree_root(&tree);

	// extract the example data on the msgpack homepage
	//~ uint32_t version = mpack_node_u32(mpack_node_map_cstr(root, "v"));
	//~ int array_len = mpack_node_array_length(mpack_node_map_cstr(root, "p"));
	
	mpack_node_t m = mpack_node_array_at(mpack_node_map_cstr(root, "p"), 25);
	int end_array = mpack_node_u32(m);
	
	// clean up and check for errors
	if ( end_array != 255 || mpack_tree_destroy(&tree) != mpack_ok) {
		fprintf(stderr, "An error occurred decoding the data!\n");
		return -1;
	}
	
	return 0;
}
