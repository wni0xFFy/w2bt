#pragma once 
#include <netinet/in.h>
typedef struct{
	int fd; 
	struct sockaddr_in settings;
	int settings_size;
} sock_t;


void destroy_socket(sock_t* sock);
sock_t* create_socket(int port);
sock_t* accept_socket(sock_t* self);
int receive_socket(sock_t* client, void* buffer, uint16_t size);
int send_socket(sock_t* client, void* buffer, uint16_t size);
void nonblocking_socket(sock_t* sc);