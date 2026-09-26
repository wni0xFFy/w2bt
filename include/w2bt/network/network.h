#pragma once 
#include <stdint.h>
#include <w2bt/core/socket.h>

typedef enum{
	IN,
	OUT, 
	ACCEPT,
	CLOSED,
} STATES;

typedef struct{
	int fd;
	void* data;
	STATES state;
} task_t;

typedef struct{
	int epoll_fd;
	uint32_t max_events_count; 
	int32_t timeout;
	struct epoll_event* evs;
	task_t** tasks;
	uint32_t tasks_lenght;
	uint32_t tasks_capacity;
} fd_poll_t;

fd_poll_t* create_poll(int events_count, int timeout);
task_t* add_poll_task(fd_poll_t* epl, sock_t* sc, STATES state, uint32_t event);
int delete_poll(fd_poll_t* epl);
task_t** wait_tasks(fd_poll_t* ep, int* tasks_count);
int delete_task(fd_poll_t* epl, task_t* task);

int network_main(fd_poll_t* epl, sock_t* server);