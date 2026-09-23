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
	if(fd == NULL){
		fprintf(stderr, "Couldn't open socket!");
		return -1;
	}

	fprintf(stdout, "[INIT] SOCKET OPENED\n");

	nonblocking_socket(fd);

	fd_poll_t* epfd = create_sockets_poll(10, -1);
	if(epfd == NULL){
		fprintf(stderr, "Couldn't open create epoll!");
	 	return -1;
	}

	fprintf(stdout, "[INIT] EPOLL CREATED\n");

	add_poll_task(epfd, fd, ACCEPT, EPOLLIN);

	fprintf(stdout, "[INIT] SOCKET ADDED TO EPOLL\n");

	fprintf(stdout, "[RUNNING] INITIATION SUCCESSFUL\n");

	uint8_t* buffer = malloc(32512);
	while(1){
		int s = 0;
		task_t** tasks = wait_tasks(epfd, &s);
		if(tasks == NULL) break; 
		for(int i = 0; i < s; i++){
			if(tasks[i]->state == ACCEPT){	
				accept_handler(fd, epfd);
				fprintf(stdout, "[RUNNING] CLIENT CONNECTED\n");
			}
			else if(tasks[i]->state == IN){
				read_handler(tasks[i], buffer, 256);
				buffer[255] = '\0';
				fprintf(stdout, "[RUNNING] CLIENT SENDED SOMETHING\n");
				for(int i = 0; i < 256; i++){
					buffer[i] = 0;
				}
			}
			else if(tasks[i]->state == CLOSED){
				if(tasks[i]->state == CLOSED) fprintf(stdout, "[RUNNING] CONNECTION CLOSED BY CLIENT\n");
 				close_handler(epfd, tasks[i]);
			}

			else{
				fprintf(stderr, "ghost socket. wrong state. data pointer 0x%x\n", (unsigned int)tasks[i]);
				free(tasks[i]);
				break;
			}
		}
		free(tasks);
	}
	free(buffer);
	delete_sockets_poll(epfd);
	return -1;
}
