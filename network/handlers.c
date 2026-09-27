#include <w2bt/network/handlers.h>
#include <w2bt/core/socket.h>
#include <w2bt/network/network.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/epoll.h>

int accept_handler(sock_t* serv, fd_poll_t* epfd){
	sock_t* client = accept_socket(serv);
	if(client == NULL) return -1;

	nonblocking_socket(client);
	if(add_poll_event(epfd, client, IN, EPOLLIN | EPOLLRDHUP) != NULL) return -2;
	return 0;
}

int read_handler(event_t* e){
	return 0;
}

int close_handler(fd_poll_t* epl, event_t* e){
	delete_event(epl, e);
	return 0;
}
