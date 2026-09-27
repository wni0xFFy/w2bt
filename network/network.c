#include <w2bt/network/network.h>
#include <w2bt/core/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/epoll.h>

fd_poll_t* create_poll(int max_evs_count, int timeout){
	fd_poll_t* fdp = malloc(sizeof(fd_poll_t));
	if(fdp == NULL) return NULL;

	fdp->events_buffer = calloc(max_evs_count, sizeof(event_t));
	if(fdp->events_buffer == NULL){
		free(fdp);
		return NULL;
	}

	fdp->epoll_fd = epoll_create1(0); // POTENTIAL BUG WITH RACE CONDITIONS IN MULTITHREADING(read internet)
	if(fdp->epoll_fd < 0){
		free(fdp->events_buffer);
		free(fdp);
		return NULL;
	}

	fdp->evs= calloc(15, sizeof(event_t*));
	if(fdp->evs == NULL){
		close(fdp->epoll_fd);
		free(fdp->events_buffer);
		free(fdp);
		return NULL;
	}

	fdp->evs_lenght = 0;
	fdp->evs_capacity = 15;
	fdp->timeout = timeout;
	fdp->max_evs_count = max_evs_count;

	return fdp;
}

int delete_poll(fd_poll_t* epl){
	if(epl == NULL) return -2;
	int e = close(epl->epoll_fd);
	/*POTENTIAL BUG here it can exit from function and doesn't free() memory*/	
	if(e == -1) return -1;
	for(uint32_t i = 0; i < epl->evs_lenght; i++){
		destroy_socket(epl->evs[i]->data);
		free(epl->evs[i]);
	}

	free(epl->evs);
	free(epl->events_buffer);		
	free(epl);
	return 0;
}

event_t* add_poll_event(fd_poll_t* epl, sock_t* sc, STATES state, uint32_t event){
	if(epl == NULL || sc == NULL) return NULL;
	event_t* e = malloc(sizeof(event_t));
	if(e == NULL) return NULL;
	
	e->state = state;
	e->data = sc;

	if(epl->evs_lenght + 1 >= epl->evs_capacity){
		event_t** tmp = realloc(epl->evs, (epl->evs_capacity + 5) * sizeof(event_t*));
		if(tmp == NULL){
			free(e);
			return NULL;
		}
		epl->evs = tmp;
		epl->evs_capacity += 5;
	}

	epl->evs[epl->evs_lenght] = e;
	epl->evs_lenght += 1;

	struct epoll_event ev;
	ev.events = event;
	ev.data.ptr = e;

	if(epoll_ctl(epl->epoll_fd, EPOLL_CTL_ADD, sc->fd, &ev)) return NULL;
	return e;
}

int wait_events(fd_poll_t* ep, event_t** evs){
	if(ep == NULL || evs == NULL) return -1;
	int c = epoll_wait(ep->epoll_fd, ep->events_buffer, ep->max_evs_count, ep->timeout);
	if(c <= 0) return -2;

	for(int i = 0; i < c; i++){
		evs[i] = ep->events_buffer[i].data.ptr;
		if((ep->events_buffer[i].events &~ EPOLLIN) == EPOLLRDHUP) evs[i]->state = CLOSED;
	}

	return c;
}

int delete_event(fd_poll_t* epl, event_t* t){
	int is_deleting = 0;
	for(uint32_t i = 1; i < epl->evs_lenght; i++){
		if(epl->evs[i] == t){
			is_deleting = 1;
		}
		if(!is_deleting) continue;
		epl->evs[i] = epl->evs[i+1];
	}
	if(!is_deleting) return -1;
	epl->evs_lenght -= 1;
	destroy_socket(t->data);
	free(t);
	return 0;
}
