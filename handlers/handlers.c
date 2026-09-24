#include <w2bt/handlers/handlers.h>
#include <w2bt/core/socket.h>
#include <w2bt/network/network.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/epoll.h>

int accept_handler(sock_t* serv, fd_poll_t* epfd){
	sock_t* client = accept_socket(serv);
	if(client == NULL) return -1;

	nonblocking_socket(client);
	if(add_poll_task(epfd, client, IN, EPOLLIN | EPOLLRDHUP) != NULL) return -2;
	return 0;
}

int read_handler(task_t* t){
	return 0;
}

int close_handler(fd_poll_t* epl, task_t* t){
	//delete_task(epl, t);
	return 0;
}
