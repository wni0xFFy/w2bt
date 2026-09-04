#include <w2bt/network.h>
#include <sys/socket.h>
#include "include/socket.h"
#include <stdio.h>
#include <unistd.h>
int main(){
	sock_t* fd = create_socket(25656);
	sock_t* client_socket = accept_socket(fd);
	for(int i = 0; i < 10; i++){
		char* data = receive_socket(client_socket, 256);
		send_socket(client_socket, data, 256);	
	}
	destroy_socket(client_socket);
	destroy_socket(fd);
	return -1;
}