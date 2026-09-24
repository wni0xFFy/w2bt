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

	fd_poll_t* epfd = create_poll(10, -1);
	if(epfd == NULL){
		fprintf(stderr, "[ERROR] Couldn't open create epoll!");
	 	return -1;
	}

	fprintf(stdout, "[INIT] EPOLL CREATED\n");

	add_poll_task(epfd, fd, ACCEPT, EPOLLIN);

	fprintf(stdout, "[INIT] SOCKET ADDED TO EPOLL\n");

	fprintf(stdout, "[RUNNING] INITIATION SUCCESSFUL\n");

	while(1){
		int tasks_count = 0;
		task_t** tasks = wait_tasks(epfd, &tasks_count);

		if(tasks == NULL) break; 
		for(int i = 0; i < tasks_count; i++){
			if(tasks[i]->state == ACCEPT){	
				accept_handler(fd, epfd);
				fprintf(stdout, "[RUNNING] CLIENT CONNECTED\n");
			}
			else if(tasks[i]->state == IN){
				read_handler(tasks[i]);
				fprintf(stdout, "[RUNNING] CLIENT SENDED SOMETHING\n");
			}
			else if(tasks[i]->state == CLOSED){
 				close_handler(epfd, tasks[i]);
 				fprintf(stdout, "[RUNNING] CONNECTION CLOSED BY CLIENT\n");
			}

			else{
				free(tasks[i]);
				fprintf(stderr, "ghost socket with wrong state");
			}
		}
		free(tasks);
	}
	delete_poll(epfd);
	return -1;
}
