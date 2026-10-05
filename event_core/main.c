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
			if(evs[i]->fd != input->wakeup_fd){
				if(evs[i]->state == ACCEPTABLE) {fprintf(stderr, "hi bitcha");}
				push_queue(output, evs[i], LOCK);
				fprintf(stderr, "\nwrong events count : %d\n", output->nodes_count);
				
				continue;
			}
			fprintf(stderr, "\nevents count : %d\n", input->nodes_count);

			event_t* ev = pop_queue(input, NONBLOCK); //THIS SHIT EQUAL TO NULL IDK WHY
			if(ev == NULL) continue;	

			if(ev->state == READABLE){add_poll_event(epl, ev->sc, READABLE, 0, EPOLLONESHOT); fprintf(stdout, "connected.");}
			if(ev->state == CLOSABLE) delete_event(epl, ev);
		}

	}
	free(evs);
	return 0;
}
