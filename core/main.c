#include <event_core/poll.h>
#include <core/queue.h>
#include <core/socket.h>	
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/epoll.h>
#include <core/commands.h>
#include <string.h>
//helper structs(for threads's workers)
typedef struct {
	fd_poll_t* epl;
	queue_t* qe;
	queue_t* eqe;
	sock_t* srv;
	queue_t* qel;
} network_args;

//workers
void* network_worker(void* args){
	network_args* arg = args;
	fd_poll_t* epl = arg->epl;
	queue_t* qe = arg->qe; 
	queue_t* eqe = arg->eqe;
	queue_t* qel = arg->qel;
	sock_t* srv = arg->srv;
	event_main(srv, epl, qe, eqe, qel);
	return 0;
}

int main(){
	//Network Module Starting
	pthread_t network_main_thread;
	fd_poll_t* epl = create_poll(15, 30000);
	if(epl == NULL){
		printf("[ERROR] EPOLL NOT STARTED");
		return -1;
	}
	sock_t* server = create_socket(25656, 10);
	if(server == NULL){
		printf("[ERROR] SOCKET NOT OPENED");
		return -2;
	}
     
	queue_t* qe = create_queue(1);
	queue_t* eqe = create_queue(1);
	queue_t* commands = create_queue(1);
	network_args args = {0};

	args.epl = epl;
	args.qe = qe;
	args.eqe = eqe;
	args.srv = server;
	args.qel = commands;

	add_poll_event(epl, server, ACCEPTABLE, 0, 0);
	add_poll_event(epl, NULL, READABLE, eqe->wakeup_fd, 0);
	add_poll_event(epl, NULL, READABLE, commands->wakeup_fd, 0);

	pthread_create(&network_main_thread, NULL, network_worker, &args);
	printf("[Network] Started.\n");

	fd_poll_t* events_poll = create_poll(15, 10);
	add_poll_event(events_poll, NULL, READABLE, qe->wakeup_fd,0);
	event_t** evs = calloc(events_poll->max_evs_count, sizeof(event_t*));
	if(evs == NULL) return -1;

	int running = 1;
	while(running){
		int c = wait_events(events_poll, evs);
		if(c < 1) continue;
		event_t* ev = pop_queue(qe, LOCK);
		if(ev == NULL) continue;

		event_t* eev;

		switch(ev->state){
			case CLOSABLE:
				fprintf(stderr, "close\n");
				eev = malloc(sizeof(event_t)); 
				eev->sc = ev->sc;
				eev->state = CLOSABLE;	
				eev->active = 1;
				destroy_socket(ev->sc);
				push_queue(eqe, eev, LOCK); 
				break;	
			case READABLE:
				char buffer[256] = {0};
				int readed = receive_socket(ev->sc, buffer, 16, 0);
				buffer[readed] = '\0';
				fprintf(stderr, "from client : %s\n", buffer);
				send_socket(ev->sc, buffer, 16, 0);
				ev->active = 0;
				if(strcmp(buffer, "stop\n") == 0){
					fprintf(stderr, "stop");
					command_t* cmd = malloc(sizeof(command_t));
					cmd->command = SHUTDOWN;	
					push_queue(commands, cmd, LOCK);
					running = 0;
				}
				push_queue(eqe, ev, LOCK); 
				break;
		}
	}
	free(evs);
	//waiting
	pthread_join(network_main_thread, NULL);
	//Cleaning
	//delete_poll(epl);	
	destroy_socket(server);
}
