#pragma once 
#include <netinet/in.h>

typedef struct{
	int fd; 
	struct sockaddr_in settings;
	int settings_size;
} sock_t;

typedef struct{
	uint8_t* data;
	uint16_t real_size;
	uint16_t expected_size;
	uint8_t attempts;
} network_buffer;

void destroy_socket(sock_t* sock);
sock_t* create_socket(int port);
sock_t* accept_socket(sock_t* self);
char* receive_socket(sock_t* client, uint16_t expected_size);
int send_socket(sock_t* client, buffer* buf);
void nonblocking_socket(sock_t* sc);