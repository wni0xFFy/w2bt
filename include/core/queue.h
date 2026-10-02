#pragma once

#include <stdint.h>
#include <pthread.h>

typedef struct node_t{
	void* ptr;
	struct node_t* next;
} node_t;

typedef struct {
	node_t* start;
	uint32_t nodes_count;
	pthread_mutex_t mtx;
} queue_t;

queue_t* create_queue();
int destroy_queue(queue_t* q);

int push_queue(queue_t* qe, void* ptr);
void* pop_queue(queue_t* qe);