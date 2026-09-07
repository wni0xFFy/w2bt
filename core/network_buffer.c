#include "include/socket.h"
#include <string.h>
#include <stdlib.h>
network_buffer* create_buffer(uint16_t expected_size, uint16_t attempts){
	network_buffer* buf = malloc(sizeof(network_buffer));
	buf->data = malloc(expected_size);
	buf->real_size = 0;
	buf->attempts = attempts;
	buf->size = expected_size;
	return buf;
}

int write_buffer(network_buffer* buf, uint8_t* data, uint16_t size){
	if(buf == NULL || data == NULL) return -1;
	if(buf->real_size + size > buf->size) return -2;

	memcpy((buf->data + buf->real_size + size), data, size);
	return 0;
}

uint8_t* read_buffer(network_buffer* buf, int offset, int size){
	if(buf == NULL) return NULL; 
	if(buf->size < size + offset) return NULL;

	uint8_t* readed = malloc(size);
	if(readed == NULL) return NULL;

	memcpy(readed, (buf->data + offset), size);
	return readed;
}

void delete_buffer(network_buffer* buf){
	free(buf->data);
	free(buf);
}