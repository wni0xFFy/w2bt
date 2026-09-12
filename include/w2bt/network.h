#pragma once 

typedef enum{
	IN,
	OUT, 
	ACCEPT,
} STATES;

typedef struct{
	int fd;
	STATES state;
	void* data;
	uint16_t size;
} data;

