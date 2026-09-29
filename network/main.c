#include <w2bt/network/network.h>
#include <w2bt/core/socket.h>
#include <w2bt/network/handlers.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>

int network_main(fd_poll_t* epl, sock_t* server, queue_t* qe){
	setbuf(stdout, NULL);
	add_poll_event(epl, server, ACCEPT, EPOLLIN);

	fprintf(stdout, "[INIT] SOCKET ADDED TO EPOLL\n");

	fprintf(stdout, "[RUNNING] INITIATION SUCCESSFUL\n");

	event_t** evs = calloc(epl->max_evs_count, sizeof(event_t*));
	if(evs == NULL) return -1;

	while(1){
		int events_count = wait_events(epl, evs);

		for(int i = 0; i < events_count; i++){
			if(evs[i]->state == ACCEPT){	
				accept_handler(server, epl);
				fprintf(stdout, "[RUNNING] CLIENT CONNECTED\n");
			}
			else if(evs[i]->state == CLOSED){
 				close_handler(epl, evs[i]);
 				fprintf(stdout, "[RUNNING] CONNECTION CLOSED BY CLIENT\n");
			}
			
			else if(evs[i]->state == IN){
				read_handler(evs[i], qe);
				fprintf(stdout, "[RUNNING] CLIENT SENDED SOMETHING\n");
			}

			else if(evs[i]->state == OUT){
				write_handler(evs[i], qe);
				fprintf(stdout, "[RUNNING] CLIENT READED SOMETHING\n");
			}

			else if(evs[i]->state == PROGRESS);
			else{
				free(evs[i]);
				fprintf(stderr, "ghost socket with wrong state");
			}
		}
	}
	free(evs);
	return 0;
}
