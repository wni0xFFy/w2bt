#include <w2bt/network.h>
#include "include/socket.h"
#include <stdlib.h>
int srv_init(sock_t* sc){
	task op = {0};
	op.fd = sc->fd;
	op.state = ACCEPT;
	op.data = NULL;

	//int epfd = epoll_create 
	//NETWORK -> 
	//TASK HANDLER -> 
	//DISTRIBUTOR ->
	//WORKER -> 
	//TASK HANDLER -> 
	//NETWORK
}

operation srv_read(){

}