#include <w2bt/network.h>
#include "include/socket.h"
#include <stdio.h>
#include <unistd.h>
int main(){
	sock_t* fd = create_socket(25656);
	sock_t* client_socket = accept_socket(fd);
	//char* data = "poop";
	while(1){
		char* data = receive_socket(client_socket, 16);
		send_socket(client_socket, data, 16);
		//fprintf(stderr, "%s", data);
	}
	destroy_socket(client_socket);
	destroy_socket(fd);
	return -1;
}