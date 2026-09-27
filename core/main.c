// #include <w2bt/network/network.h>
// #include <w2bt/core/socket.h>	
// #include <stdio.h>
// #include <stdlib.h>
// #include <pthread.h>

// //helper structs(for threads's workers)
// typedef struct {
// 	fd_poll_t* epl;
// 	sock_t* srv;
// } network_args;

// //workers
// void* network_worker(void* args){
// 	network_args* arg = args;
// 	fd_poll_t* epl = arg->epl;
// 	sock_t* srv = arg->srv;

// 	network_main(epl, srv);
// }

// int main(){
// 	//Network Module Starting
// 	pthread_t network_main_thread;
// 	fprintf(stderr, "[Network] Startked.\n");
// 	fd_poll_t* epl = create_poll(15, 30000);
// 	if(epl == NULL){
// 		printf("[ERROR] EPOLL NOT STARTED");
// 		return -1;
// 	}
// 	sock_t* server = create_socket(25656, 10);
// 	if(server == NULL){
// 		printf("[ERROR] SOCKET NOT OPENED");
// 		return -2;
// 	}
// 	network_args args = {0};
// 	args.epl = epl;
// 	args.srv = server;

// 	pthread_create(&network_main_thread, NULL, network_worker, &args);
// 	printf("[Network] Started.\n");

// 	//waiting
// 	pthread_join(network_main_thread, NULL);
// 	//Cleaning
// 	delete_poll(epl);	
// }

#include <w2bt/transport/queue.h>
#include <stdio.h>
#include <stdlib.h>
int main(){
	int* x = calloc(4,1); *x = 0;
	int* y = calloc(4,1); *y = 1;
	int* z = calloc(4,1); *z = 2;
	int* b = calloc(4,1); *b = 3;

	queue_t* qe = create_queue();

	push_front(qe,x);
	push_front(qe,y);
	push_front(qe,z);
	push_front(qe,b);
	pop_back(qe);
	node_t* tmp = qe->start->next;
	for(int i = 0; i < qe->nodes_count; i++){
		int* ptr = tmp->ptr;
		printf("%d\n", *ptr);
		tmp = tmp->next;
	}
}