#include <w2bt/network.h>
#include "include/socket.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
int main(){
	sock_t* fd = create_socket(25656);
	sock_t* client_socket = accept_socket(fd);
	fprintf(stderr, "conncetion establisied");
	nonblocking_socket(client_socket);
	uint8_t* buffer = malloc(256);
	fprintf(stderr, "waiting..\n");
	while(1){
		int e = receive_socket(client_socket, buffer, 256);
		if(!(e < 0)){
			send_socket(client_socket, buffer, 256);
			fprintf(stderr, "%s", buffer);
			fprintf(stderr, "waiting..\n");
		}
	}
	destroy_socket(client_socket);
	destroy_socket(fd);
	return -1;
}