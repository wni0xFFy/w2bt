#include <w2bt/network.h>
#include <w2bt/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/epoll.h>

int create_sockets_poll(sock_t* sc){
	task_t* op = malloc(sizeof(task_t));
	op->fd = sc->fd;
	op->state = ACCEPT;

	int epfd = epoll_create1(0);
	if(epfd == -1){
		return -1;
	}

	struct epoll_event server_event;

	server_event.events = EPOLLIN;
	server_event.data.ptr = op;

	if(epoll_ctl(epfd, EPOLL_CTL_ADD, sc->fd, &server_event)){
		close(epfd);
		return -2;
	}						

	return epfd;
}

//operation srv_read(){

//}