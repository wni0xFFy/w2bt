#include <w2bt/network/network.h>
#include <w2bt/core/socket.h>
#include <w2bt/network/handlers.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>

int main(){
	setbuf(stdout, NULL);

	sock_t* fd = create_socket(25656, 5);
	if(fd == NULL) return -1;
	nonblocking_socket(fd);

	fd_poll_t* epfd = create_sockets_poll(10, -1);
	if(epfd == NULL) return -1;

	add_socket_event(epfd, fd, ACCEPT, EPOLLIN);
	fprintf(stdout, "waiting..\n");
	
	int timeout = 10;
	while(timeout){
		int s = 0;
		task_t** tasks = wait_tasks(epfd, &s);
		if(tasks == NULL) break; 
		for(int i = 0; i < s; i++){
			if(tasks[i]->state == ACCEPT){	
				accept_handler(fd, epfd);
				fprintf(stdout, "%d : CLIENT CONNECTES\n", i);
			}
			else if(tasks[i]->state == IN){
				read_handler(tasks[i], NULL);
				fprintf(stdout, "%d : CLIENT SEND\n", i);
			}
			else{
				fprintf(stdout, "%d : GHOST SOCKET\n", i);
				free(tasks[i]);
			}
		}
		free(tasks);
		timeout--;
	}

	destroy_socket(fd);
	delete_sockets_poll(epfd);
	return -1;
}