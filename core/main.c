#include <w2bt/network/network.h>
#include <w2bt/core/queue.h>
#include <w2bt/core/socket.h>	
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

//helper structs(for threads's workers)
typedef struct {
	fd_poll_t* epl;
	sock_t* srv;
	queue_t* qe;
} network_args;

//workers
void* network_worker(void* args){
	network_args* arg = args;
	fd_poll_t* epl = arg->epl;
	sock_t* srv = arg->srv;
	queue_t* qe = arg->qe;
	network_main(epl, srv, qe);
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
	queue_t* qe = create_queue();
	network_args args = {0};
	args.epl = epl;
	args.srv = server;
	args.qe = qe;

	pthread_create(&network_main_thread, NULL, network_worker, &args);
	printf("[Network] Started.\n");

	//waiting
	pthread_join(network_main_thread, NULL);
	//Cleaning

	delete_poll(epl);	
}
