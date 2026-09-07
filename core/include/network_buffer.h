#pragma once 

typedef struct{
	uint8_t* data;
	uint16_t real_size;
	uint16_t size;
	uint8_t attempts;
} network_buffer;

network_buffer* create_buffer(uint16_t expected_size, uint16_t attempts);
void delete_buffer(network_buffer* buf);
int write_buffer(network_buffer* buf, uint8_t* data, uint16_t size);
uint8_t* read_buffer(network_buffer* buf, int offset, int size);