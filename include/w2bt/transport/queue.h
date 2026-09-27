#include <stdint.h>

typedef struct node_t{
	void* ptr;
	struct node_t* next;
} node_t;

//linked list for tasks.
typedef struct {
	node_t* start;
	uint32_t nodes_count;
} queue_t;

queue_t* create_queue();
int push_front(queue_t* qe, void* ptr);
void* pop_back(queue_t* qe);