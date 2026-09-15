#pragma once 
#include <stdint.h>
#include <w2bt/socket.h>
typedef enum{
	IN,
	OUT, 
	ACCEPT,
} STATES;

typedef struct{
	int fd;
	STATES state;
} task_t;

int create_sockets_poll(sock_t* sc);

