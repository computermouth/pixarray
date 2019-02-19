
#include <stdio.h>
#include <unistd.h>
#include "mpack.h"

int mwrite(){
	// encode to memory buffer
	char* data;
	size_t size;
	mpack_writer_t writer;
	mpack_writer_init_growable(&writer, &data, &size);
	int rc = 0;

	// write the example on the msgpack homepage
	mpack_start_map(&writer, 2);
	mpack_write_cstr(&writer, "v");
	mpack_write_u8(&writer, 2);
	mpack_write_cstr(&writer, "p");
	mpack_start_array(&writer, 10);
	mpack_write_u32(&writer, 0);
	mpack_write_u32(&writer, 1);
	mpack_write_u32(&writer, 2);
	mpack_write_u32(&writer, 3);
	mpack_write_u32(&writer, 4);
	mpack_write_u32(&writer, 5);
	mpack_write_u32(&writer, 6);
	mpack_write_u32(&writer, 7);
	mpack_write_u32(&writer, 8);
	mpack_write_u32(&writer, 9);
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
	
	return 0;
}

int mread(){
	// parse a file into a node tree
	mpack_tree_t tree;
	mpack_tree_init_filename(&tree, "test.mp", 0);
	mpack_tree_parse(&tree);
	mpack_node_t root = mpack_tree_root(&tree);

	// extract the example data on the msgpack homepage
	uint32_t version = mpack_node_u32(mpack_node_map_cstr(root, "v"));
	int array_len = mpack_node_array_length(mpack_node_map_cstr(root, "p"));
	
	mpack_node_t m = mpack_node_array_at(mpack_node_map_cstr(root, "p"), 9);
	int sub_two = mpack_node_u32(m);
	
	printf("ver: %d\n", version);
	printf("len: %d\n", array_len);
	printf("m: %p\n", &m);
	printf("sub_two: %d\n", sub_two);
	
	// clean up and check for errors
	if (mpack_tree_destroy(&tree) != mpack_ok) {
		fprintf(stderr, "An error occurred decoding the data!\n");
		return -1;
	}
	
	return 0;
}

int main(int argc, char *argv[]){
	
	if(argc < 2){
		return 1;
	}
	
	if( strcmp(argv[1], "w") == 0 ){
		printf("write\n");
		int rc = mwrite();
		if( rc != 0 ){
			printf("error on write\n");
			return rc;
		}
	} else if ( strcmp(argv[1], "r") == 0 ){
		printf("read\n");
		int rc = mread();
		if( rc != 0 ){
			printf("error on read\n");
			return rc;
		}
	} else {
		printf("?\n");
		return 1;
	}
	
	return 0;
}
