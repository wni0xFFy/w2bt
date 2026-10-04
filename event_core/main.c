#include <event_core/poll.h>
#include <core/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>

int event_main(sock_t* server, fd_poll_t* epl, queue_t* output, queue_t* input){
	fprintf(stdout, "[Network] init ended\n");

	event_t** evs = calloc(epl->max_evs_count, sizeof(event_t*));
	if(evs == NULL) return -1;

	while(1){
		int c = wait_events(epl, evs);
		for(int i = 0; i < c; i++) {	
			if(evs[i]->fd == input->wakeup_fd){
				event_t* ev = pop_queue(input, LOCK);
				if(ev != NULL){
					if(add_poll_event(epl, ev->sc, ev->state, 0) != NULL) fprintf(stderr, "client on port %d\n", ev->sc->fd);
				}	
				continue;
			}

			switch(evs[i]->state){
				case CLOSABLE:
					fprintf(stderr, "close\n");
					destroy_socket(evs[i]->sc);
					delete_event(epl, evs[i]);
					break;	
				case ACCEPTABLE:
					fprintf(stderr, "accept\n");
					sock_t* client = accept_socket(server);
					add_poll_event(epl, client, READABLE, 0);
					break;
			}

 			//push_queue(output, evs[i], LOCK);
		}

	}
	free(evs);
	return 0;
}
