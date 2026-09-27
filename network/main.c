#include <w2bt/network/network.h>
#include <w2bt/core/socket.h>
#include <w2bt/network/handlers.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>

int network_main(fd_poll_t* epl, sock_t* server){
	setbuf(stdout, NULL);
	add_poll_event(epl, server, ACCEPT, EPOLLIN);

	fprintf(stdout, "[INIT] SOCKET ADDED TO EPOLL\n");

	fprintf(stdout, "[RUNNING] INITIATION SUCCESSFUL\n");
	while(1){
		int events_count = 0;
		event_t** evs = wait_events(epl, &events_count);

		if(evs == NULL) break; 
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
				read_handler(evs[i]);
				fprintf(stdout, "[RUNNING] CLIENT SENDED SOMETHING\n");
			}

			else{
				free(evs[i]);
				fprintf(stderr, "ghost socket with wrong state");
			}
		}
		free(evs);
	}
	return 0;
}
