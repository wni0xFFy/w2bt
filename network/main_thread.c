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
	int epfd = create_sockets_poll(fd);

	if(epfd < 1){
		printf("Error with opening epoll fd.");
		return -1;
	}

	uint8_t* buffer = malloc(256);
	fprintf(stderr, "waiting..\n");

	struct epoll_event evs[32] = {0};

	while(1){
		int c = epoll_wait(epfd, &evs, MAXEVENTS, -1);
		for(int i = 0; i < c; i++){
			task_t* t = evs[i].data.ptr;
			if(t->state == ACCEPT){
				printf("YEEEEEEAAAAAAAAAAH");
				return -1;
			}
		}
	}
	close(epfd);
	//destroy_socket(client_socket);
	destroy_socket(fd);
	return -1;
}