#include <core/queue.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

queue_t* create_queue(){
	queue_t* qe = calloc(1, sizeof(queue_t));
	if(qe == NULL) return NULL;

	qe->nodes_count = 0;
	qe->start = calloc(1, sizeof(node_t));
	qe->start->next = NULL;
	qe->start->ptr = NULL;
	pthread_mutex_init(&qe->mtx, NULL);
	return qe;
}

int push_queue(queue_t* qe, void* ptr, queue_param_t param){
	if(qe == NULL) return -1;
	if(param != LOCK){
		if(pthread_mutex_trylock(&qe->mtx) != 0) return 1;
	}
	if(param == LOCK){
		pthread_mutex_lock(&qe->mtx);
	}

	node_t* nd = calloc(1, sizeof(node_t));
	if(nd == NULL){
		pthread_mutex_unlock(&qe->mtx);
	 	return -1;
	}

	nd->ptr = ptr;
	nd->next = qe->start->next;
	qe->start->next = nd;
	qe->nodes_count++;

	pthread_mutex_unlock(&qe->mtx);

	return 0;
}

void* pop_queue(queue_t* qe, queue_param_t param){
	if(qe->nodes_count == 0) return NULL;
	if(param != LOCK){
		if(pthread_mutex_trylock(&qe->mtx) != 0) return NULL;
	}
	if(param == LOCK){
		pthread_mutex_lock(&qe->mtx);
	}

	void* return_ptr = NULL;
	node_t* tmp = qe->start;

	//getting PRE-END node
	for(uint32_t i = 0; i < qe->nodes_count - 1; i++) tmp = tmp->next;

	return_ptr = tmp->next->ptr;
	free(tmp->next);
	tmp->next = NULL;
	qe->nodes_count--;	
	pthread_mutex_unlock(&qe->mtx);

	return return_ptr;
}

int destroy_queue(queue_t* q){
	if(q->nodes_count < 0) return -1;
	free(q->start);
	pthread_mutex_destroy(&q->mtx);
	free(q);
	return 0;
}