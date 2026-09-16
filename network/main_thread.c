#include <w2bt/network.h>
#include <w2bt/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>

const int MAXEVENTS = 12;

int main(){
	sock_t* fd = create_socket(25656, 5);
	nonblocking_socket(fd);

	fd_poll_t* epfd = create_sockets_poll(10);
	if(epfd == NULL) return -1;

	add_socket_event(epfd, fd, ACCEPT, EPOLLIN);
	uint8_t* buffer = malloc(256);
	fprintf(stderr, "waiting..\n");

	while(1){
		int c = epoll_wait(epfd->epoll_fd, epfd->evs, 16, -1);
		for(int i = 0; i < c; i++){
			task_t* t = epfd->evs[i].data.ptr;
			if(t->state == ACCEPT){
				sock_t* client = accept_socket(fd);
				nonblocking_socket(client);
				add_socket_event(epfd, client, IN, EPOLLIN);
				continue;
			}
			if(t->state == IN){
				fprintf(stderr, "YEAH. THAT'S FUCKING AWESOME");
				destroy_socket(t->data);
			}
		}
	}
	destroy_socket(fd);
	delete_sockets_poll(epfd);
	return -1;
}