#pragma once 
#include <netinet/in.h>

typedef struct{
	int fd; 
	struct sockaddr_in settings;
	int settings_size;
} sock_t;

sock_t create_socket(int port);
sock_t accept_socket(sock_t* master);