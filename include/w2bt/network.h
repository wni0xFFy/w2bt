#pragma once 

typedef enum{
	SERVER_READ,
	SERVER_WRITE,
	CLIENT_WRITE,
	CLIENT_READ,
} server_states;

typedef struct{
	int fd;
	server_states state;
	void* data;
} operation;

