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
	uint8_t* buffer = malloc(32512);
	while(1){
		int s = 0;
		task_t** tasks = wait_tasks(epfd, &s);
		if(tasks == NULL) break; 
		for(int i = 0; i < s; i++){
			if(tasks[i]->state == ACCEPT){	
				accept_handler(fd, epfd);
				fprintf(stdout, "%d : CLIENT %x CONNECTES\n", i, tasks[i]);
			}
			else if(tasks[i]->state == IN){
				read_handler(tasks[i], buffer, 256);
				buffer[255] = '\0';
				fprintf(stdout, "%d : CLIENT SEND\n", i);
				for(int i = 0; i < 256; i++){
					buffer[i] = 0;
				}
			}
			else if(tasks[i]->state == CLOSED){
				if(tasks[i]->state == CLOSED) fprintf(stdout, "CONNECTION %x BY CLIENT\n", tasks[i]);
 				close_handler(epfd, tasks[i]);
			}

			else{
				fprintf(stdout, "%d : GHOST SOCKET %x\n ", i, tasks[i]);
				free(tasks[i]);
				break;
			}
		}
		free(tasks);
		fprintf(stdout, "events : %d\n", s);
		fprintf(stdout, "c1 : %d\n", epfd->tasks_lenght);
	}
	free(buffer);
	fprintf(stdout, "c : %d", epfd->tasks_lenght);
	delete_sockets_poll(epfd);
	return -1;
}
