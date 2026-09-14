#include <w2bt/network.h>
#include <w2bt/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
int main(){
	sock_t* fd = create_socket(25656, 5);
	uint8_t* buffer = malloc(256);
	fprintf(stderr, "waiting..\n");
	nonblocking_socket(fd);

	while(1){
		int c = epoll_wait();
		int e = receive_socket(client_socket, buffer, 256, 0);
		if(e == -2){
			fprintf(stderr, "connection closed by client");
			break;
		}
		if(!(e < 0)){
			send_socket(client_socket, buffer, 256, 0);
			fprintf(stderr, "%s\n", buffer);
			fprintf(stderr, "waiting..\n");
			memset(buffer, 0, 256);
		}
	}
	destroy_socket(client_socket);
	destroy_socket(fd);
	return -1;
}