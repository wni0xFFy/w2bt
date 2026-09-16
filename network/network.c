#include <w2bt/network.h>
#include <w2bt/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/epoll.h>


fd_poll_t* create_sockets_poll(int events_count){
	fd_poll_t* fdp = malloc(sizeof(fd_poll_t));
	fdp->epoll_fd = epoll_create1(0);
	fdp->timeout = 30000;
	if(fdp->epoll_fd == -1){
		free(fdp);
		return NULL;
	}
	fdp->evs = malloc(sizeof(task_t) * events_count);
	fdp->events_count = events_count;
	return fdp;
}

void delete_sockets_poll(fd_poll_t* epl){
	close(epl->epoll_fd);
	free(epl->evs);
	free(epl);
}

int add_socket_event(fd_poll_t* epl, sock_t* sc, STATES state, uint32_t event){
	task_t* task = malloc(sizeof(task_t));
	task->fd = sc->fd;
	task->state = state;
	task->data = sc;

	struct epoll_event ev;
	ev.events = event;
	ev.data.ptr = task;

	if(epoll_ctl(epl->epoll_fd, EPOLL_CTL_ADD, sc->fd, &ev)){
		return -1;
	}

	return 0;
}

task_t** wait_tasks(fd_poll_t* ep, int* size){
	task_t** tasks = NULL;
	int c = epoll_wait(ep->epoll_fd, ep->evs, ep->events_count, ep->timeout);
	if(c <= 0) return NULL;
	tasks = malloc(sizeof(task_t*) * c);
	for(int i = 0; i < c; i++){
		tasks[i] = ep->evs[i].data.ptr;
	}
	*size = c;
	return tasks;
}
