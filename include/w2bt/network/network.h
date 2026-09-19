#pragma once 
#include <stdint.h>
#include <w2bt/core/socket.h>

typedef enum{
	IN,
	OUT, 
	ACCEPT,
} STATES;

typedef struct{
	int fd;
	void* data;
	STATES state;
} task_t;

typedef struct{
	int epoll_fd;
	uint16_t max_events_count; 
	int16_t timeout;
	struct epoll_event* evs;
} fd_poll_t;

fd_poll_t* create_sockets_poll(int events_count, int timeout);
task_t* add_socket_event(fd_poll_t* epl, sock_t* sc, STATES state, uint32_t event);
int delete_sockets_poll(fd_poll_t* epl);
task_t** wait_tasks(fd_poll_t* ep, int* tasks_count);

