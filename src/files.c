
#include <stdio.h>
#include <unistd.h>
#include "mpack.h"
#include "files.h"

typedef struct {
	// player stuff
	uint32_t level;
	uint32_t max_hp;
	uint32_t hp;
	uint32_t speed;
	uint32_t baseatk;
	uint32_t basedef;
	uint32_t atk[4];
	uint32_t def[4];
	uint32_t exp[9];
	uint32_t x_pos;
	uint32_t y_pos;
	// misc
	uint32_t volume;
	uint32_t scale;
} save_file_t;

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
	char *outname = "test.mp";
	
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
	mpack_tree_init_filename(&tree, "test.mp", 0);
	mpack_tree_parse(&tree);
	mpack_node_t root = mpack_tree_root(&tree);

	// extract the example data on the msgpack homepage
	uint32_t version = mpack_node_u32(mpack_node_map_cstr(root, "v"));
	int array_len = mpack_node_array_length(mpack_node_map_cstr(root, "p"));
	
	mpack_node_t m = mpack_node_array_at(mpack_node_map_cstr(root, "p"), 25);
	int end_array = mpack_node_u32(m);
	
	// clean up and check for errors
	if ( end_array != 255 || mpack_tree_destroy(&tree) != mpack_ok) {
		fprintf(stderr, "An error occurred decoding the data!\n");
		return -1;
	}
	
	return 0;
}
