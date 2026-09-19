#include <w2bt/network/network.h>
#include <w2bt/core/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/epoll.h>

fd_poll_t* create_sockets_poll(int max_events_count, int timeout){
	fd_poll_t* fdp = malloc(sizeof(fd_poll_t));
	if(fdp == NULL) return NULL;

	fdp->evs = malloc(sizeof(task_t) * max_events_count);
	if(fdp->evs == NULL) return NULL;

	fdp->epoll_fd = epoll_create1(0); // POTENTIAL BUG WITH RACE CONDITIONS IN MULTITHREADING(read internet)
	if(fdp->epoll_fd < 0){
		free(fdp->evs);
		free(fdp);
		return NULL;
	}

	fdp->timeout = timeout;
	fdp->max_events_count = max_events_count;

	return fdp;
}

int delete_sockets_poll(fd_poll_t* epl){
	if(epl == NULL) return -2;
	int e = close(epl->epoll_fd);
	/*POTENTIAL BUG here it can exit from function and doesn't free() memory*/	
	if(e == -1) return -1;
	free(epl->evs);		
	free(epl);

	return 0;
}

task_t* add_socket_event(fd_poll_t* epl, sock_t* sc, STATES state, uint32_t event){
	if(epl == NULL || sc == NULL) return NULL;
	task_t* task = malloc(sizeof(task_t));
	if(task == NULL) return NULL;

	task->fd = sc->fd;
	task->state = state;
	task->data = sc;

	struct epoll_event ev;
	ev.events = event;
	ev.data.ptr = task;

	if(epoll_ctl(epl->epoll_fd, EPOLL_CTL_ADD, sc->fd, &ev)) return NULL;
	return task;
}

task_t** wait_tasks(fd_poll_t* ep, int* tasks_count){
	task_t** tasks = NULL;
	int c = epoll_wait(ep->epoll_fd, ep->evs, ep->max_events_count, ep->timeout);
	if(c <= 0) return NULL;

	tasks = malloc(sizeof(task_t*) * c);
	if(tasks == NULL) return NULL;

	for(int i = 0; i < c; i++){
		tasks[i] = ep->evs[i].data.ptr;
	}

	*tasks_count = c;
	return tasks;
}
