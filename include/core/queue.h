#pragma once

#include <stdint.h>
#include <pthread.h>

typedef enum{
	LOCK,
	NONBLOCK,
} queue_param_t;

typedef struct node_t{
	void* ptr;
	struct node_t* next;
} node_t;

typedef struct {
	node_t* start;
	uint32_t nodes_count;
	pthread_mutex_t mtx;
	int wakeup_fd;
} queue_t;

queue_t* create_queue(int is_event);
int destroy_queue(queue_t* q);

int push_queue(queue_t* qe, void* ptr, queue_param_t param);
void* pop_queue(queue_t* qe, queue_param_t param);