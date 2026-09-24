#include <w2bt/network/network.h>
#include <w2bt/core/socket.h>	
#include <stdio.h>
#include <stdlib.h>
int main(){
	fd_poll_t* epl = create_poll(15, 30000);
	if(epl == NULL){
		printf("[ERROR] EPOLL NOT STARTED");
		return -1;
	}
	sock_t* server = create_socket(25656, 10);
	if(server == NULL){
		printf("[ERROR] SOCKET NOT OPENED");
		return -2;
	}

	network_main(epl, server);
	delete_poll(epl);	
}