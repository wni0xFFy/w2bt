#include <w2bt/network.h>
#include "include/socket.h"
#include "include/network_buffer.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
int main(){
	sock_t* fd = create_socket(25656);
	sock_t* client_socket = accept_socket(fd);
	network_buffer* buf = create_buffer(256, 10);

	fprintf(stderr, "conncetion establisied");
	nonblocking_socket(client_socket);
	
	while(1){
		int e = receive_socket(client_socket, buf);
		if(e < 0){
			fprintf(stderr, "null.");
		}
		else{
			fprintf(stderr, "%s", buf->data);
			return 1;
		}
	}
	destroy_socket(client_socket);
	destroy_socket(fd);
	return -1;
}