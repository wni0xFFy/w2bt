#include <w2bt/transport/queue.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
queue_t* create_queue(){
	queue_t* qe = calloc(1, sizeof(queue_t));
	if(qe == NULL) return NULL;

	qe->nodes_count = 0;
	qe->start = calloc(1, sizeof(node_t));
	qe->start->next = NULL;
	qe->start->ptr = NULL;
	return qe;
}

int push_front(queue_t* qe, void* ptr){
	if(qe == NULL) return -2;
	node_t* nd = calloc(1, sizeof(node_t));
	if(nd == NULL) return -1;

	nd->ptr = ptr;
	nd->next = qe->start->next;
	qe->start->next = nd;
	qe->nodes_count++;
}

void* pop_back(queue_t* qe){
	node_t* tmp = qe->start->next;
	for(uint32_t i = 0; i <= qe->nodes_count; i++){
		if(tmp->next->next == NULL){
			void* ptr = tmp->next->ptr;
			qe->nodes_count--;
			tmp->next = NULL;
			return ptr;
		}
		tmp = tmp->next;
	}
}