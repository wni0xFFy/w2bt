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
} network_args;

//workers
void* network_worker(void* args){
	network_args* arg = args;
	fd_poll_t* epl = arg->epl;
	queue_t* qe = arg->qe;
	event_main(epl, qe);
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

	add_poll_event(epl, server, ACCEPTABLE);

	queue_t* qe = create_queue();
	network_args args = {0};

	args.epl = epl;
	args.qe = qe;

	pthread_create(&network_main_thread, NULL, network_worker, &args);
	printf("[Network] Started.\n");

	while(1){
		event_t* ev = pop_queue(qe, LOCK);
		if(ev == NULL) continue;
		sock_t* client;
		switch(ev->state){
			case ACCEPTABLE:
				fprintf(stderr, "accept\n");
				client = accept_socket(server);
				if(add_poll_event(epl, client, READABLE) == NULL) fprintf(stderr, "idk");
				//destroy_socket(client);
				break;
			case CLOSABLE:
				fprintf(stderr, "close\n");
				destroy_socket(ev->sc);
				delete_event(epl, ev);
				break;
		}

	}
	//waiting
	pthread_join(network_main_thread, NULL);
	//Cleaning
	delete_poll(epl);	
	destroy_socket(server);
}
