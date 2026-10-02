#include <event_core/poll.h>
#include <core/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>

int event_main(fd_poll_t* epl, queue_t* qe){
	fprintf(stdout, "[Network] init ended\n");

	event_t** evs = calloc(epl->max_evs_count, sizeof(event_t*));
	if(evs == NULL) return -1;

	while(1){
		int c = wait_events(epl, evs);
		for(int i = 0; i < c; i++){
			push_queue(qe, evs[i], NONBLOCK);
		}
	}
	free(evs);
	return 0;
}
