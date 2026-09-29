#include <w2bt/network/network.h>
#include <w2bt/core/socket.h>
#include <w2bt/core/queue.h>

int accept_handler(sock_t* serv, fd_poll_t* epfd);
int close_handler(fd_poll_t* epl, event_t* e);

int read_handler(event_t* e, queue_t* qe);
int write_handler(event_t* e, queue_t* qe);
