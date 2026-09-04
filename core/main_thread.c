#include <w2bt/network.h>
#include <sys/socket.h>
#include "include/socket.h"
#include <stdio.h>
#include <unistd.h>
int main(){
	sock_t fd = create_socket(25656);
	sock_t client_socket = accept_socket(&fd);
	int x = 'C';
	while(1){
		send(client_socket.fd, &x, sizeof(int), MSG_NOSIGNAL);	
	}
	printf("ZEROONEZEROONE %d", client_socket.settings.sin_port);
	return -1;
}