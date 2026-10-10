#include <event_core/poll.h>
#include <core/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <core/commands.h>

int event_main(sock_t* server, fd_poll_t* epl, queue_t* output, queue_t* input, queue_t* command_queue){
	fprintf(stdout, "[Network] init ended\n");

	event_t** evs = calloc(epl->max_evs_count, sizeof(event_t*));
	if(evs == NULL) return -1;

	int running = 1;
	while(running){
		int c = wait_events(epl, evs);
		for(int i = 0; i < c; i++) {
			if(evs[i]->fd == input->wakeup_fd ){ 
				event_t* ev = pop_queue(input, NONBLOCK); 
				if(ev == NULL) continue;

				if(ev->active == 0) {event_up(epl, ev, EPOLLONESHOT); continue;}

				switch(ev->state){
					case READABLE: 
						add_poll_event(epl, ev->sc, READABLE, 0, EPOLLONESHOT); 
						fprintf(stdout, "connected.\n");
						break;
					case CLOSABLE:
						delete_event(epl, ev); 
						fprintf(stderr, "closed\n");
						break;
				}		
			}

			else if(evs[i]->fd == command_queue->wakeup_fd){	
				command_t* cmd = pop_queue(command_queue, NONBLOCK);
				if(cmd == NULL) continue;
				switch(cmd->command){
					case SHUTDOWN:
						free(evs);
						return 0;
				}
			}

			else{
				if(evs[i]->state == ACCEPTABLE) {
						fprintf(stderr, "accept\n");
						sock_t* client = accept_socket(server);
						event_t* eev = malloc(sizeof(event_t)); 
						eev->sc = client;
						eev->state = READABLE;	
						eev->active = 1;
						if(push_queue(input, eev, LOCK) != 0) fprintf(stderr, "Helsinki\n"); 
						continue; 
				}
				push_queue(output, evs[i], LOCK); 
			}
		}
	}
	free(evs);
	return 0;
}
