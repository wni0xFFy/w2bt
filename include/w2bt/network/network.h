#pragma once 
#include <stdint.h>
#include <w2bt/core/socket.h>
#include <w2bt/core/queue.h>
typedef enum{
	IN,
	OUT, 
	ACCEPT,
	CLOSED,
	PROGRESS,
} STATES;
 
typedef struct{
	void* data;
	STATES state;
} event_t;

typedef struct{
	int epoll_fd;
	uint32_t max_evs_count; 
	int32_t timeout;
	struct epoll_event* events_buffer;
	event_t** evs;
	uint32_t evs_lenght;
	uint32_t evs_capacity;
} fd_poll_t;

fd_poll_t* create_poll(int evs_count, int timeout);
event_t* add_poll_event(fd_poll_t* epl, sock_t* sc, STATES state, uint32_t event);
int delete_poll(fd_poll_t* epl);
int wait_events(fd_poll_t* ep, event_t** evs);
int delete_event(fd_poll_t* epl, event_t* e);

int network_main(fd_poll_t* epl, sock_t* server, queue_t* qe);