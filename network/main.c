#include <w2bt/network/network.h>
#include <w2bt/core/socket.h>
#include <w2bt/handlers/handlers.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>

int network_main(fd_poll_t* epl, sock_t* server){
	setbuf(stdout, NULL);
	add_poll_task(epl, server, ACCEPT, EPOLLIN);

	fprintf(stdout, "[INIT] SOCKET ADDED TO EPOLL\n");

	fprintf(stdout, "[RUNNING] INITIATION SUCCESSFUL\n");
	uint8_t buf[256];
	while(1){
		int tasks_count = 0;
		task_t** tasks = wait_tasks(epl, &tasks_count);

		if(tasks == NULL) break; 
		for(int i = 0; i < tasks_count; i++){
			if(tasks[i]->state == ACCEPT){	
				accept_handler(server, epl);
				fprintf(stdout, "[RUNNING] CLIENT CONNECTED\n");
			}
			else if(tasks[i]->state == IN){
				read_handler(tasks[i], buf, 256);
				fprintf(stdout, "[RUNNING] CLIENT SENDED SOMETHING\n");
			}
			else if(tasks[i]->state == CLOSED){
 				close_handler(epl, tasks[i]);
 				fprintf(stdout, "[RUNNING] CONNECTION CLOSED BY CLIENT\n");
			}

			else{
				free(tasks[i]);
				fprintf(stderr, "ghost socket with wrong state");
			}
		}
		free(tasks);
	}
	return 0;
}
