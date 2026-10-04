#include <event_core/poll.h>
#include <core/queue.h>
#include <core/socket.h>	
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

//helper structs(for threads's workers)
typedef struct {
	fd_poll_t* epl;
	queue_t* qe;
	queue_t* eqe;
	sock_t* srv;
} network_args;

//workers
void* network_worker(void* args){
	network_args* arg = args;
	fd_poll_t* epl = arg->epl;
	queue_t* qe = arg->qe;
	queue_t* eqe = arg->eqe;
	sock_t* srv = arg->srv;
	event_main(srv, epl, qe, eqe);
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
	network_args args = {0};

	args.epl = epl;
	args.qe = qe;
	args.eqe = eqe;
	args.srv = server;
	add_poll_event(epl, server, ACCEPTABLE, 0);
	add_poll_event(epl, NULL, READABLE, eqe->wakeup_fd);
	add_poll_event(epl, NULL, READABLE, qe->wakeup_fd);

	pthread_create(&network_main_thread, NULL, network_worker, &args);
	printf("[Network] Started.\n");

	fd_poll_t* events_poll = create_poll(15, 30000);
	add_poll_event(events_poll, NULL, READABLE, qe->wakeup_fd);
	event_t** evs = calloc(events_poll->max_evs_count, sizeof(event_t*));
	if(evs == NULL) return -1;

	while(1){
		int c = wait_events(events_poll, evs);
		if(c < 1) continue;
		event_t* ev = pop_queue(qe, NONBLOCK);
		if(ev == NULL) continue;

		fprintf(stderr, "\nNow processing event on socket : ");
		sock_t* client;
		switch(ev->state){
			case READABLE:
				fprintf(stderr, "close\n");
				destroy_socket(ev->sc);
				delete_event(epl, ev);
				break;	
			case ACCEPTABLE:
				fprintf(stderr, "accept\n");
				client = accept_socket(server);
				event_t* eev = malloc(sizeof(event_t)); 
				eev->sc = client;
				eev->state = READABLE;
				push_queue(eqe, eev, LOCK); 
				//destroy_socket(client);
				break;
		}
	}
	free(evs);
	//waiting
	pthread_join(network_main_thread, NULL);
	//Cleaning
	delete_poll(epl);	
	destroy_socket(server);
}