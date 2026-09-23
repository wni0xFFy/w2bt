#include <w2bt/network/network.h>
#include <w2bt/core/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/epoll.h>

fd_poll_t* create_sockets_poll(int max_events_count, int timeout){
	fd_poll_t* fdp = malloc(sizeof(fd_poll_t));
	if(fdp == NULL) return NULL;

	fdp->evs = calloc(max_events_count, sizeof(task_t));
	if(fdp->evs == NULL){
		free(fdp);
		return NULL;
	}

	fdp->epoll_fd = epoll_create1(0); // POTENTIAL BUG WITH RACE CONDITIONS IN MULTITHREADING(read internet)
	if(fdp->epoll_fd < 0){
		free(fdp->evs);
		free(fdp);
		return NULL;
	}

	fdp->tasks = calloc(15, sizeof(task_t*));
	if(fdp->tasks == NULL){
		close(fdp->epoll_fd);
		free(fdp->evs);
		free(fdp);
		return NULL;
	}

	fdp->tasks_lenght = 0;
	fdp->tasks_capacity = 15;
	fdp->timeout = timeout;
	fdp->max_events_count = max_events_count;

	return fdp;
}

int delete_sockets_poll(fd_poll_t* epl){
	if(epl == NULL) return -2;
	int e = close(epl->epoll_fd);
	/*POTENTIAL BUG here it can exit from function and doesn't free() memory*/	
	if(e == -1) return -1;
	for(uint32_t i = 0; i < epl->tasks_lenght; i++){
		destroy_socket(epl->tasks[i]->data);
		free(epl->tasks[i]);
	}

	free(epl->tasks);
	free(epl->evs);		
	free(epl);
	return 0;
}

task_t* add_poll_task(fd_poll_t* epl, sock_t* sc, STATES state, uint32_t event){
	if(epl == NULL || sc == NULL) return NULL;
	task_t* task = malloc(sizeof(task_t));
	if(task == NULL) return NULL;

	task->fd = sc->fd;
	task->state = state;
	task->data = sc;

	if(epl->tasks_lenght + 1 >= epl->tasks_capacity){
		task_t** tmp = realloc(epl->tasks, (epl->tasks_capacity + 5) * sizeof(task_t*));
		if(tmp == NULL){
			free(task);
			return NULL;
		}
		epl->tasks = tmp;
		epl->tasks_capacity += 5;
	}

	epl->tasks[epl->tasks_lenght] = task;
	epl->tasks_lenght += 1;

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
		if((ep->evs[i].events &~ EPOLLIN) == EPOLLRDHUP) tasks[i]->state = CLOSED;
	}

	*tasks_count = c;
	return tasks;
}

int delete_task(fd_poll_t* epl, task_t* t){
	fprintf(stdout, "deleting task %x\n", t);
	int is_deleting = 0;
	for(uint32_t i = 1; i < epl->tasks_lenght; i++){
		if(epl->tasks[i] == t){
			is_deleting = 1;
		}
		if(!is_deleting) continue;
		epl->tasks[i] = epl->tasks[i+1];
	}
	if(!is_deleting) return -1;
	epl->tasks_lenght -= 1;
	destroy_socket(t->data);
	free(t);
	return 0;
}
