#pragma once 
#include <stdint.h>
#include <core/socket.h>
#include <core/queue.h>

typedef enum{
	READABLE = 1,
	WRITABLE, 
	ACCEPTABLE,
	CLOSABLE,
	CLOSING,
	READING,
	ACCEPTING
} STATES;
 
typedef struct{
	int fd;
	sock_t* sc;
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
event_t* add_poll_event(fd_poll_t* epl, sock_t* sc, STATES state, int fd, uint32_t mask);
int delete_poll(fd_poll_t* epl);
int wait_events(fd_poll_t* ep, event_t** evs);
int delete_event(fd_poll_t* epl, event_t* e);
int event_up(fd_poll_t* epl, event_t* e, uint32_t mask);

int event_main(sock_t* server, fd_poll_t* epl, queue_t* output, queue_t* input);